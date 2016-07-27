// ------------------------------------------------------------------------------------------------
#include "Misc/Weapon.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstring>
#include <algorithm>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
static String CS_Weapon_Names[] = {
"Unarmed",                   "Brass Knuckles",            "Screwdriver",               "Golf Club",
"Nightstick",                "Knife",                     "Baseball Bat",              "Hammer",
"Meat Cleaver",              "Machete",                   "Katana",                    "Chainsaw",
"Grenade",                   "Remote Detonation Grenade", "Tear Gas",                  "Molotov Cocktails",
"Missile",                   "Colt .45",                  "Python",                    "Pump-Action Shotgun",
"SPAS-12 Shotgun",           "Stubby Shotgun",            "TEC-9",                     "Uzi",
"Silenced Ingram",           "MP5",                       "M4",                        "Ruger",
"Sniper Rifle",              "Laserscope Sniper Rifle",   "Rocket Launcher",           "Flamethrower",
"M60",                       "Minigun",                   "Bomb",                      "Helicannon",
"Camera",                    "",                          "",                          "Vehicle",
"",                          "Explosion",                 "Driveby",                   "Drowned",
"Fall",                      "",                          "",                          "",
"",                          "",                          "",                          "Explosion",
"",                          "",                          "",                          "",
"",                          "",                          "",                          "",
"",                          "",                          "",                          "",
"",                          "",                          "",                          "",
"",                          "",                          "Suicide",                   ""
};

// ------------------------------------------------------------------------------------------------
CCStr GetWeaponName(Uint32 id)
{
    return (id > 70) ? _SC("") : CS_Weapon_Names[id].c_str();
}

// ------------------------------------------------------------------------------------------------
void SetWeaponName(Uint32 id, CCStr name)
{
    if (id <= 70)
    {
        CS_Weapon_Names[id].assign(name);
    }
}

