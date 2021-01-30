// ------------------------------------------------------------------------------------------------
#include "Core.hpp"
#include "Base/Color3.hpp"
#include "Entity/Player.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
static inline bool SqCanBeInteger(HSQUIRRELVM vm, int32_t idx)
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
SQRESULT SqGrabPlayerMessageColor(HSQUIRRELVM vm, int32_t idx, uint32_t & color, int32_t & msg_idx)
{
    const auto top = static_cast< int32_t >(sq_gettop(vm));
    // Is the color argument a Color3/Color4 instance?
    if (sq_gettype(vm, idx) == OT_INSTANCE)
    {
        // Whether it failed to retrieve a Color3 value
        bool failed = false;
        // Attempt to extract a Color3 value
        try
        {
            color = (Var< Color3 >(vm, idx).value.GetRGBA() | 0xFFu);
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
            catch (const std::exception & e)
            {
                return sq_throwerror(vm, e.what());
            }
        }
        // The message starts right after the color
        msg_idx += 1;
    }
    // Is the color argument an unpacked RGBA color?
    else if ((top - msg_idx) >= 4 && SqCanBeInteger(vm, idx)
                        && SqCanBeInteger(vm, idx+1)
                        && SqCanBeInteger(vm, idx+2)
                        && SqCanBeInteger(vm, idx+3))
    {
        color = SQMOD_PACK_RGBA(ConvTo< uint8_t >::From(PopStackInteger(vm, idx)), // NOLINT(hicpp-signed-bitwise)
                                ConvTo< uint8_t >::From(PopStackInteger(vm, idx+1)),
                                ConvTo< uint8_t >::From(PopStackInteger(vm, idx+2)),
                                ConvTo< uint8_t >::From(PopStackInteger(vm, idx+3)));
        // The message starts right after the color
        msg_idx += 4;
    }
    // Is the color argument an unpacked RGB color?
    else if ((top - msg_idx) >= 3 && SqCanBeInteger(vm, idx)
                        && SqCanBeInteger(vm, idx+1)
                        && SqCanBeInteger(vm, idx+2))
    {
        color = SQMOD_PACK_RGBA(ConvTo< uint8_t >::From(PopStackInteger(vm, idx)), // NOLINT(hicpp-signed-bitwise)
                                ConvTo< uint8_t >::From(PopStackInteger(vm, idx+1)),
                                ConvTo< uint8_t >::From(PopStackInteger(vm, idx+2)),
                                0xFFu);
        // The message starts right after the color
        msg_idx += 3;
    }
    // Is the color argument an packed RGBA color?
    else if (SqCanBeInteger(vm, idx))
    {
        color = static_cast< uint32_t >(PopStackInteger(vm, idx));
        // The message starts right after the color
        msg_idx += 1;
    }
    // Is the first argument a string which can be interpreted as a color?
    else if (sq_gettype(vm, idx) == OT_STRING)
    {
        const SQChar * str = nullptr;
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
            color = ConvTo< uint32_t >::From(std::strtoull(++str, nullptr, 16));
            // Adjust the color if necessary
            switch (ClampMin(--len, static_cast< SQInteger >(0)))
            {
                case 0:
                {
                    color = 0x000000FFu;
                } break;
                case 1:
                {
                    color <<= 28u;
                    color |= 0x00000FFu;
                } break;
                case 2:
                {
                    color <<= 24u;
                    color |= 0x0000FFu;
                } break;
                case 3:
                {
                    color <<= 20u;
                    color |= 0x000FFu;
                } break;
                case 4:
                {
                    color <<= 16u;
                    color |= 0x00FFu;
                } break;
                case 5:
                {
                    color <<= 12u;
                    color |= 0x0FFu;
                } break;
                case 6:
                {
                    color <<= 8u;
                    color |= 0xFFu;
                } break;
                case 7:
                {
                    color <<= 4u;
                    color |= 0x0u;
                } break;
              default: break;
            }
        }
        // Are the first characters 0x so we can treat this as hex?
        else if (*str == '0' && (*(str+1) == 'x' || *(str+1) == 'X'))
        {
            // Attempt to treat the value as a hex color
            color = ConvTo< uint32_t >::From(std::strtoull(str, nullptr, 16));
            // Adjust the color if necessary
            switch (ClampMin(len-2, static_cast< SQInteger >(0)))
            {
                case 0:
                {
                    color = 0x000000FFu;
                } break;
                case 1:
                {
                    color <<= 28u;
                    color |= 0x00000FFu;
                } break;
                case 2:
                {
                    color <<= 24u;
                    color |= 0x0000FFu;
                } break;
                case 3:
                {
                    color <<= 20u;
                    color |= 0x000FFu;
                } break;
                case 4:
                {
                    color <<= 16u;
                    color |= 0x00FFu;
                } break;
                case 5:
                {
                    color <<= 12u;
                    color |= 0x0FFu;
                } break;
                case 6:
                {
                    color <<= 8u;
                    color |= 0xFFu;
                } break;
                case 7:
                {
                    color <<= 4u;
                    color |= 0x0u;
                } break;
              default: break;
            }
        }
        else
        {
            // Attempt to treat the value as a color name
            try
            {
                color = (::SqMod::GetColorStr(str).GetRGBA() | 0xFFu);
            }
            catch (...)
            {
                return sq_throwerror(vm, "Cannot identify color string");
            }
        }
        // The message starts right after the color
        msg_idx += 1;
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
    const auto top = static_cast< int32_t >(sq_gettop(vm));
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
    int32_t msg_idx = 2;
    // The message color
    uint32_t color = 0;
    // Attempt to identify and extract the color
    const SQRESULT res = SqGrabPlayerMessageColor(vm, 2, color, msg_idx);
    // Did we fail to identify a color?
    if (SQ_FAILED(res))
    {
        return res; // Propagate the error!
    }

    // Attempt to generate the string value
    StackStrF val(vm, msg_idx);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }

    // Obtain the ends of the entity pool
    auto itr = Core::Get().GetPlayers().cbegin();
    auto end = Core::Get().GetPlayers().cend();
    // The number of players that the message was sent to
    uint32_t count = 0;
    // Currently processed player
    CPlayer * player;

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
                return sq_throwerrorf(vm, "Client message too big [%s]", player->GetTag().c_str());
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
    const auto top = static_cast< int32_t >(sq_gettop(vm));
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
    uint32_t index;
    // Attempt to extract the argument values
    try
    {
        index = ConvTo< uint32_t >::From(Var< SQInteger >(vm, 2).value);
    }
    catch (const std::exception & e)
    {
        return sq_throwerror(vm, e.what());
    }

    // Perform a range check on the specified prefix index
    if (index >= SQMOD_PLAYER_MSG_PREFIXES)
    {
        return sq_throwerrorf(vm, "Prefix index is out of range: %u >= %u",
                                        index, SQMOD_PLAYER_MSG_PREFIXES);
    }

    // Attempt to generate the string value
    StackStrF val(vm, 3);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }

    vcmpError result;
    // Obtain the ends of the entity pool
    auto itr = Core::Get().GetPlayers().cbegin();
    auto end = Core::Get().GetPlayers().cend();
    // The number of players that the message was sent to
    uint32_t count = 0;
    // Currently processed player
    CPlayer * player;

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
                return sq_throwerrorf(vm, "Client message too big [%s]", player->GetTag().c_str());
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
    const auto top = static_cast< int32_t >(sq_gettop(vm));
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
    uint32_t index;
    // Attempt to extract the argument values
    try
    {
        index = ConvTo< uint32_t >::From(Var< SQInteger >(vm, 2).value);
    }
    catch (const std::exception & e)
    {
        return sq_throwerror(vm, e.what());
    }

    // Perform a range check on the specified prefix index
    if (index >= SQMOD_PLAYER_MSG_PREFIXES)
    {
        return sq_throwerrorf(vm, "Prefix index is out of range: %u >= %u",
                                        index, SQMOD_PLAYER_MSG_PREFIXES);
    }

    // The index where the message should start
    int32_t msg_idx = 3;
    // The message color
    uint32_t color = 0;
    // Attempt to identify and extract the color
    const SQRESULT res = SqGrabPlayerMessageColor(vm, 3, color, msg_idx);
    // Did we fail to identify a color?
    if (SQ_FAILED(res))
    {
        return res; // Propagate the error!
    }

    // Attempt to generate the string value
    StackStrF val(vm, msg_idx);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        return val.mRes; // Propagate the error!
    }

    vcmpError result;
    // Obtain the ends of the entity pool
    auto itr = Core::Get().GetPlayers().cbegin();
    auto end = Core::Get().GetPlayers().cend();
    // The number of players that the message was sent to
    uint32_t count = 0;
    // Currently processed player
    CPlayer * player;

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
                return sq_throwerrorf(vm, "Client message too big [%s]", player->GetTag().c_str());
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
    const auto top = static_cast< int32_t >(sq_gettop(vm));
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
    auto itr = Core::Get().GetPlayers().cbegin();
    auto end = Core::Get().GetPlayers().cend();
    // The number of players that the message was sent to
    uint32_t count = 0;
    // Currently processed player
    CPlayer * player;

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
                return sq_throwerrorf(vm, "Client message too big [%s]", player->GetTag().c_str());
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
    const auto top = static_cast< int32_t >(sq_gettop(vm));
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
    auto itr = Core::Get().GetPlayers().cbegin();
    auto end = Core::Get().GetPlayers().cend();
    // The number of players that the message was sent to
    uint32_t count = 0;
    // Currently processed player
    CPlayer * player;

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
                return sq_throwerrorf(vm, "Invalid announcement style %d [%s]",
                                                player->mAnnounceStyle, player->GetTag().c_str());
            }
            else if (result == vcmpErrorTooLargeInput)
            {
                return sq_throwerrorf(vm, "Game message too big [%s]", player->GetTag().c_str());
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
    const auto top = static_cast< int32_t >(sq_gettop(vm));
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

    int32_t style;
    // style to extract the argument values
    try
    {
        style = Var< int32_t >(vm, 2).value;
    }
    catch (const std::exception & e)
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
    auto itr = Core::Get().GetPlayers().cbegin();
    auto end = Core::Get().GetPlayers().cend();
    // The number of players that the message was sent to
    uint32_t count = 0;
    // Currently processed player
    CPlayer * player;

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
                return sq_throwerrorf(vm, "Invalid announcement style %d [%s]",
                                                style, player->GetTag().c_str());
            }
            else if (result == vcmpErrorTooLargeInput)
            {
                return sq_throwerrorf(vm, "Game message too big [%s]", player->GetTag().c_str());
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
