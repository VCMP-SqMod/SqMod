// ------------------------------------------------------------------------------------------------
#include "Event/Shared.hpp"
#include "Config.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const SQChar * GetEventName(SQInt32 type)
{
    switch (type)
    {
        case EVT_BLIPCREATED:           return _SC("Blip Created");
        case EVT_CHECKPOINTCREATED:     return _SC("Checkpoint Created");
        case EVT_KEYBINDCREATED:        return _SC("Keybind Created");
        case EVT_OBJECTCREATED:         return _SC("Object Created");
        case EVT_PICKUPCREATED:         return _SC("Pickup Created");
        case EVT_PLAYERCREATED:         return _SC("Player Created");
        case EVT_SPHERECREATED:         return _SC("Sphere Created");
        case EVT_SPRITECREATED:         return _SC("Sprite Created");
        case EVT_TEXTDRAWCREATED:       return _SC("Textdraw Created");
        case EVT_VEHICLECREATED:        return _SC("Vehicle Created");
        case EVT_BLIPDESTROYED:         return _SC("Blip Destroyed");
        case EVT_CHECKPOINTDESTROYED:   return _SC("Checkpoint Destroyed");
        case EVT_KEYBINDDESTROYED:      return _SC("Keybind Destroyed");
        case EVT_OBJECTDESTROYED:       return _SC("Object Destroyed");
        case EVT_PICKUPDESTROYED:       return _SC("Pickup Destroyed");
        case EVT_PLAYERDESTROYED:       return _SC("Player Destroyed");
        case EVT_SPHEREDESTROYED:       return _SC("Sphere Destroyed");
        case EVT_SPRITEDESTROYED:       return _SC("Sprite Destroyed");
        case EVT_TEXTDRAWDESTROYED:     return _SC("Textdraw Destroyed");
        case EVT_VEHICLEDESTROYED:      return _SC("Vehicle Destroyed");
        case EVT_BLIPCUSTOM:            return _SC("Blip Custom");
        case EVT_CHECKPOINTCUSTOM:      return _SC("Checkpoint Custom");
        case EVT_KEYBINDCUSTOM:         return _SC("Keybind Custom");
        case EVT_OBJECTCUSTOM:          return _SC("Object Custom");
        case EVT_PICKUPCUSTOM:          return _SC("Pickup Custom");
        case EVT_PLAYERCUSTOM:          return _SC("Player Custom");
        case EVT_SPHERECUSTOM:          return _SC("Sphere Custom");
        case EVT_SPRITECUSTOM:          return _SC("Sprite Custom");
        case EVT_TEXTDRAWCUSTOM:        return _SC("Textdraw Custom");
        case EVT_VEHICLECUSTOM:         return _SC("Vehicle Custom");
        case EVT_PLAYERAWAY:            return _SC("Player Away");
        case EVT_PLAYERGAMEKEYS:        return _SC("Player Game Keys");
        case EVT_PLAYERRENAME:          return _SC("Player Rename");
        case EVT_PLAYERREQUESTCLASS:    return _SC("Player Request Class");
        case EVT_PLAYERREQUESTSPAWN:    return _SC("Player Request Spawn");
        case EVT_PLAYERSPAWN:           return _SC("Player Spawn");
        case EVT_PLAYERSTARTTYPING:     return _SC("Player Start Typing");
        case EVT_PLAYERSTOPTYPING:      return _SC("Player Stop Typing");
        case EVT_PLAYERCHAT:            return _SC("Player Chat");
        case EVT_PLAYERCOMMAND:         return _SC("Player Command");
        case EVT_PLAYERMESSAGE:         return _SC("Player Message");
        case EVT_PLAYERHEALTH:          return _SC("Player Health");
        case EVT_PLAYERARMOUR:          return _SC("Player Armour");
        case EVT_PLAYERWEAPON:          return _SC("Player Weapon");
        case EVT_PLAYERMOVE:            return _SC("Player Move");
        case EVT_PLAYERWASTED:          return _SC("Player Wasted");
        case EVT_PLAYERKILLED:          return _SC("Player Killed");
        case EVT_PLAYERTEAMKILL:        return _SC("Player Team Kill");
        case EVT_PLAYERSPECTATE:        return _SC("Player Spectate");
        case EVT_PLAYERCRASHREPORT:     return _SC("Player Crash Report");
        case EVT_PLAYERBURNING:         return _SC("Player Burning");
        case EVT_PLAYERCROUCHING:       return _SC("Player Crouching");
        case EVT_PLAYERSTATE:           return _SC("Player State");
        case EVT_PLAYERACTION:          return _SC("Player Action");
        case EVT_STATENONE:             return _SC("State None");
        case EVT_STATENORMAL:           return _SC("State Normal");
        case EVT_STATESHOOTING:         return _SC("State Shooting");
        case EVT_STATEDRIVER:           return _SC("State Driver");
        case EVT_STATEPASSENGER:        return _SC("State Passenger");
        case EVT_STATEENTERDRIVER:      return _SC("State Enter Driver");
        case EVT_STATEENTERPASSENGER:   return _SC("State Enter Passenger");
        case EVT_STATEEXITVEHICLE:      return _SC("State Exit Vehicle");
        case EVT_STATEUNSPAWNED:        return _SC("State Unspawned");
        case EVT_ACTIONNONE:            return _SC("Action None");
        case EVT_ACTIONNORMAL:          return _SC("Action Normal");
        case EVT_ACTIONAIMING:          return _SC("Action Aiming");
        case EVT_ACTIONSHOOTING:        return _SC("Action Shooting");
        case EVT_ACTIONJUMPING:         return _SC("Action Jumping");
        case EVT_ACTIONLIEDOWN:         return _SC("Action Lie Down");
        case EVT_ACTIONGETTINGUP:       return _SC("Action Getting Up");
        case EVT_ACTIONJUMPVEHICLE:     return _SC("Action Jump Vehicle");
        case EVT_ACTIONDRIVING:         return _SC("Action Driving");
        case EVT_ACTIONDYING:           return _SC("Action Dying");
        case EVT_ACTIONWASTED:          return _SC("Action Wasted");
        case EVT_ACTIONEMBARKING:       return _SC("Action Embarking");
        case EVT_ACTIONDISEMBARKING:    return _SC("Action Disembarking");
        case EVT_VEHICLERESPAWN:        return _SC("Vehicle Respawn");
        case EVT_VEHICLEEXPLODE:        return _SC("Vehicle Explode");
        case EVT_VEHICLEHEALTH:         return _SC("Vehicle Health");
        case EVT_VEHICLEMOVE:           return _SC("Vehicle Move");
        case EVT_PICKUPRESPAWN:         return _SC("Pickup Respawn");
        case EVT_KEYBINDKEYPRESS:       return _SC("Keybind Key Press");
        case EVT_KEYBINDKEYRELEASE:     return _SC("Keybind Key Release");
        case EVT_VEHICLEEMBARKING:      return _SC("Vehicle Embarking");
        case EVT_VEHICLEEMBARKED:       return _SC("Vehicle Embarked");
        case EVT_VEHICLEDISEMBARK:      return _SC("Vehicle Disembark");
        case EVT_PICKUPCLAIMED:         return _SC("Pickup Claimed");
        case EVT_PICKUPCOLLECTED:       return _SC("Pickup Collected");
        case EVT_OBJECTSHOT:            return _SC("Object Shot");
        case EVT_OBJECTBUMP:            return _SC("Object Bump");
        case EVT_CHECKPOINTENTERED:     return _SC("Checkpoint Entered");
        case EVT_CHECKPOINTEXITED:      return _SC("Checkpoint Exited");
        case EVT_SPHEREENTERED:         return _SC("Sphere Entered");
        case EVT_SPHEREEXITED:          return _SC("Sphere Exited");
        case EVT_SERVERFRAME:           return _SC("Server Frame");
        case EVT_SERVERSTARTUP:         return _SC("Server Startup");
        case EVT_SERVERSHUTDOWN:        return _SC("Server Shutdown");
        case EVT_INTERNALCOMMAND:       return _SC("Internal Command");
        case EVT_LOGINATTEMPT:          return _SC("Login Attempt");
        case EVT_CUSTOMEVENT:           return _SC("Custom Event");
        case EVT_WORLDOPTION:           return _SC("World Option");
        case EVT_WORLDTOGGLE:           return _SC("World Toggle");
        case EVT_SCRIPTRELOAD:          return _SC("Script Reload");
        case EVT_LOGMESSAGE:            return _SC("Log Message");
        default:                        return _SC("Unknown");
    }

}