// ------------------------------------------------------------------------------------------------
Int32 GetWeaponID(CCStr name)
{
    // Clone the string into an editable version
    String str(name);
    // Strip non alphanumeric characters from the name
    str.erase(std::remove_if(str.begin(), str.end(), std::not1(std::ptr_fun(::isalnum))), str.end());
    // Convert the string to lowercase
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    // See if we still have a valid name after the cleanup
    if(str.length() < 1)
    {
        return SQMOD_UNKNOWN;
    }
    // Grab the actual length of the string
    Uint32 len = static_cast< Uint32 >(str.length());
    // Get the most significant characters used to identify a weapon
    CharT a = str[0], b = 0, c = 0, d = str[len-1];
    // Look for deeper specifiers
    if(len >= 3)
    {
        c = str[2];
        b = str[1];
    }
    else if(len >= 2)
    {
        b = str[1];
    }
    // Search for a pattern in the name
    switch(a)
    {
        // [B]aseball Bat
        // [B]omb
        // [B]rass Knuckles
        case 'b':
            // [Ba]seball Bat
            if (b == 'a') return SQMOD_WEAPON_BASEBALLBAT;
            // [Bo]mb
            else if (b == 'o') return SQMOD_WEAPON_BOMB;
            // [Br]ass Knuckles
            else if (b == 'r') return SQMOD_WEAPON_BRASSKNUCKLES;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [C]amera
        // [C]hainsaw
        // [C]olt .45
        case 'c':
            // [Ca]mera
            if (b == 'a') return SQMOD_WEAPON_CAMERA;
            // [Ch]ainsaw
            else if (b == 'h') return SQMOD_WEAPON_CHAINSAW;
            // [Co]lt .45
            else if (b == 'o') return SQMOD_WEAPON_COLT45;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [D]riveby
        // [D]rowned
        case 'd':
            // [Dr]iveby
            if (b == 'r' && (c == 'i' || d == 'y')) return SQMOD_WEAPON_DRIVEBY;
            // [Dr]owned
            else if (b == 'r' && (c == 'o' || d == 'd')) return SQMOD_WEAPON_DROWNED;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [E]xplosion
        case 'e': return SQMOD_WEAPON_EXPLOSION2;
        // [F]all
        // [F]lamethrower
        case 'f':
            // [Fa]ll
            if (b == 'a') return SQMOD_WEAPON_FALL;
            // [Fl]amethrower
            else if (b == 'l') return SQMOD_WEAPON_FLAMETHROWER;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [G]olf Club
        // [G]renade
        case 'g':
            // [Go]lf Club
            if (b == 'o') return SQMOD_WEAPON_GOLFCLUB;
            // [Gr]enade
            else if (b == 'r') return SQMOD_WEAPON_GRENADE;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [H]ammer
        // [H]eliblades
        // [H]elicannon
        case 'h':
            // [Ha]mmer
            if (b == 'a') return SQMOD_WEAPON_HAMMER;
            // [He]li[b]lade[s]
            else if (b == 'e' && (d == 's' || d == 'b' || (len > 4 && str[4] == 'b'))) return SQMOD_WEAPON_HELIBLADES;
            // [He]li[c]anno[n]
            else if (b == 'e' && (d == 'c' || d == 'n' || (len > 4 && str[4] == 'c'))) return SQMOD_WEAPON_HELICANNON;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [K]atana
        // [K]nife
        case 'k':
            // [Ka]tana
            if (b == 'a') return SQMOD_WEAPON_KATANA;
            // [Kn]ife
            else if (b == 'n') return SQMOD_WEAPON_KNIFE;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [L]aserscope Sniper Rifle
        case 'l': return SQMOD_WEAPON_LASERSCOPE;
        // [M]4
        // [M]60
        // [M]achete
        // [M]eat Cleaver
        // [M]inigun
        // [M]issile
        // [M]olotov Cocktails
        // [M]P5
        case 'm':
            // [M4]
            if (b == '4') return SQMOD_WEAPON_M4;
            // [M6]0
            else if (b == '6') return SQMOD_WEAPON_M60;
            // [Ma]chete
            else if (b == 'a') return SQMOD_WEAPON_MACHETE;
            // [Me]at Cleaver
            else if (b == 'e') return SQMOD_WEAPON_MEATCLEAVER;
            // [Min]igu[n]
            else if (b == 'i' && (c == 'n' || d == 'n')) return SQMOD_WEAPON_MINIGUN;
            // [Mis]sil[e]
            else if (b == 'i' && (c == 's' || d == 's' || d == 'e')) return SQMOD_WEAPON_ROCKET;
            // [Mo]lotov Cocktails
            else if (b == 'o') return SQMOD_WEAPON_MOLOTOV;
            // [MP]5
            else if (b == 'p') return SQMOD_WEAPON_MP5;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [N]ightstick
        case 'n': return SQMOD_WEAPON_NIGHTSTICK;
        case 'i': return SQMOD_WEAPON_INGRAM;
        // [P]ump-Action Shotgun
        // [P]ython
        case 'p':
            // [Pu]mp-Action Shotgun
            if (b == 'u') return SQMOD_WEAPON_SHOTGUN;
            // [Py]thon
            else if (b == 'y') return SQMOD_WEAPON_PYTHON;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [R]emote Detonation Grenade
        // [R]ocket Launcher
        // [R]uger
        case 'r':
            // [Re]mote Detonation Grenade
            if (b == 'e') return SQMOD_WEAPON_REMOTE;
            // [Ro]cket [L]aunche[r]
            else if (b == 'o' && (d == 'r' || d == 'l' || (len > 6 && str[6] == 'l'))) return SQMOD_WEAPON_ROCKETLAUNCHER;
            // [Ru]ger
            else if (b == 'u') return SQMOD_WEAPON_RUGER;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [S]crewdriver
        // [S]ilenced Ingram
        // [S]niper Rifle
        // [S]PAS-12 Shotgun
        // [S]tubby Shotgun
        // [S]uicide
        case 's':
            // [Sc]rewdriver
            if (b == 'c') return SQMOD_WEAPON_SCREWDRIVER;
            // [Si]lenced Ingram
            else if (b == 'i') return SQMOD_WEAPON_INGRAM;
            // [Sn]iper Rifle
            else if (b == 'n') return SQMOD_WEAPON_SNIPER;
            // [SP]AS-12 Shotgun
            else if (b == 'p') return SQMOD_WEAPON_SPAS12;
            // [St]ubby Shotgun
            else if (b == 't') return SQMOD_WEAPON_STUBBY;
            // [Su]icide
            else if (b == 'u') return SQMOD_WEAPON_SUICIDE;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [T]ear Gas
        // [T]EC-9
        case 't':
            // [Tea]r Ga[s]
            if (b == 'e' && (c == 'a' || d == 's')) return SQMOD_WEAPON_TEARGAS;
            // [TEC]-[9]
            else if (b == 'e' && (c == 'c' || d == '9')) return SQMOD_WEAPON_TEC9;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [U]narmed
        // [U]zi
        case 'u':
            // [Un]armed
            if (b == 'n') return SQMOD_WEAPON_UNARMED;
            // [Uz]i
            else if (b == 'z') return SQMOD_WEAPON_UZI;
            // Default to unknwon
            else return SQMOD_UNKNOWN;
        // [V]ehicle
        case 'v': return SQMOD_WEAPON_VEHICLE;
        // Default to unknwon
        default: return SQMOD_UNKNOWN;
    }
}

