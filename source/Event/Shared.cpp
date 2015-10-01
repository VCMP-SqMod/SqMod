// ------------------------------------------------------------------------------------------------
#include "Event/Shared.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const SQChar * GetEventName(EventType evt)
{
    switch (evt)
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

// ================================================================================================
bool Register_Event(HSQUIRRELVM vm)
{
    return true;
}

} // Namespace:: SqMod
