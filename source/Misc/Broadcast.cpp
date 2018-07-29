// ------------------------------------------------------------------------------------------------
#include "Core.hpp"
#include "Base/Shared.hpp"
#include "Base/Color3.hpp"
#include "Base/Color4.hpp"
#include "Entity/Player.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
static inline bool SqCanBeInteger(HSQUIRRELVM vm, Int32 idx)
{
    switch (sq_gettype(vm, idx))
    {
        case OT_NULL:
        case OT_INTEGER:
        case OT_FLOAT:
        case OT_BOOL:
            return true;
        default:
            return false;
    }
}

// ------------------------------------------------------------------------------------------------
SQRESULT SqGrabPlayerMessageColor(HSQUIRRELVM vm, Int32 idx, Uint32 & color, Int32 & msgidx)
{
    const Int32 top = sq_gettop(vm);
    // Is the color argument a Color3/Color4 instance?
    if (sq_gettype(vm, idx) == OT_INSTANCE)
    {
        // Whether it failed to retrieve a Color3 value
        bool failed = false;
        // Attempt to extract a Color3 value
        try
        {
            color = (Var< Color3 >(vm, idx).value.GetRGBA() | 0xFF);
        }
        catch (...)
        {
            failed = true;
        }
        // Did we failed to retrieve a Color3 instance?
        if (failed)
        {
            // Attempt to extract a Color4 value
            try
            {
                color = Var< Color4 >(vm, idx).value.GetRGBA();
            }
            catch (const Sqrat::Exception & e)
            {
                return sq_throwerror(vm, e.what());
            }
        }
        // The message starts right after the color
        msgidx += 1;
    }
    // Is the color argument an unpacked RGBA color?
    else if ((top - msgidx) >= 4 && SqCanBeInteger(vm, idx)
                        && SqCanBeInteger(vm, idx+1)
                        && SqCanBeInteger(vm, idx+2)
                        && SqCanBeInteger(vm, idx+3))
    {
        color = SQMOD_PACK_RGBA(ConvTo< Uint8 >::From(PopStackInteger(vm, idx)),
                                ConvTo< Uint8 >::From(PopStackInteger(vm, idx+1)),
                                ConvTo< Uint8 >::From(PopStackInteger(vm, idx+2)),
                                ConvTo< Uint8 >::From(PopStackInteger(vm, idx+3)));
        // The message starts right after the color
        msgidx += 4;
    }
    // Is the color argument an unpacked RGB color?
    else if ((top - msgidx) >= 3 && SqCanBeInteger(vm, idx)
                        && SqCanBeInteger(vm, idx+1)
                        && SqCanBeInteger(vm, idx+2))
    {
        color = SQMOD_PACK_RGBA(ConvTo< Uint8 >::From(PopStackInteger(vm, idx)),
                                ConvTo< Uint8 >::From(PopStackInteger(vm, idx+1)),
                                ConvTo< Uint8 >::From(PopStackInteger(vm, idx+2)),
                                0xFF);
        // The message starts right after the color
        msgidx += 3;
    }
    // Is the color argument an packed RGBA color?
    else if (SqCanBeInteger(vm, idx))
    {
        color = static_cast< Uint32 >(PopStackInteger(vm, idx));
        // The message starts right after the color
        msgidx += 1;
    }
    // Is the first argument a string which can be interpreted as a color?
    else if (sq_gettype(vm, idx) == OT_STRING)
    {
        CSStr str = nullptr;
        SQInteger len = 0;
        // Attempt to retrieve the color argument as a string
        if (SQ_FAILED(sq_getstringandsize(vm, idx, &str, &len)))
        {
            return sq_throwerror(vm, "Cannot retrieve the color string");
        }
        // Do we even have any color?
        else if (!str || *str == '\0')
        {
            return sq_throwerror(vm, "Invalid or empty color string");
        }
        // Skip white space at the beginning
        while (std::isspace(*str) != 0)
        {
            ++str, --len;
        }
        // Is the first character a number sign?
        if (*str == '#')
        {
            // Attempt to treat the value as a hex color
            color = ConvTo< Uint32 >::From(std::strtoull(++str, nullptr, 16));
            // Adjust the color if necessary
            switch (ClampMin(--len, static_cast< SQInteger >(0)))
            {
                case 0:
                {
                    color = 0x000000FF;
                } break;
                case 1:
                {
                    color <<= 28;
                    color |= 0x00000FF;
                } break;
                case 2:
                {
                    color <<= 24;
                    color |= 0x0000FF;
                } break;
                case 3:
                {
                    color <<= 20;
                    color |= 0x000FF;
                } break;
                case 4:
                {
                    color <<= 16;
                    color |= 0x00FF;
                } break;
                case 5:
                {
                    color <<= 12;
                    color |= 0x0FF;
                } break;
                case 6:
                {
                    color <<= 8;
                    color |= 0xFF;
                } break;
                case 7:
                {
                    color <<= 4;
                    color |= 0x0;
                } break;
            }
        }
        // Are the first characters 0x so we can treat this as hex?
        else if (*str == '0' && (*(str+1) == 'x' || *(str+1) == 'X'))
        {
            // Attempt to treat the value as a hex color
            color = ConvTo< Uint32 >::From(std::strtoull(str, nullptr, 16));
            // Adjust the color if necessary
            switch (ClampMin(len-2, static_cast< SQInteger >(0)))
            {
                case 0:
                {
                    color = 0x000000FF;
                } break;
                case 1:
                {
                    color <<= 28;
                    color |= 0x00000FF;
                } break;
                case 2:
                {
                    color <<= 24;
                    color |= 0x0000FF;
                } break;
                case 3:
                {
                    color <<= 20;
                    color |= 0x000FF;
                } break;
                case 4:
                {
                    color <<= 16;
                    color |= 0x00FF;
                } break;
                case 5:
                {
                    color <<= 12;
                    color |= 0x0FF;
                } break;
                case 6:
                {
                    color <<= 8;
                    color |= 0xFF;
                } break;
                case 7:
                {
                    color <<= 4;
                    color |= 0x0;
                } break;
            }
        }
        else
        {
            // Attempt to treat the value as a color name
            try
            {
                color = (::SqMod::GetColorStr(str).GetRGBA() | 0xFF);
            }
            catch (...)
            {
                return sq_throwerror(vm, "Cannot identify color string");
            }
        }
        // The message starts right after the color
        msgidx += 1;
    }
    else
    {
        return sq_throwerror(vm, "Unable to identify a valid color");
    }
    // At this point we've extracted the color
    return SQ_OK;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqBroadcastMsg(HSQUIRRELVM vm)
{
    // The function needs at least 2 arguments
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

    // The index where the message should start
    Int32 msgidx = 2;
    // The message color
    Uint32 color = 0;
    // Attempt to identify and extract the color
    const SQRESULT res = SqGrabPlayerMessageColor(vm, 2, color, msgidx);
    // Did we fail to identify a color?
    if (SQ_FAILED(res))
    {
        return res; // Propagate the error!
    }

    // Attempt to generate the string value
    StackStrF val(vm, msgidx);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }

    // Obtain the ends of the entity pool
    Core::Players::const_iterator itr = Core::Get().GetPlayers().cbegin();
    Core::Players::const_iterator end = Core::Get().GetPlayers().cend();
    // The number of players that the message was sent to
    Uint32 count = 0;
    // Currently processed player
    CPlayer * player = nullptr;

    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Grab the player instance
        player = itr->mInst;
        // Is this player instance valid?
        if (VALID_ENTITYEX(itr->mID, SQMOD_PLAYER_POOL) && player != nullptr)
        {
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
    Uint32 index = 0;
    // Attempt to extract the argument values
    try
    {
        index = ConvTo< Uint32 >::From(Var< SQInteger >(vm, 2).value);
    }
    catch (const Sqrat::Exception & e)
    {
        return sq_throwerror(vm, e.what());
    }

    // Perform a range check on the specified prefix index
    if (index >= SQMOD_PLAYER_MSG_PREFIXES)
    {
        return sq_throwerror(vm, ToStrF("Prefix index is out of range: %u >= %u",
                                        index, SQMOD_PLAYER_MSG_PREFIXES));
    }

    // Attempt to generate the string value
    StackStrF val(vm, 3);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }

    vcmpError result = vcmpErrorNone;
    // Obtain the ends of the entity pool
    Core::Players::const_iterator itr = Core::Get().GetPlayers().cbegin();
    Core::Players::const_iterator end = Core::Get().GetPlayers().cend();
    // The number of players that the message was sent to
    Uint32 count = 0;
    // Currently processed player
    CPlayer * player = nullptr;

    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Grab the player instance
        player = itr->mInst;
        // Is this player instance valid?
        if (VALID_ENTITYEX(itr->mID, SQMOD_PLAYER_POOL) && player != nullptr)
        {
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
    // Was the index of the message prefix specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing prefix index");
    }
    // Was the message color specified?
    else if (top <= 2)
    {
        return sq_throwerror(vm, "Missing message color");
    }
    // Was the message value specified?
    else if (top <= 3)
    {
        return sq_throwerror(vm, "Missing message value");
    }

    // The prefix index
    Uint32 index = 0;
    // Attempt to extract the argument values
    try
    {
        index = ConvTo< Uint32 >::From(Var< SQInteger >(vm, 2).value);
    }
    catch (const Sqrat::Exception & e)
    {
        return sq_throwerror(vm, e.what());
    }

    // Perform a range check on the specified prefix index
    if (index >= SQMOD_PLAYER_MSG_PREFIXES)
    {
        return sq_throwerror(vm, ToStrF("Prefix index is out of range: %u >= %u",
                                        index, SQMOD_PLAYER_MSG_PREFIXES));
    }

    // The index where the message should start
    Int32 msgidx = 3;
    // The message color
    Uint32 color = 0;
    // Attempt to identify and extract the color
    const SQRESULT res = SqGrabPlayerMessageColor(vm, 3, color, msgidx);
    // Did we fail to identify a color?
    if (SQ_FAILED(res))
    {
        return res; // Propagate the error!
    }

    // Attempt to generate the string value
    StackStrF val(vm, msgidx);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }

    vcmpError result = vcmpErrorNone;
    // Obtain the ends of the entity pool
    Core::Players::const_iterator itr = Core::Get().GetPlayers().cbegin();
    Core::Players::const_iterator end = Core::Get().GetPlayers().cend();
    // The number of players that the message was sent to
    Uint32 count = 0;
    // Currently processed player
    CPlayer * player = nullptr;

    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Grab the player instance
        player = itr->mInst;
        // Is this player instance valid?
        if (VALID_ENTITYEX(itr->mID, SQMOD_PLAYER_POOL) && player != nullptr)
        {
            // Send the resulted message string
            if (player->mLimitPrefixPostfixMessage)
            {
                result = _Func->SendClientMessage(itr->mID, color, "%s%s",
                                                    player->mMessagePrefixes[index].c_str(), val.mPtr);
            }
            else
            {
                result = _Func->SendClientMessage(itr->mID, color, "%s%s%s%s",
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
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }

    // Obtain the ends of the entity pool
    Core::Players::const_iterator itr = Core::Get().GetPlayers().cbegin();
    Core::Players::const_iterator end = Core::Get().GetPlayers().cend();
    // The number of players that the message was sent to
    Uint32 count = 0;
    // Currently processed player
    CPlayer * player = nullptr;

    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Grab the player instance
        player = itr->mInst;
        // Is this player instance valid?
        if (VALID_ENTITYEX(itr->mID, SQMOD_PLAYER_POOL) && player != nullptr)
        {
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
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }

    // Obtain the ends of the entity pool
    Core::Players::const_iterator itr = Core::Get().GetPlayers().cbegin();
    Core::Players::const_iterator end = Core::Get().GetPlayers().cend();
    // The number of players that the message was sent to
    Uint32 count = 0;
    // Currently processed player
    CPlayer * player = nullptr;

    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Grab the player instance
        player = itr->mInst;
        // Is this player instance valid?
        if (VALID_ENTITYEX(itr->mID, SQMOD_PLAYER_POOL) && player != nullptr)
        {
            // Send the resulted announcement string
            const vcmpError result = _Func->SendGameMessage(itr->mID, player->mAnnounceStyle,
                                                            "%s%s%s",
                                                            player->mAnnouncePrefix.c_str(),
                                                            val.mPtr,
                                                            player->mAnnouncePostfix.c_str());
            // Validate the result
            if (result == vcmpErrorArgumentOutOfBounds)
            {
                return sq_throwerror(vm, ToStrF("Invalid announcement style %d [%s]",
                                                player->mAnnounceStyle, player->GetTag().c_str()));
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

    Int32 style;
    // style to extract the argument values
    try
    {
        style = Var< Int32 >(vm, 2).value;
    }
    catch (const Sqrat::Exception & e)
    {
        return sq_throwerror(vm, e.what());
    }

    // Attempt to generate the string value
    StackStrF val(vm, 3);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }

    // Obtain the ends of the entity pool
    Core::Players::const_iterator itr = Core::Get().GetPlayers().cbegin();
    Core::Players::const_iterator end = Core::Get().GetPlayers().cend();
    // The number of players that the message was sent to
    Uint32 count = 0;
    // Currently processed player
    CPlayer * player = nullptr;

    // Process each entity in the pool
    for (; itr != end; ++itr)
    {
        // Grab the player instance
        player = itr->mInst;
        // Is this player instance valid?
        if (VALID_ENTITYEX(itr->mID, SQMOD_PLAYER_POOL) && player != nullptr)
        {
            // Send the resulted announcement string
            const vcmpError result = _Func->SendGameMessage(itr->mID, style,
                                                            "%s%s%s",
                                                            player->mAnnouncePrefix.c_str(),
                                                            val.mPtr,
                                                            player->mAnnouncePostfix.c_str());
            // Validate the result
            if (result == vcmpErrorArgumentOutOfBounds)
            {
                return sq_throwerror(vm, ToStrF("Invalid announcement style %d [%s]",
                                                style, player->GetTag().c_str()));
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
