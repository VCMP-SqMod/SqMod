#ifndef _SQTCC_COMMON_HPP_
#define _SQTCC_COMMON_HPP_

// ------------------------------------------------------------------------------------------------
#include "ModBase.hpp"

// ------------------------------------------------------------------------------------------------
#include <cassert>

// ------------------------------------------------------------------------------------------------
#include <vector>

// ------------------------------------------------------------------------------------------------
#include <vcmp.h>
#include <sqrat.h>

// ------------------------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

struct TCCState;

#ifdef __cplusplus
} // extern "C"
#endif

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * SOFTWARE INFORMATION
*/
#define SQTCC_NAME "Squirrel TCC Module"
#define SQTCC_AUTHOR "Sandu Liviu Catalin (S.L.C)"
#define SQTCC_COPYRIGHT "Copyright (C) 2016 Sandu Liviu Catalin"
#define SQTCC_HOST_NAME "SqModTCCHost"
#define SQTCC_VERSION 001
#define SQTCC_VERSION_STR "0.0.1"
#define SQTCC_VERSION_MAJOR 0
#define SQTCC_VERSION_MINOR 0
#define SQTCC_VERSION_PATCH 1

/* ------------------------------------------------------------------------------------------------
 * Retrieve the temporary buffer.
*/
SStr GetTempBuff();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the size of the temporary buffer.
*/
Uint32 GetTempBuffSize();

/* ------------------------------------------------------------------------------------------------
 * Throw a formatted exception.
*/
void SqThrowF(CSStr str, ...);

/* ------------------------------------------------------------------------------------------------
 * Generate a formatted string.
*/
CSStr FmtStr(CSStr str, ...);