// ------------------------------------------------------------------------------------------------
bool IsEntityEvent(SQInt32 type)
{
    switch (type)
    {
        case EVT_BLIPCREATED:
        case EVT_CHECKPOINTCREATED:
        case EVT_KEYBINDCREATED:
        case EVT_OBJECTCREATED:
        case EVT_PICKUPCREATED:
        case EVT_PLAYERCREATED:
        case EVT_SPHERECREATED:
        case EVT_SPRITECREATED:
        case EVT_TEXTDRAWCREATED:
        case EVT_VEHICLECREATED:
        case EVT_BLIPDESTROYED:
        case EVT_CHECKPOINTDESTROYED:
        case EVT_KEYBINDDESTROYED:
        case EVT_OBJECTDESTROYED:
        case EVT_PICKUPDESTROYED:
        case EVT_PLAYERDESTROYED:
        case EVT_SPHEREDESTROYED:
        case EVT_SPRITEDESTROYED:
        case EVT_TEXTDRAWDESTROYED:
        case EVT_VEHICLEDESTROYED:
        case EVT_BLIPCUSTOM:
        case EVT_CHECKPOINTCUSTOM:
        case EVT_KEYBINDCUSTOM:
        case EVT_OBJECTCUSTOM:
        case EVT_PICKUPCUSTOM:
        case EVT_PLAYERCUSTOM:
        case EVT_SPHERECUSTOM:
        case EVT_SPRITECUSTOM:
        case EVT_TEXTDRAWCUSTOM:
        case EVT_VEHICLECUSTOM:
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
        case EVT_VEHICLERESPAWN:
        case EVT_VEHICLEEXPLODE:
        case EVT_VEHICLEHEALTH:
        case EVT_VEHICLEMOVE:
        case EVT_PICKUPRESPAWN:
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

// ------------------------------------------------------------------------------------------------
bool IsCreateEvent(SQInt32 type)
{
    switch (type)
    {
        case EVT_BLIPCREATED:
        case EVT_CHECKPOINTCREATED:
        case EVT_KEYBINDCREATED:
        case EVT_OBJECTCREATED:
        case EVT_PICKUPCREATED:
        case EVT_PLAYERCREATED:
        case EVT_SPHERECREATED:
        case EVT_SPRITECREATED:
        case EVT_TEXTDRAWCREATED:
        case EVT_VEHICLECREATED:
            return true;
        default:
            return false;
    }
}

// ------------------------------------------------------------------------------------------------
bool IsDestroyEvent(SQInt32 type)
{
    switch (type)
    {
        case EVT_BLIPDESTROYED:
        case EVT_CHECKPOINTDESTROYED:
        case EVT_KEYBINDDESTROYED:
        case EVT_OBJECTDESTROYED:
        case EVT_PICKUPDESTROYED:
        case EVT_PLAYERDESTROYED:
        case EVT_SPHEREDESTROYED:
        case EVT_SPRITEDESTROYED:
        case EVT_TEXTDRAWDESTROYED:
        case EVT_VEHICLEDESTROYED:
            return true;
        default:
            return false;
    }
}

// ------------------------------------------------------------------------------------------------
bool IsCustomEvent(SQInt32 type)
{
    switch (type)
    {
        case EVT_BLIPCUSTOM:
        case EVT_CHECKPOINTCUSTOM:
        case EVT_KEYBINDCUSTOM:
        case EVT_OBJECTCUSTOM:
        case EVT_PICKUPCUSTOM:
        case EVT_PLAYERCUSTOM:
        case EVT_SPHERECUSTOM:
        case EVT_SPRITECUSTOM:
        case EVT_TEXTDRAWCUSTOM:
        case EVT_VEHICLECUSTOM:
            return true;
        default:
            return false;
    }
}

// ------------------------------------------------------------------------------------------------
bool CanBeInversed(SQInt32 type)
{
    switch (type)
    {
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

// ================================================================================================
bool Register_Event(HSQUIRRELVM vm)
{
    SQMOD_UNUSED_VAR(vm);
    return true;
}

} // Namespace:: SqMod
