// ------------------------------------------------------------------------------------------------
#include "Core.hpp"
#include "Logger.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqstdstring.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
static Object & GetBlip(Int32 id) { return _Core->GetBlip(id).mObj; }
static Object & GetCheckpoint(Int32 id) { return _Core->GetCheckpoint(id).mObj; }
static Object & GetForcefield(Int32 id) { return _Core->GetForcefield(id).mObj; }
static Object & GetKeybind(Int32 id) { return _Core->GetKeybind(id).mObj; }
static Object & GetObject(Int32 id) { return _Core->GetObject(id).mObj; }
static Object & GetPickup(Int32 id) { return _Core->GetPickup(id).mObj; }
static Object & GetPlayer(Int32 id) { return _Core->GetPlayer(id).mObj; }
static Object & GetSprite(Int32 id) { return _Core->GetSprite(id).mObj; }
static Object & GetTextdraw(Int32 id) { return _Core->GetTextdraw(id).mObj; }
static Object & GetVehicle(Int32 id) { return _Core->GetVehicle(id).mObj; }

// ------------------------------------------------------------------------------------------------
static bool DelBlip(Int32 id, Int32 header, Object & payload)
{
    return _Core->DelBlip(id, header, payload);
}

static bool DelCheckpoint(Int32 id, Int32 header, Object & payload)
{
    return _Core->DelCheckpoint(id, header, payload);
}

static bool DelForcefield(Int32 id, Int32 header, Object & payload)
{
    return _Core->DelForcefield(id, header, payload);
}

static bool DelKeybind(Int32 id, Int32 header, Object & payload)
{
    return _Core->DelKeybind(id, header, payload);
}

static bool DelObject(Int32 id, Int32 header, Object & payload)
{
    return _Core->DelObject(id, header, payload);
}

static bool DelPickup(Int32 id, Int32 header, Object & payload)
{
    return _Core->DelPickup(id, header, payload);
}

static bool DelSprite(Int32 id, Int32 header, Object & payload)
{
    return _Core->DelSprite(id, header, payload);
}

static bool DelTextdraw(Int32 id, Int32 header, Object & payload)
{
    return _Core->DelTextdraw(id, header, payload);
}

static bool DelVehicle(Int32 id, Int32 header, Object & payload)
{
    return _Core->DelVehicle(id, header, payload);
}

// ------------------------------------------------------------------------------------------------
static void BindEvent(Int32 id, Object & env, Function & func)
{
    _Core->BindEvent(id, env, func);
}

// ------------------------------------------------------------------------------------------------
static Int32 GetState()
{
    return _Core->GetState();
}

static void SetState(Int32 value)
{
    return _Core->SetState(value);
}

// ------------------------------------------------------------------------------------------------
static CSStr GetOption(CSStr name)
{
    return _Core->GetOption(name);
}

static CSStr GetOptionOr(CSStr name, CSStr value)
{
    return _Core->GetOption(name, value);
}

static void SetOption(CSStr name, CSStr value)
{
    return _Core->SetOption(name, value);
}

// ================================================================================================
void Register_Core(HSQUIRRELVM vm)
{
    RootTable(vm)
    .Bind(_SC("SqCore"), Table(vm)
        .Func(_SC("Bind"), &BindEvent)
        .Func(_SC("GetState"), &GetState)
        .Func(_SC("SetState"), &SetState)
        .Func(_SC("GetOption"), &GetOption)
        .Func(_SC("GetOptionOr"), &GetOptionOr)
        .Func(_SC("SetOption"), &SetOption)
        .Func(_SC("GetBlip"), &GetBlip)
        .Func(_SC("GetCheckpoint"), &GetCheckpoint)
        .Func(_SC("GetForcefield"), &GetForcefield)
        .Func(_SC("GetKeybind"), &GetKeybind)
        .Func(_SC("GetObject"), &GetObject)
        .Func(_SC("GetPickup"), &GetPickup)
        .Func(_SC("GetPlayer"), &GetPlayer)
        .Func(_SC("GetSprite"), &GetSprite)
        .Func(_SC("GetTextdraw"), &GetTextdraw)
        .Func(_SC("GetVehicle"), &GetVehicle)
        .Func(_SC("DestroyBlip"), &DelBlip)
        .Func(_SC("DestroyCheckpoint"), &DelCheckpoint)
        .Func(_SC("DestroyForcefield"), &DelForcefield)
        .Func(_SC("DestroyKeybind"), &DelKeybind)
        .Func(_SC("DestroyObject"), &DelObject)
        .Func(_SC("DestroyPickup"), &DelPickup)
        .Func(_SC("DestroySprite"), &DelSprite)
        .Func(_SC("DestroyTextdraw"), &DelTextdraw)
        .Func(_SC("DestroyVehicle"), &DelVehicle)
    );
}

// ------------------------------------------------------------------------------------------------
template < Uint8 L, bool S > static SQInteger LogBasicMessage(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the message value specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing message value");
    }
    // Do we have enough values to call the format function?
    else if (top > 2)
    {
        SStr msg = NULL;
        SQInteger len = 0;
        // Attempt to generate the specified string format
        SQRESULT ret = sqstd_format(vm, 2, &len, &msg);
        // Did the format failed?
        if (SQ_FAILED(ret))
        {
            return ret; // Propagate the exception
        }
        // Log the resulted string value
        _Log->Message(L, S, "%s", msg);
    }
    else
    {
        // Attempt to retrieve the value from the stack as a string
        Var< CSStr > msg(vm, 2);
        // See if the obtained value is a valid string
        if (!msg.value)
        {
            return sq_throwerror(vm, "Unable to retrieve the value");
        }
        // Log the resulted string value
        _Log->Message(L, S, "%s", msg.value);
    }
    // This function does not return a value
    return 1;
}

// ================================================================================================
void Register_Log(HSQUIRRELVM vm)
{
    RootTable(vm)
    .Bind(_SC("SqLog"), Table(vm)
        .SquirrelFunc(_SC("Dbg"), &LogBasicMessage< LL_DBG, false >)
        .SquirrelFunc(_SC("Usr"), &LogBasicMessage< LL_USR, false >)
        .SquirrelFunc(_SC("Scs"), &LogBasicMessage< LL_SCS, false >)
        .SquirrelFunc(_SC("Inf"), &LogBasicMessage< LL_INF, false >)
        .SquirrelFunc(_SC("Wrn"), &LogBasicMessage< LL_WRN, false >)
        .SquirrelFunc(_SC("Err"), &LogBasicMessage< LL_ERR, false >)
        .SquirrelFunc(_SC("Ftl"), &LogBasicMessage< LL_FTL, false >)
        .SquirrelFunc(_SC("SDbg"), &LogBasicMessage< LL_DBG, true >)
        .SquirrelFunc(_SC("SUsr"), &LogBasicMessage< LL_USR, true >)
        .SquirrelFunc(_SC("SScs"), &LogBasicMessage< LL_SCS, true >)
        .SquirrelFunc(_SC("SInf"), &LogBasicMessage< LL_INF, true >)
        .SquirrelFunc(_SC("SWrn"), &LogBasicMessage< LL_WRN, true >)
        .SquirrelFunc(_SC("SErr"), &LogBasicMessage< LL_ERR, true >)
        .SquirrelFunc(_SC("SFtl"), &LogBasicMessage< LL_FTL, true >)
    );
}

} // Namespace:: SqMod
