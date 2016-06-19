// ------------------------------------------------------------------------------------------------
#include "Core.hpp"
#include "Base/Shared.hpp"
#include "Base/Color3.hpp"
#include "Base/Color4.hpp"
#include "Entity/Player.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
static SQInteger SqBroadcastMsg(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the message color specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing message color");
    }
    // Was the message value specified?
    else if (top <= 2)
    {
        return sq_throwerror(vm, "Missing message value");
    }
    // Failed to retrieve a Color4 value
    bool failed = false;
    // The message color
    Color4 color;
    // Attempt to extract the argument values
    try
    {
        color = Var< Color4 >(vm, 2).value;
    }
    catch (...)
    {
        failed = true;
    }
    // Did we failed to retrieve a Color4 instance?
    if (failed)
    {
        // Attempt to extract the argument values
        try
        {
            color = Var< Color3 >(vm, 2).value;
        }
        catch (const Sqrat::Exception & e)
        {
            // Propagate the error
            return sq_throwerror(vm, e.Message().c_str());
        }
    }
    // Attempt to generate the string value
    StackStrF val(vm, 3);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // Obtain the ends of the entity pool
    Core::Players::const_iterator itr = Core::Get().GetPlayers().cbegin();
    Core::Players::const_iterator end = Core::Get().GetPlayers().cend();
    // The number of players that the message was sent to
    Uint32 count = 0;
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Is this player instance valid?
        if (VALID_ENTITYEX(itr->mID, SQMOD_PLAYER_POOL) && itr->mInst != nullptr)
        {
            // Grab the player instance
            CPlayer * player = itr->mInst;
            // Send the resulted message string
            const vcmpError result = _Func->SendClientMessage(itr->mID, color.GetRGBA(),
                                                        "%s%s%s",
                                                        player->mMessagePrefix.c_str(),
                                                        val.mPtr,
                                                        player->mMessagePostfix.c_str());
            // Check the result
            if (result == vcmpErrorTooLargeInput)
            {
                return sq_throwerror(vm, ToStrF("Client message too big [%s]", player->GetTag().c_str()));
            }
            // Add this player to the count
            ++count;
        }
    }
    // Push the count count on the stack
    sq_pushinteger(vm, count);
    // Specify that this function returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqBroadcastMsgP(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the index of the message prefix specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing prefix index");
    }
    // Was the message value specified?
    else if (top <= 2)
    {
        return sq_throwerror(vm, "Missing message value");
    }
    // The prefix index
    Uint32 index;
    // Attempt to extract the argument values
    try
    {
        index = Var< Uint32 >(vm, 2).value;
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.Message().c_str());
    }
    // Perform a range check on the specified prefix index
    if (index > SQMOD_PLAYER_MSG_PREFIXES)
    {
        return sq_throwerror(vm, ToStrF("Prefix index is out of range: %u > %u",
                                        index, SQMOD_PLAYER_MSG_PREFIXES));
    }
    // Attempt to generate the string value
    StackStrF val(vm, 3);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    vcmpError result = vcmpErrorNone;
    // Send the resulted message string
    // Obtain the ends of the entity pool
    Core::Players::const_iterator itr = Core::Get().GetPlayers().cbegin();
    Core::Players::const_iterator end = Core::Get().GetPlayers().cend();
    // The number of players that the message was sent to
    Uint32 count = 0;
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Is this player instance valid?
        if (VALID_ENTITYEX(itr->mID, SQMOD_PLAYER_POOL) && itr->mInst != nullptr)
        {
            // Grab the player instance
            CPlayer * player = itr->mInst;
            // Send the resulted message string
            if (player->mLimitPrefixPostfixMessage)
            {
                result = _Func->SendClientMessage(itr->mID, player->mMessageColor, "%s%s",
                                                    player->mMessagePrefixes[index].c_str(), val.mPtr);
            }
            else
            {
                result = _Func->SendClientMessage(itr->mID, player->mMessageColor, "%s%s%s%s",
                                                    player->mMessagePrefix.c_str(),
                                                    player->mMessagePrefixes[index].c_str(), val.mPtr,
                                                    player->mMessagePostfix.c_str());
            }
            // Check the result
            if (result == vcmpErrorTooLargeInput)
            {
                return sq_throwerror(vm, ToStrF("Client message too big [%s]", player->GetTag().c_str()));
            }
            // Add this player to the count
            ++count;
        }
    }
    // Push the count count on the stack
    sq_pushinteger(vm, count);
    // Specify that this function returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqBroadcastMsgEx(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the message color specified?
    if (top <= 4)
    {
        return sq_throwerror(vm, "Missing message color");
    }
    // Was the message value specified?
    else if (top <= 5)
    {
        return sq_throwerror(vm, "Missing message value");
    }
    // The message color
    Uint32 color;
    // Attempt to extract the argument values
    try
    {
        color = SQMOD_PACK_RGBA(Var< Uint8 >(vm, 2).value,
                                Var< Uint8 >(vm, 3).value,
                                Var< Uint8 >(vm, 4).value,
                                Var< Uint8 >(vm, 5).value);
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.Message().c_str());
    }
    // Attempt to generate the string value
    StackStrF val(vm, 6);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // Obtain the ends of the entity pool
    Core::Players::const_iterator itr = Core::Get().GetPlayers().cbegin();
    Core::Players::const_iterator end = Core::Get().GetPlayers().cend();
    // The number of players that the message was sent to
    Uint32 count = 0;
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Is this player instance valid?
        if (VALID_ENTITYEX(itr->mID, SQMOD_PLAYER_POOL) && itr->mInst != nullptr)
        {
            // Grab the player instance
            CPlayer * player = itr->mInst;
            // Send the resulted message string
            const vcmpError result = _Func->SendClientMessage(itr->mID, color,
                                                                "%s%s%s",
                                                                player->mMessagePrefix.c_str(),
                                                                val.mPtr,
                                                                player->mMessagePostfix.c_str());
            // Check the result
            if (result == vcmpErrorTooLargeInput)
            {
                return sq_throwerror(vm, ToStrF("Client message too big [%s]", player->GetTag().c_str()));
            }
            // Add this player to the count
            ++count;
        }
    }
    // Push the count count on the stack
    sq_pushinteger(vm, count);
    // Specify that this function returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqBroadcastMessage(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the message value specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing message value");
    }
    // Attempt to generate the string value
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // Obtain the ends of the entity pool
    Core::Players::const_iterator itr = Core::Get().GetPlayers().cbegin();
    Core::Players::const_iterator end = Core::Get().GetPlayers().cend();
    // The number of players that the message was sent to
    Uint32 count = 0;
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Is this player instance valid?
        if (VALID_ENTITYEX(itr->mID, SQMOD_PLAYER_POOL) && itr->mInst != nullptr)
        {
            // Grab the player instance
            CPlayer * player = itr->mInst;
            // Send the resulted message string
            const vcmpError result = _Func->SendClientMessage(itr->mID, player->mMessageColor,
                                                                "%s%s%s",
                                                                player->mMessagePrefix.c_str(),
                                                                val.mPtr,
                                                                player->mMessagePostfix.c_str());
            // Check the result
            if (result == vcmpErrorTooLargeInput)
            {
                return sq_throwerror(vm, ToStrF("Client message too big [%s]", player->GetTag().c_str()));
            }
            // Add this player to the count
            ++count;
        }
    }
    // Push the count count on the stack
    sq_pushinteger(vm, count);
    // Specify that this function returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqBroadcastAnnounce(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the announcement value specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing announcement value");
    }
    // Attempt to generate the string value
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // Obtain the ends of the entity pool
    Core::Players::const_iterator itr = Core::Get().GetPlayers().cbegin();
    Core::Players::const_iterator end = Core::Get().GetPlayers().cend();
    // The number of players that the message was sent to
    Uint32 count = 0;
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Is this player instance valid?
        if (VALID_ENTITYEX(itr->mID, SQMOD_PLAYER_POOL) && itr->mInst != nullptr)
        {
            // Grab the player instance
            CPlayer * player = itr->mInst;
            // Send the resulted announcement string
            const vcmpError result = _Func->SendGameMessage(itr->mID, player->mAnnounceStyle,
                                                            "%s%s%s",
                                                            player->mAnnouncePrefix.c_str(),
                                                            val.mPtr,
                                                            player->mAnnouncePostfix.c_str());
            // Validate the result
            if (result == vcmpErrorArgumentOutOfBounds)
            {
                return sq_throwerror(vm, ToStrF("Invalid announcement style [%s]", player->GetTag().c_str()));
            }
            else if (result == vcmpErrorTooLargeInput)
            {
                return sq_throwerror(vm, ToStrF("Game message too big [%s]", player->GetTag().c_str()));
            }
            // Add this player to the count
            ++count;
        }
    }
    // Push the count count on the stack
    sq_pushinteger(vm, count);
    // Specify that this function returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqBroadcastAnnounceEx(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the announcement style specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing announcement style");
    }
    // Was the announcement value specified?
    else if (top <= 2)
    {
        return sq_throwerror(vm, "Missing announcement value");
    }
    // The announcement style
    Int32 style;
    // style to extract the argument values
    try
    {
        style = Var< Int32 >(vm, 2).value;
    }
    catch (const Sqrat::Exception & e)
    {
        // Propagate the error
        return sq_throwerror(vm, e.Message().c_str());
    }
    // Attempt to generate the string value
    StackStrF val(vm, 3);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // Obtain the ends of the entity pool
    Core::Players::const_iterator itr = Core::Get().GetPlayers().cbegin();
    Core::Players::const_iterator end = Core::Get().GetPlayers().cend();
    // The number of players that the message was sent to
    Uint32 count = 0;
    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Is this player instance valid?
        if (VALID_ENTITYEX(itr->mID, SQMOD_PLAYER_POOL) && itr->mInst != nullptr)
        {
            // Grab the player instance
            CPlayer * player = itr->mInst;
            // Send the resulted announcement string
            const vcmpError result = _Func->SendGameMessage(itr->mID, style,
                                                            "%s%s%s",
                                                            player->mAnnouncePrefix.c_str(),
                                                            val.mPtr,
                                                            player->mAnnouncePostfix.c_str());
            // Validate the result
            if (result == vcmpErrorArgumentOutOfBounds)
            {
                return sq_throwerror(vm, ToStrF("Invalid announcement style [%s]", player->GetTag().c_str()));
            }
            else if (result == vcmpErrorTooLargeInput)
            {
                return sq_throwerror(vm, ToStrF("Game message too big [%s]", player->GetTag().c_str()));
            }
            // Add this player to the count
            ++count;
        }
    }
    // Push the count count on the stack
    sq_pushinteger(vm, count);
    // Specify that this function returned a value
    return 1;
}

// ================================================================================================
void Register_Broadcast(HSQUIRRELVM vm)
{
    Table bns(vm);

    bns
    .SquirrelFunc(_SC("Msg"), &SqBroadcastMsg)
    .SquirrelFunc(_SC("MsgP"), &SqBroadcastMsgP)
    .SquirrelFunc(_SC("MsgEx"), &SqBroadcastMsgEx)
    .SquirrelFunc(_SC("Message"), &SqBroadcastMessage)
    .SquirrelFunc(_SC("Announce"), &SqBroadcastAnnounce)
    .SquirrelFunc(_SC("AnnounceEx"), &SqBroadcastAnnounceEx)
    .SquirrelFunc(_SC("Text"), &SqBroadcastAnnounce)
    .SquirrelFunc(_SC("TextEx"), &SqBroadcastAnnounceEx);

    RootTable(vm).Bind(_SC("SqBroadcast"), bns);
}

} // Namespace:: SqMod