/* ------------------------------------------------------------------------------------------------
 * Implements RAII to restore the VM stack to it's initial size on function exit.
*/
struct StackGuard
{
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    StackGuard();

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    StackGuard(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~StackGuard();

private:

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    StackGuard(const StackGuard &);

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    StackGuard(StackGuard &&);

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    StackGuard & operator = (const StackGuard &);

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    StackGuard & operator = (StackGuard &&);

private:

    // --------------------------------------------------------------------------------------------
    HSQUIRRELVM m_VM; // The VM where the stack should be restored.
    Int32       m_Top; // The top of the stack when this instance was created.
};

/* ------------------------------------------------------------------------------------------------
 * Helper structure for retrieving a value from the stack as a string or a formatted string.
*/
struct StackStrF
{
    // --------------------------------------------------------------------------------------------
    CSStr       mPtr; // Pointer to the C string that was retrieved.
    SQInteger   mLen; // The string length if it could be retrieved.
    SQRESULT    mRes; // The result of the retrieval attempts.
    HSQOBJECT   mObj; // Strong reference to the string object.
    HSQUIRRELVM mVM; // The associated virtual machine.

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    StackStrF(HSQUIRRELVM vm, SQInteger idx, bool fmt = true);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    StackStrF(const StackStrF & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    StackStrF(StackStrF && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~StackStrF();

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    StackStrF & operator = (const StackStrF & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    StackStrF & operator = (StackStrF && o) = delete;
};

// ------------------------------------------------------------------------------------------------
class State;
class Symbol;
class StateHnd;

/* ------------------------------------------------------------------------------------------------
 * Register the standard API on the specified compiler state.
*/
extern void RegisterAPI(StateHnd & s);

/* ------------------------------------------------------------------------------------------------
 * Server events that can be toggled.
*/
enum EventType
{
    EvUnknown = 0,
    EvOnFrame,
    EvOnPlayerConnect,
    EvOnPlayerDisconnect,
    EvOnPlayerBeginTyping,
    EvOnPlayerEndTyping,
    EvOnPlayerRequestClass,
    EvOnPlayerRequestSpawn,
    EvOnPlayerSpawn,
    EvOnPlayerDeath,
    EvOnPlayerUpdate,
    EvOnPlayerRequestEnter,
    EvOnPlayerEnterVehicle,
    EvOnPlayerExitVehicle,
    EvOnPlayerNameChange,
    EvOnPlayerStateChange,
    EvOnPlayerActionChange,
    EvOnPlayerOnFireChange,
    EvOnPlayerCrouchChange,
    EvOnPlayerGameKeysChange,
    EvOnPickupClaimPicked,
    EvOnPickupPickedUp,
    EvOnPickupRespawn,
    EvOnVehicleUpdate,
    EvOnVehicleExplode,
    EvOnVehicleRespawn,
    EvOnObjectShot,
    EvOnObjectBump,
    EvOnPublicMessage,
    EvOnCommandMessage,
    EvOnPrivateMessage,
    EvOnLoginAttempt,
    EvOnEntityPoolChange,
    EvOnKeyBindDown,
    EvOnKeyBindUp,
    EvOnPlayerAwayChange,
    EvOnPlayerSpectate,
    EvOnPlayerCrashReport,
    EvOnServerPerformanceReport,
    EvOnCheckpointEntered,
    EvOnCheckpointExited,
    EvOnSphereEntered,
    EvOnSphereExited,
    EvCount
};

/* ------------------------------------------------------------------------------------------------
 * Manages a reference counted compiler state handle.
*/
class StateHnd
{
    // --------------------------------------------------------------------------------------------
    friend class State;
    friend class Symbol;

public:

    // --------------------------------------------------------------------------------------------
    typedef TCCState        Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*           Pointer; // Pointer to the managed type.
    typedef const Type*     ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&           Reference; // Reference to the managed type.
    typedef const Type&     ConstRef; // Constant reference to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef unsigned int    Counter; // Reference counter type.

    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and throw exception if invalid.
    */
    void Validate() const;

protected:

    /* --------------------------------------------------------------------------------------------
     * The structure that holds the data associated with a certain state.
    */
    struct Handle
    {
        // ----------------------------------------------------------------------------------------
        Pointer     mPtr; // The TCC state handle resource.
        Counter     mRef; // Reference count to the managed handle.

        // ----------------------------------------------------------------------------------------
        SDK_OnInitServer                mOnInitServer;
        SDK_OnShutdownServer            mOnShutdownServer;
        SDK_OnFrame                     mOnFrame;
        SDK_OnPlayerConnect             mOnPlayerConnect;
        SDK_OnPlayerDisconnect          mOnPlayerDisconnect;
        SDK_OnPlayerBeginTyping         mOnPlayerBeginTyping;
        SDK_OnPlayerEndTyping           mOnPlayerEndTyping;
        SDK_OnPlayerRequestClass        mOnPlayerRequestClass;
        SDK_OnPlayerRequestSpawn        mOnPlayerRequestSpawn;
        SDK_OnPlayerSpawn               mOnPlayerSpawn;
        SDK_OnPlayerDeath               mOnPlayerDeath;
        SDK_OnPlayerUpdate              mOnPlayerUpdate;
        SDK_OnPlayerRequestEnter        mOnPlayerRequestEnter;
        SDK_OnPlayerEnterVehicle        mOnPlayerEnterVehicle;
        SDK_OnPlayerExitVehicle         mOnPlayerExitVehicle;
        SDK_OnPlayerNameChange          mOnPlayerNameChange;
        SDK_OnPlayerStateChange         mOnPlayerStateChange;
        SDK_OnPlayerActionChange        mOnPlayerActionChange;
        SDK_OnPlayerOnFireChange        mOnPlayerOnFireChange;
        SDK_OnPlayerCrouchChange        mOnPlayerCrouchChange;
        SDK_OnPlayerGameKeysChange      mOnPlayerGameKeysChange;
        SDK_OnPickupClaimPicked         mOnPickupClaimPicked;
        SDK_OnPickupPickedUp            mOnPickupPickedUp;
        SDK_OnPickupRespawn             mOnPickupRespawn;
        SDK_OnVehicleUpdate             mOnVehicleUpdate;
        SDK_OnVehicleExplode            mOnVehicleExplode;
        SDK_OnVehicleRespawn            mOnVehicleRespawn;
        SDK_OnObjectShot                mOnObjectShot;
        SDK_OnObjectBump                mOnObjectBump;
        SDK_OnPublicMessage             mOnPublicMessage;
        SDK_OnCommandMessage            mOnCommandMessage;
        SDK_OnPrivateMessage            mOnPrivateMessage;
        SDK_OnInternalCommand           mOnInternalCommand;
        SDK_OnLoginAttempt              mOnLoginAttempt;
        SDK_OnEntityPoolChange          mOnEntityPoolChange;
        SDK_OnKeyBindDown               mOnKeyBindDown;
        SDK_OnKeyBindUp                 mOnKeyBindUp;
        SDK_OnPlayerAwayChange          mOnPlayerAwayChange;
        SDK_OnPlayerSpectate            mOnPlayerSpectate;
        SDK_OnPlayerCrashReport         mOnPlayerCrashReport;
        SDK_OnServerPerformanceReport   mOnServerPerformanceReport;
        SDK_OnCheckpointEntered         mOnCheckpointEntered;
        SDK_OnCheckpointExited          mOnCheckpointExited;
        SDK_OnSphereEntered             mOnSphereEntered;
        SDK_OnSphereExited              mOnSphereExited;

        // ----------------------------------------------------------------------------------------
        bool        mRelocated; // Whether the state was relocated.
        bool        mNoErrors; // Don't output errors even if there is no error handler.

        // ----------------------------------------------------------------------------------------
        Function    mErrorHandler; // The custom script error handler.

        // ----------------------------------------------------------------------------------------
        String      mName;  // The name given to this state.

        /* ----------------------------------------------------------------------------------------
         * Base constructor.
        */
        Handle(Counter counter, CSStr name);

        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~Handle();
    };

private:

    // --------------------------------------------------------------------------------------------
    typedef std::vector< Handle * > Handles; // List of handles that receive callbacks.

    // --------------------------------------------------------------------------------------------
    static Handles s_Handles;

    /* --------------------------------------------------------------------------------------------
     * See whether the specified state instance is in the handles list.
    */
    static bool IsListed(Handle * hnd);

    /* --------------------------------------------------------------------------------------------
     * Add the specified state instance to the handles list to receive callbacks.
    */
    static void Remember(Handle * hnd);

    /* --------------------------------------------------------------------------------------------
     * Remove the specified state instance from the handles list.
    */
    static void Forget(Handle * hnd);

    /* --------------------------------------------------------------------------------------------
     * Make the specified handle to be the first in the list.
    */
    static void ToHead(Handle * hnd);

protected:

    /* --------------------------------------------------------------------------------------------
     * The error handler assigned to every created state.
    */
    static void ErrorHandler(void * opaque, const char * msg);

    /* --------------------------------------------------------------------------------------------
     * Add a symbol to the compiled program.
    */
    void PushSymbol(CSStr name, const void * value);

    /* --------------------------------------------------------------------------------------------
     * Return symbol value or NULL if not found.
    */
    void * PullSymbol(CSStr name) const;

private:

    // --------------------------------------------------------------------------------------------
    Handle* m_Hnd;

    /* --------------------------------------------------------------------------------------------
     * Grab a strong reference to a state handle.
    */
    void Grab()
    {
        if (m_Hnd)
        {
            ++(m_Hnd->mRef);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Drop a strong reference to a state handle.
    */
    void Drop()
    {
        if (m_Hnd && --(m_Hnd->mRef) == 0)
        {
            delete m_Hnd; // Let the destructor take care of cleaning up (if necessary)
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    StateHnd(CSStr name)
        : m_Hnd(name ? new Handle(1, name) : nullptr)
    {
        RegisterAPI(*this);
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor (null).
    */
    StateHnd()
        : m_Hnd(nullptr)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    StateHnd(const StateHnd & o)
        : m_Hnd(o.m_Hnd)
    {
        Grab();
    }

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    StateHnd(StateHnd && o)
        : m_Hnd(o.m_Hnd)
    {
        o.m_Hnd = nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~StateHnd()
    {
        Drop();
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    StateHnd & operator = (const StateHnd & o)
    {
        if (m_Hnd != o.m_Hnd)
        {
            Drop();
            m_Hnd = o.m_Hnd;
            Grab();
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    StateHnd & operator = (StateHnd && o)
    {
        if (m_Hnd != o.m_Hnd)
        {
            m_Hnd = o.m_Hnd;
            o.m_Hnd = nullptr;
        }

        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two state handles.
    */
    bool operator == (const StateHnd & o) const
    {
        return (m_Hnd == o.m_Hnd);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two state handles.
    */
    bool operator != (const StateHnd & o) const
    {
        return (m_Hnd != o.m_Hnd);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean for use in boolean operations.
    */
    operator bool () const
    {
        return m_Hnd && m_Hnd->mPtr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance.
    */
    operator Pointer ()
    {
        return m_Hnd ? m_Hnd->mPtr : nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance.
    */
    operator Pointer () const
    {
        return m_Hnd ? m_Hnd->mPtr : nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance.
    */
    operator Reference ()
    {
        assert(m_Hnd && m_Hnd->mPtr);
        return *(m_Hnd->mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance.
    */
    operator ConstRef () const
    {
        assert(m_Hnd && m_Hnd->mPtr);
        return *(m_Hnd->mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Member operator for dereferencing the managed pointer.
    */
    Handle * operator -> () const
    {
        assert(m_Hnd);
        return m_Hnd;
    }

    /* --------------------------------------------------------------------------------------------
     * Indirection operator for obtaining a reference of the managed pointer.
    */
    Handle & operator * () const
    {
        assert(m_Hnd);
        return *m_Hnd;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw handle structure pointer.
    */
    Handle * HndPtr()
    {
        return m_Hnd;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw handle structure pointer.
    */
    Handle * HndPtr() const
    {
        return m_Hnd;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of active references to the managed instance.
    */
    Counter Count() const
    {
        return m_Hnd ? m_Hnd->mRef : 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Set options as from command line (multiple supported).
    */
    void SetOptions(CSStr opt) const;

    /* --------------------------------------------------------------------------------------------
     * Define preprocessor symbol 'sym'. Can put optional value.
    */
    void DefineSymbol(CSStr sym, CSStr value) const;

    /* --------------------------------------------------------------------------------------------
     * Undefine preprocess symbol 'sym'.
    */
    void UndefineSymbol(CSStr sym) const;

    /* --------------------------------------------------------------------------------------------
     * Add include path.
    */
    void AddIncludePath(CSStr path) const;

    /* --------------------------------------------------------------------------------------------
     * Add in system include path.
    */
    void AddSysIncludePath(CSStr path) const;

    /* --------------------------------------------------------------------------------------------
     * Set CONFIG_TCCDIR at runtime.
    */
    void SetLibPath(CSStr path) const;

    /* --------------------------------------------------------------------------------------------
     * Equivalent to -Lpath option.
    */
    void AddLibraryPath(CSStr path) const;

    /* --------------------------------------------------------------------------------------------
     * The library name is the same as the argument of the '-l' option.
    */
    void AddLibrary(CSStr name) const;

    /* --------------------------------------------------------------------------------------------
     * Compile a string containing a C source.
    */
    void CompileString(CSStr str) const;

    /* --------------------------------------------------------------------------------------------
     * Add a file (C file, dll, object, library, ld script).
    */
    void AddFile(CSStr filename, Int32 type) const;

    /* --------------------------------------------------------------------------------------------
     * Do all relocations (needed before using tcc_get_symbol())
    */
    void Relocate() const;

    /* --------------------------------------------------------------------------------------------
     * Add a symbol to the compiled program.
    */
    template < typename T > void AddSymbol(CSStr name, T value)
    {
        PushSymbol(name, reinterpret_cast< const void * >(value));
    }

    /* --------------------------------------------------------------------------------------------
     * Return symbol value or NULL if not found.
    */
    template < typename T > T GetSymbol(CSStr name) const
    {
        return reinterpret_cast< T >(PullSymbol(name));
    }

    /* --------------------------------------------------------------------------------------------
     * Server callbacks.
    */
    static int OnInitServer(void);
    static void OnShutdownServer(void);
    static void OnFrame(float delta);
    static void OnPlayerConnect(int player);
    static void OnPlayerDisconnect(int player, int reason);
    static void OnPlayerBeginTyping(int player);
    static void OnPlayerEndTyping(int player);
    static int OnPlayerRequestClass(int player, int offset);
    static int OnPlayerRequestSpawn(int player);
    static void OnPlayerSpawn(int player);
    static void OnPlayerDeath(int player, int killer, int reason, int body_part);
    static void OnPlayerUpdate(int player, int type);
    static int OnPlayerRequestEnter(int player, int vehicle, int slot);
    static void OnPlayerEnterVehicle(int player, int vehicle, int slot);
    static void OnPlayerExitVehicle(int player, int vehicle);
    static void OnPlayerNameChange(int player, const char * previous, const char * current);
    static void OnPlayerStateChange(int player, int previous, int current);
    static void OnPlayerActionChange(int player, int previous, int current);
    static void OnPlayerOnFireChange(int player, unsigned int state);
    static void OnPlayerCrouchChange(int player, unsigned int state);
    static void OnPlayerGameKeysChange(int player, int previous, int current);
    static int OnPickupClaimPicked(int pickup, int player);
    static void OnPickupPickedUp(int pickup, int player);
    static void OnPickupRespawn(int pickup);
    static void OnVehicleUpdate(int vehicle, int type);
    static void OnVehicleExplode(int vehicle);
    static void OnVehicleRespawn(int vehicle);
    static void OnObjectShot(int object, int player, int weapon);
    static void OnObjectBump(int object, int player);
    static int OnPublicMessage(int player, const char * message);
    static int OnCommandMessage(int player, const char * command);
    static int OnPrivateMessage(int player, int target, const char * message);
    static int OnInternalCommand(unsigned int type, const char * text);
    static int OnLoginAttempt(char * name, const char * passwd, const char * ipaddr);
    static void OnEntityPoolChange(int type, int entity, unsigned int deleted);
    static void OnKeyBindDown(int player, int keybind);
    static void OnKeyBindUp(int player, int keybind);
    static void OnPlayerAwayChange(int player, unsigned int status);
    static void OnPlayerSpectate(int player, int target);
    static void OnPlayerCrashReport(int player, const char * report);
    static void OnServerPerformanceReport(int count, const char ** description, unsigned long long * millispent);
    static void OnCheckpointEntered(int checkpoint, int player);
    static void OnCheckpointExited(int checkpoint, int player);
    static void OnSphereEntered(int sphere, int player);
    static void OnSphereExited(int sphere, int player);

    /* --------------------------------------------------------------------------------------------
     * Enable or disable a certain server event.
    */
    static void ToggleEvent(Int32 evt, bool toggle);
};

} // Namespace:: SqMod

#endif // _SQTCC_COMMON_HPP_