// ------------------------------------------------------------------------------------------------
bool IsWeaponValid(Int32 id)
{
    CSStr name = GetWeaponName(id);
    return (name && *name != '\0');
}

// ------------------------------------------------------------------------------------------------
Int32 WeaponToModel(Int32 id)
{
    switch (id)
    {
        case SQMOD_WEAPON_UNARMED:          return 293;
        case SQMOD_WEAPON_BRASSKNUCKLES:    return 259;
        case SQMOD_WEAPON_SCREWDRIVER:      return 260;
        case SQMOD_WEAPON_GOLFCLUB:         return 261;
        case SQMOD_WEAPON_NIGHTSTICK:       return 262;
        case SQMOD_WEAPON_KNIFE:            return 263;
        case SQMOD_WEAPON_BASEBALLBAT:      return 264;
        case SQMOD_WEAPON_HAMMER:           return 265;
        case SQMOD_WEAPON_MEATCLEAVER:      return 266;
        case SQMOD_WEAPON_MACHETE:          return 267;
        case SQMOD_WEAPON_KATANA:           return 268;
        case SQMOD_WEAPON_CHAINSAW:         return 269;
        case SQMOD_WEAPON_GRENADE:          return 270;
        case SQMOD_WEAPON_REMOTE:           return 291;
        case SQMOD_WEAPON_TEARGAS:          return 271;
        case SQMOD_WEAPON_MOLOTOV:          return 272;
        case SQMOD_WEAPON_ROCKET:           return 273;
        case SQMOD_WEAPON_COLT45:           return 274;
        case SQMOD_WEAPON_PYTHON:           return 275;
        case SQMOD_WEAPON_SHOTGUN:          return 277;
        case SQMOD_WEAPON_SPAS12:           return 278;
        case SQMOD_WEAPON_STUBBY:           return 279;
        case SQMOD_WEAPON_TEC9:             return 281;
        case SQMOD_WEAPON_UZI:              return 282;
        case SQMOD_WEAPON_INGRAM:           return 283;
        case SQMOD_WEAPON_MP5:              return 284;
        case SQMOD_WEAPON_M4:               return 280;
        case SQMOD_WEAPON_RUGER:            return 276;
        case SQMOD_WEAPON_SNIPER:           return 285;
        case SQMOD_WEAPON_LASERSCOPE:       return 286;
        case SQMOD_WEAPON_ROCKETLAUNCHER:   return 287;
        case SQMOD_WEAPON_FLAMETHROWER:     return 288;
        case SQMOD_WEAPON_M60:              return 289;
        case SQMOD_WEAPON_MINIGUN:          return 290;
        case SQMOD_WEAPON_HELICANNON:       return 294;
        case SQMOD_WEAPON_CAMERA:           return 292;
        default:                            return SQMOD_UNKNOWN;
    }
}

// ------------------------------------------------------------------------------------------------
bool IsWeaponNatural(Int32 id)
{
    switch (id)
    {
        case SQMOD_WEAPON_VEHICLE:
        case SQMOD_WEAPON_DRIVEBY:
        case SQMOD_WEAPON_DROWNED:
        case SQMOD_WEAPON_FALL:
        case SQMOD_WEAPON_EXPLOSION2:
        case SQMOD_WEAPON_SUICIDE:  return true;
        default:                    return false;
    }
}

} // Namespace:: SqMod
