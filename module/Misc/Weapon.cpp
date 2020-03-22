// ------------------------------------------------------------------------------------------------
#include "Misc/Weapon.hpp"
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstring>
#include <algorithm>
#include <unordered_map>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
static String CS_Weapon_Names[] = { // NOLINT(cert-err58-cpp)
    /* 0  */ "Unarmed",                     /* 1  */ "Brass Knuckles",
    /* 2  */ "Screwdriver",                 /* 3  */ "Golf Club",
    /* 4  */ "Nightstick",                  /* 5  */ "Knife",
    /* 6  */ "Baseball Bat",                /* 7  */ "Hammer",
    /* 8  */ "Meat Cleaver",                /* 9  */ "Machete",
    /* 10 */ "Katana",                      /* 11 */ "Chainsaw",
    /* 12 */ "Grenade",                     /* 13 */ "Remote Detonation Grenade",
    /* 14 */ "Tear Gas",                    /* 15 */ "Molotov Cocktails",
    /* 16 */ "Missile",                     /* 17 */ "Colt .45",
    /* 18 */ "Python",                      /* 19 */ "Pump-Action Shotgun",
    /* 20 */ "SPAS-12 Shotgun",             /* 21 */ "Stubby Shotgun",
    /* 22 */ "TEC-9",                       /* 23 */ "Uzi",
    /* 24 */ "Silenced Ingram",             /* 25 */ "MP5",
    /* 26 */ "M4",                          /* 27 */ "Ruger",
    /* 28 */ "Sniper Rifle",                /* 29 */ "Laserscope Sniper Rifle",
    /* 30 */ "Rocket Launcher",             /* 31 */ "Flamethrower",
    /* 32 */ "M60",                         /* 33 */ "Minigun",
    /* 34 */ "Bomb",                        /* 35 */ "Helicanon",
    /* 36 */ "Camera",                      /* 37 */ "",
    /* 38 */ "",                            /* 39 */ "Vehicle",
    /* 40 */ "Heliblade",                   /* 41 */ "Explosion",
    /* 42 */ "Driveby",                     /* 43 */ "Drowned",
    /* 44 */ "Fall",                        /* 45 */ "",
    /* 46 */ "",                            /* 47 */ "",
    /* 48 */ "",                            /* 49 */ "",
    /* 50 */ "",                            /* 51 */ "Explosion",
    /* 52 */ "",                            /* 53 */ "",
    /* 54 */ "",                            /* 55 */ "",
    /* 56 */ "",                            /* 57 */ "",
    /* 58 */ "",                            /* 59 */ "",
    /* 60 */ "Heliblade",                   /* 61 */ "",
    /* 62 */ "",                            /* 63 */ "",
    /* 64 */ "",                            /* 65 */ "",
    /* 66 */ "",                            /* 67 */ "",
    /* 68 */ "",                            /* 69 */ "",
    /* 70 */ "Suicide",                     /* 71 */ ""
};
/// Fall back for custom weapon names.
static std::unordered_map<Uint32, String> CS_Custom_Weapon_Names{};

// ------------------------------------------------------------------------------------------------
static inline bool IsCustomWeapon(Uint32 id)
{
	return (id > 70);
}

// ------------------------------------------------------------------------------------------------
Uint32 GetWeaponSlot(Uint32 id)
{
	switch(id) {
        case 0:
		case 1:
			return 0;
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
			return 1;
		case 12:
		case 13:
		case 14:
		case 15:
			return 2;
		case 17:
		case 18:
			return 3;
		case 19:
		case 20:
		case 21:
			return 4;
		case 22:
		case 23:
		case 24:
		case 25:
			return 5;
		case 26:
		case 27:
			return 6;
		case 28:
		case 29:
			return 8;
		case 30:
		case 31:
		case 32:
		case 33:
			return 7;
		default:
            return 255;
	}
}

// ------------------------------------------------------------------------------------------------
CCStr GetWeaponName(Uint32 id)
{
	// Can we consider this a custom weapon ID?
	if (IsCustomWeapon(id))
	{
		// Attempt to look for the specified identifier
		auto it = CS_Custom_Weapon_Names.find(id);
		// If we found anything than return the associated name
		if ((it != CS_Custom_Weapon_Names.end()))
		{
			return it->second.c_str();
		}
	}
	else
	{
		return CS_Weapon_Names[id].c_str(); // Use the standard weapon name
	}
	// Fall back to an empty name
    return _SC("");
}

// ------------------------------------------------------------------------------------------------
void SetWeaponName(Uint32 id, StackStrF & name)
{
	// Can we consider this a custom weapon ID?
	if (IsCustomWeapon(id))
	{
		// Attempt to insert or update the name into the custom weapon table
		CS_Custom_Weapon_Names[id] = String(name.mPtr, static_cast< size_t >(name.mLen));
	}
	else
	{
		// Attempt to insert or update the name into the standard weapon table
        CS_Weapon_Names[id].assign(name.mPtr);
	}
}

// ------------------------------------------------------------------------------------------------
Uint32 GetCustomWeaponNamePoolSize()
{
	return static_cast< Uint32 >(CS_Custom_Weapon_Names.size());
}

// ------------------------------------------------------------------------------------------------
void ClearCustomWeaponNamePool()
{
	CS_Custom_Weapon_Names.clear();
}

// ------------------------------------------------------------------------------------------------
Int32 GetWeaponID(StackStrF & name)
{
    // Clone the string into an editable version
    String str(name.mPtr, static_cast< size_t >(name.mLen));
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
    const Uint32 len = ConvTo< Uint32 >::From(str.length());
    // Get the most significant characters used to identify a weapon
    CharT a = str[0], b = 0, c = 0, d = str[len-1];
    // Look for deeper specifiers
    if(len > 2)
    {
        c = str[2];
        b = str[1];
    }
    else if(len > 1)
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
        // [H]eliblades1/A
        // [H]eliblades2/B
        // [H]elicannon
        case 'h':
            // [Ha]mmer
            if (b == 'a') return SQMOD_WEAPON_HAMMER;
            // [He]li[b]lades[1/A]
            else if (b == 'e' && (c == 'b' || (len > 4 && str[4] == 'b')) && (d == '1' || d == 'a')) return SQMOD_WEAPON_HELIBLADES1;
            // [He]li[b]lades[2/B]
            else if (b == 'e' && (c == 'b' || (len > 4 && str[4] == 'b')) && (d == '2' || d == 'b')) return SQMOD_WEAPON_HELIBLADES2;
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
            // [Ro]cket Launcher
            else if (b == 'o') return SQMOD_WEAPON_ROCKETLAUNCHER;
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
	    // Pump action [Sh]otgun
            else if (b == 'h') return SQMOD_WEAPON_SHOTGUN;
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
    CSStr name = GetWeaponName(static_cast< Uint32 >(id));
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
