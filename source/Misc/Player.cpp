// ------------------------------------------------------------------------------------------------
#include "Misc/Player.hpp"
#include "Base/Color3.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstring>
#include <algorithm>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
static String CS_Skin_Names[] = {
"Tommy Vercetti",        "Cop",                   "SWAT",                  "FBI",                   "Army",
"Paramedic",             "Firefighter",           "Golf Guy #1",           "",                      "Bum Lady #1",
"Bum Lady #2",           "Punk #1",               "Lawyer",                "Spanish Lady #1",       "Spanish Lady #2",
"Cool Guy #1",           "Arabic Guy",            "Beach Lady #1",         "Beach Lady #2",         "Beach Guy #1",
"Beach Guy #2",          "Office Lady #1",        "Waitress #1",           "Food Lady",             "Prostitute #1",
"Bum Lady #3",           "Bum Guy #1",            "Garbageman #1",         "Taxi Driver #1",        "Haitian #1",
"Criminal #1",           "Hood Lady",             "Granny #1",             "Businessman #1",        "Church Guy",
"Club Lady",             "Church Lady",           "Pimp",                  "Beach Lady #3",         "Beach Guy #3",
"Beach Lady #4",         "Beach Guy #4",          "Businessman #2",        "Prostitute #2",         "Bum Lady #4",
"Bum Guy #2",            "Haitian #2",            "Construction Worker #1","Punk #2",               "Prostitute #3",
"Granny #2",             "Punk #3",               "Businessman #3",        "Spanish Lady #3",       "Spanish Lady #4",
"Cool Guy #2",           "Businessman #4",        "Beach Lady #5",         "Beach Guy #5",          "Beach Lady #6",
"Beach Guy #6",          "Construction Worker #2","Golf Guy #2",           "Golf Lady",             "Golf Guy #3",
"Beach Lady #7",         "Beach Guy #7",          "Office Lady #2",        "Businessman #5",        "Businessman #6",
"Prostitute #2",         "Bum Lady #4",           "Bum Guy #3",            "Spanish Guy",           "Taxi Driver #2",
"Gym Lady",              "Gym Guy",               "Skate Lady",            "Skate Guy",             "Shopper #1",
"Shopper #2",            "Tourist #1",            "Tourist #2",            "Cuban #1",              "Cuban #2",
"Haitian #3",            "Haitian #4",            "Shark #1",              "Shark #2",              "Diaz Guy #1",
"Diaz Guy #2",           "DBP Security #1",       "DBP Security #2",       "Biker #1",              "Biker #2",
"Vercetti Guy #1",       "Vercetti Guy #2",       "Undercover Cop #1",     "Undercover Cop #2",     "Undercover Cop #3",
"Undercover Cop #4",     "Undercover Cop #5",     "Undercover Cop #6",     "Rich Guy",              "Cool Guy #3",
"Prostitute #3",         "Prostitute #4",         "Love Fist #1",          "Ken Rosenburg",         "Candy Suxx",
"Hilary",                "Love Fist #2",          "Phil",                  "Rockstar Guy",          "Sonny",
"Lance",                 "Mercedes",              "Love Fist #3",          "Alex Shrub",            "Lance (Cop)",
"Lance",                 "Cortez",                "Love Fist #4",          "Columbian Guy #1",      "Hilary (Robber)",
"Mercedes",              "Cam",                   "Cam (Robber)",          "Phil (One Arm)",        "Phil (Robber)",
"Cool Guy #4",           "Pizza Man",             "Taxi Driver #1",        "Taxi Driver #2",        "Sailor #1",
"Sailor #2",             "Sailor #3",             "Chef",                  "Criminal #2",           "French Guy",
"Garbageman #2",         "Haitian #5",            "Waitress #2",           "Sonny Guy #1",          "Sonny Guy #2",
"Sonny Guy #3",          "Columbian Guy #2",      "Haitian #6",            "Beach Guy #8",          "Garbageman #3",
"Garbageman #4",         "Garbageman #5",         "Tranny",                "Thug #5",               "SpandEx Guy #1",
"SpandEx Guy #2",        "Stripper #1",           "Stripper #2",           "Stripper #3",           "Store Clerk",
""
};

// ------------------------------------------------------------------------------------------------
CCStr GetSkinName(Uint32 id)
{
    return (id > 159) ? _SC("") : CS_Skin_Names[id].c_str();
}

// ------------------------------------------------------------------------------------------------
void SetSkinName(Uint32 id, CCStr name)
{
    if (id <= 159)
        CS_Skin_Names[id].assign(name);
}

// ------------------------------------------------------------------------------------------------
Int32 GetSkinID(CCStr name)
{
    // Clone the string into an editable version
    String str(name);
    // Strip non alphanumeric characters from the name
    str.erase(std::remove_if(str.begin(), str.end(), std::not1(std::ptr_fun(::isalnum))), str.end());
    // Convert the string to lowercase
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    // See if we still have a valid name after the cleanup
    if(str.empty())
        return SQMOD_UNKNOWN;
    // Grab the actual length of the string
    Uint32 len = static_cast< Uint32 >(str.length());
    // Get the most significant characters used to identify a skin
    CharT a = str[0], b = 0, c = 0, d = str[len-1];
    // Look for deeper specifiers
    if (str.length() >= 3)
    {
        b = str[1];
        c = str[2];
    }
    else if (str.length() >= 2)
        b = str[1];
    // Search for a pattern in the name
    switch (a)
    {
        // [A]lex Srub, [A]rabic guy, [A]rmy
        case 'a':
            switch (b)
            {
                // [Al]ex [S]rub
                case 'l':
                case 's': return SQMOD_SKIN_ALEX_SRUB;
                // [A]rabic [g]uy
                case 'g': return SQMOD_SKIN_ARABIC_GUY;
                // [Ara]bic guy, [Arm]y
                case 'r':
                    if (c == 'a') return SQMOD_SKIN_ARABIC_GUY;
                    else if (c == 'm') return SQMOD_SKIN_ARMY;
            }
        // [B]each guy (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)/(#6|F)/(#7|G)/(#8|H)
        // [B]each lady (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)/(#6|F)/(#7|G)
        // [B]iker (#1|A)/(#2|B)
        // [B]um guy (#1|A)/(#2|B)/(#3|C)
        // [B]um lady (#1|A)/(#2|B)/(#2|C)/(#3|D)/(#4|E)
        // [B]usiness man (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)/(#6|F)
        case 'b':
            // [Be]ach [g]uy (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)/(#6|F)/(#7|G)/(#8|H)
            if (b == 'e' && (c == 'g' || (len > 4 && str[5] == 'g')))
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_BEACH_GUY_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_BEACH_GUY_B;
                    case '3':
                    case 'c': return SQMOD_SKIN_BEACH_GUY_C;
                    case '4':
                    case 'd': return SQMOD_SKIN_BEACH_GUY_D;
                    case '5':
                    case 'e': return SQMOD_SKIN_BEACH_GUY_E;
                    case '6':
                    case 'f': return SQMOD_SKIN_BEACH_GUY_F;
                    case '7':
                    case 'g': return SQMOD_SKIN_BEACH_GUY_G;
                    case '8':
                    case 'h': return SQMOD_SKIN_BEACH_GUY_H;
                }
            }
            // [Be]ach [l]ady (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)/(#6|F)/(#7|G)
            else if (b == 'e' && (c == 'l' || (len > 4 && str[5] == 'l')))
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_BEACH_LADY_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_BEACH_LADY_B;
                    case '3':
                    case 'c': return SQMOD_SKIN_BEACH_LADY_C;
                    case '4':
                    case 'd': return SQMOD_SKIN_BEACH_LADY_D;
                    case '5':
                    case 'e': return SQMOD_SKIN_BEACH_LADY_E;
                    case '6':
                    case 'f': return SQMOD_SKIN_BEACH_LADY_F;
                    case '7':
                    case 'g': return SQMOD_SKIN_BEACH_LADY_G;
                }
            }
            // [Bi]ker (#1|A)/(#2|B)
            else if (b == 'i' && (d == '1' || d == 'a')) return SQMOD_SKIN_BIKER_A;
            else if (b == 'i' && (d == '2' || d == 'b')) return SQMOD_SKIN_BIKER_B;
            // [Bum] [g]uy (#1|A)/(#2|B)/(#3|C)
            // [Bum] [l]ady (#1|A)/(#2|B)/(#2|C)/(#3|D)/(#4|E)
            else if (b == 'u' && (c && (c == 'm' || c == 'g' || c == 'l')))
            {
                // [Bum] [g]uy (#1|A)/(#2|B)/(#3|C)
                if (c == 'g' || (len > 2 && str[3] == 'g'))
                {
                    if (d == '1' || d == 'a') return SQMOD_SKIN_BUM_GUY_A;
                    else if (d == '2' || d == 'b') return SQMOD_SKIN_BUM_GUY_B;
                    else if (d == '3' || d == 'c') return SQMOD_SKIN_BUM_GUY_C;
                }
                // [Bum] [l]ady (#1|A)/(#2|B)/(#2|C)/(#3|D)/(#4|E)
                else if (c == 'l' || (len > 2 && str[3] == 'l'))
                {
                    if (d == '1' || d == 'a') return SQMOD_SKIN_BUM_LADY_A;
                    else if (d == '2' || d == 'b') return SQMOD_SKIN_BUM_LADY_B;
                    else if (d == '2' || d == 'c') return SQMOD_SKIN_BUM_LADY_C;
                    else if (d == '3' || d == 'd') return SQMOD_SKIN_BUM_LADY_D;
                    else if (d == '4' || d == 'e') return SQMOD_SKIN_BUM_LADY_E;
                }
            }
            // [Bus]iness [m]an (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)/(#6|F)
            else if (b == 'u' && (c == 's' || (len > 8 && str[9] == 'm')))
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_BUSINESS_MAN_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_BUSINESS_MAN_B;
                    case '3':
                    case 'c': return SQMOD_SKIN_BUSINESS_MAN_C;
                    case '4':
                    case 'd': return SQMOD_SKIN_BUSINESS_MAN_D;
                    case '5':
                    case 'e': return SQMOD_SKIN_BUSINESS_MAN_E;
                    case '6':
                    case 'f': return SQMOD_SKIN_BUSINESS_MAN_F;
                }
            }
        // [C]am, [C]am (Robber), [C]andy Suxx, [C]hef
        // [C]hurch guy, [C]hurch lady, [C]lub lady
        // [C]olumbian guy (#1|A)/(#2|B),
        // [C]onstruction worker (#1|A)/(#2|B)
        // [C]ool guy (#1|A)/(#2|B)/(#3|C)/(#4|D)
        // [C]op, [C]ortez
        // [C]riminal (#1|A)/(#2|B)
        // [C]uban (#1|A)/(#2|B)
        case 'c':
            // [Ca]m, [Ca]m (Robber), [Ca]ndy Suxx
            if (b == 'a')
            {
                // [Cam] ([R]obbe[r])
                if (c && (c == 'm' || c == 'r') && d == 'r') return SQMOD_SKIN_CAM_ROBBER;
                // [Cam]
                else if (c == 'm') return SQMOD_SKIN_CAM;
                // [Can]dy [S]ux[x]
                else if (c && (c == 'n' || c == 's' || d == 'x')) return SQMOD_SKIN_CANDY_SUXX;
            }
            // [Ch]ef, [Ch]urch guy, [Ch]urch lady
            else if (b == 'h')
            {
                // [Che][f]
                if (c && (c == 'e' || d == 'f')) return SQMOD_SKIN_CHEF;
                // [Chu]rch [g]uy
                else if (c && ((c == 'u' && len > 5 && str[6] == 'g') || (c == 'g')))
                    return SQMOD_SKIN_CHURCH_GUY;
                // [Chu]rch [l]ady
                else if (c && ((c == 'u' && len > 5 && str[6] == 'l') || (c == 'l')))
                    return SQMOD_SKIN_CHURCH_LADY;
            }
            // [Cl]ub [l]ady
            else if (b == 'l') return SQMOD_SKIN_CLUB_LADY;
            // [Co]lumbian guy (#1|A)/(#2|B)
            // [Co]nstruction worker (#1|A)/(#2|B)
            // [Co]ol guy (#1|A)/(#2|B)/(#3|C)/(#4|D)
            // [Co]p, [Co]rtez
            else if (b == 'o')
            {
                // [Col]umbian [g]uy (#1|A)/(#2|B)
                if (c && ((c == 'l' && len > 8 && str[9] == 'g') || (c == 'g')))
                {
                    if (d == '1' || d == 'a') return SQMOD_SKIN_COLUMBIAN_GUY_A;
                    else if (d == '2' || d == 'b') return SQMOD_SKIN_COLUMBIAN_GUY_B;
                }
                // [Con]struction [w]orker (#1|A)/(#2|B)
                else if (c && (c == 'n' || (len > 11 && str[12] == 'g')))
                {
                    if (d == '1' || d == 'a') return SQMOD_SKIN_CONSTRUCTION_WORKER_A;
                    else if (d == '2' || d == 'b') return SQMOD_SKIN_CONSTRUCTION_WORKER_B;
                }
                // [Coo]l guy (#1|A)/(#2|B)/(#3|C)/(#4|D)
                else if (c == 'o')
                {
                    switch (d)
                    {
                        case '1':
                        case 'a': return SQMOD_SKIN_COOL_GUY_A;
                        case '2':
                        case 'b': return SQMOD_SKIN_COOL_GUY_B;
                        case '3':
                        case 'c': return SQMOD_SKIN_COOL_GUY_C;
                        case '4':
                        case 'd': return SQMOD_SKIN_COOL_GUY_D;
                    }
                }
                // [Cop]
                else if (c == 'p') return SQMOD_SKIN_COP;
                // [Cor]te[z]
                else if (c && (c == 'r' || c == 'z' || d == 'z')) return SQMOD_SKIN_CORTEZ;
            }
            // [Cr]iminal (#1|A)/(#2|B)
            else if (b == 'r' && (d == '1' || d == 'a')) return SQMOD_SKIN_CRIMINAL_A;
            else if (b == 'r' && (d == '2' || d == 'b')) return SQMOD_SKIN_CRIMINAL_B;
            // [Cu]ban (#1|A)/(#2|B)
            else if (b == 'u' && (d == '1' || d == 'a')) return SQMOD_SKIN_CUBAN_A;
            else if (b == 'u' && (d == '2' || d == 'b')) return SQMOD_SKIN_CUBAN_B;
        // [D]BP security (#1|A)/(#2|B)
        // [D]iaz guy (#1|A)/(#2|B)
        case 'd':
            switch (b)
            {
                // [DB]P [s]ecurity (#1|A)/(#2|B)
                case 'b':
                case 's':
                    if (d == '1' || d == 'a') return SQMOD_SKIN_DBP_SECURITY_A;
                    else if (d == '2' || d == 'b') return SQMOD_SKIN_DBP_SECURITY_B;
                // [Di]a[z] [g]uy (#1|A)/(#2|B)
                case 'i':
                case 'z':
                case 'g':
                    if (d == '1' || d == 'a') return SQMOD_SKIN_DIAZ_GUY_A;
                    else if (d == '2' || d == 'b') return SQMOD_SKIN_DIAZ_GUY_B;
            }
        // [F]BI, [F]ireman, [F]ood lady, [F]rench guy
        case 'f':
            switch (b)
            {
                // [FB]I
                case 'b': return SQMOD_SKIN_FBI;
                // [Fi]re[m]an
                case 'i':
                case 'm': return SQMOD_SKIN_FIREMAN;
                // [Fo]od [l]ady
                case 'o':
                case 'l': return SQMOD_SKIN_FOOD_LADY;
                // [Fr]ench [g]uy
                case 'r':
                case 'g': return SQMOD_SKIN_FRENCH_GUY;
            }
        // [G]arbageman (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)
        // [G]olf guy (#1|A)/(#2|B)/(#3|C)
        // [G]olf lady
        // [G]ranny (#1|A)/(#2|B)
        // [G]ym guy, [G]ym lady
        case 'g':
            // [Ga]rbage[m]an (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)
            if (b && (b == 'a' || b == 'm'))
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_GARBAGEMAN_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_GARBAGEMAN_B;
                    case '3':
                    case 'c': return SQMOD_SKIN_GARBAGEMAN_C;
                    case '4':
                    case 'd': return SQMOD_SKIN_GARBAGEMAN_D;
                    case '5':
                    case 'e': return SQMOD_SKIN_GARBAGEMAN_E;
                }
            }
            // [Go]lf [g]uy (#1|A)/(#2|B)/(#3|C)
            else if (b == 'o' && ((c == 'g') || (len > 3 && str[4] == 'g')))
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_GOLF_GUY_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_GOLF_GUY_B;
                    case '3':
                    case 'c': return SQMOD_SKIN_GOLF_GUY_C;
                }
            }
            // [Go]lf [l]ady
            else if (b == 'o' && ((c == 'l') || (len > 3 && str[4] == 'l')))
                return SQMOD_SKIN_GOLF_LADY;
            // [Gr]anny (#1|A)/(#2|B)
            else if (b == 'r')
            {
                if (d == '1' || d == 'a') return SQMOD_SKIN_GRANNY_A;
                else if (d == '2' || d == 'b') return SQMOD_SKIN_GRANNY_B;
            }
            // [Gy]m [g]uy
            else if (b && (b == 'g' || (b == 'y' && len > 2 && str[3] == 'g')))
                return SQMOD_SKIN_GYM_GUY;
            // [Gy]m [l]ady
            else if (b && (b == 'l' || (b == 'y' && len > 2 && str[3] == 'l')))
                return SQMOD_SKIN_GYM_LADY;
        // [H]atian (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)
        // [H]ilary, [H]ilary (Robber), [H]ood lady
        case 'h':
            // [H]atian (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)
            if (b == 'a')
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_HATIAN_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_HATIAN_B;
                    case '3':
                    case 'c': return SQMOD_SKIN_HATIAN_C;
                    case '4':
                    case 'd': return SQMOD_SKIN_HATIAN_D;
                    case '5':
                    case 'e': return SQMOD_SKIN_HATIAN_E;
                }
            }
            // [Hi]lary ([R]obbe[r])
            else if (b && (b == 'i' || b == 'r') && d == 'r') return SQMOD_SKIN_HILARY_ROBBER;
            // [Hi]lary
            else if (b == 'i') return SQMOD_SKIN_HILARY;
            // [Ho]od [l]ady
            if (b && (b == 'o' || b == 'l')) return SQMOD_SKIN_HOOD_LADY;
        // [K]en Rosenburg
        case 'k':
            return SQMOD_SKIN_KEN_ROSENBURG;
        // [L]ance (#1|A)/(#1|B)
        // [L]ance (Cop)
        // [L]awyer
        // [L]ove Fist (#1|A)/(#2|B)/(#3|C)/(#3|D)
        case 'l':
            //[Lan]ce ([C]o[p])
            if ((b == 'a') && (c == 'n') && ((len > 4 && str[5] == 'c') || d == 'p'))
                return SQMOD_SKIN_LANCE_COP;
            else if (b && (b == 'c' ||  (b == 'a' && (c == 'n'))))
                return SQMOD_SKIN_LANCE_COP;
            // [La]nce (#1|A)/(#1|B)
            else if (b == 'a' && c == 'n')
            {
                if (d == '1' || d == 'a') return SQMOD_SKIN_LANCE_A;
                else if (d == '2' || d == 'b') return SQMOD_SKIN_LANCE_B;
            }
            // [Law]yer
            else if (b && (b == 'w' || (b == 'a' && c == 'w'))) return SQMOD_SKIN_LAWYER;
            // [Lo]ve [F]ist (#1|A)/(#2|B)/(#3|C)/(#3|D)
            else if (b && (b == 'o' || b == 'f'))
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_LOVE_FIST_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_LOVE_FIST_B;
                    case '3':
                    case 'c': return SQMOD_SKIN_LOVE_FIST_C;
                    case 'd': return SQMOD_SKIN_LOVE_FIST_D;
                }
            }
        // [M]ercades
        case 'm':
            if (d == 'b') return SQMOD_SKIN_MERCADES_B;
            else return SQMOD_SKIN_MERCADES_A;
        // [O]ffice lady (#1|A)/(#2|B)
        case 'o':
            if (d == '1' || d == 'a') return SQMOD_SKIN_OFFICE_LADY_A;
            else if (d == '2' || d == 'b') return SQMOD_SKIN_OFFICE_LADY_B;
        // [P]aramedic, [P]hil,  [P]hil (One arm), [P]hil (Robber)
        // [P]imp, [P]izzaman
        // [P]rostitute (#1|A)/(#2|B)/(#2|C)/(#2|D)/(#3|D)/(#4|D)
        // [P]unk (#1|A)/(#2|B)/(#3|C)
        case 'p':
            // [Pa]ramedic
            if (b == 'a') return SQMOD_SKIN_PARAMEDIC;
            // [Ph]il (One arm), [Ph]il (Robber)
            else if (b == 'h')
            {
                // [Ph]il ([O]ne ar[m])
                if (b == 'o' || (c == 'o') || (len > 3 && str[4] == 'o') || d == 'm')
                    return SQMOD_SKIN_PHIL_ONE_ARM;
                // [Ph]il ([R]obbe[r])
                else if (c && (c == 'r' || d == 'r' || (len > 3 && str[4] == 'r')))
                    return SQMOD_SKIN_PHIL_ROBBER;
                // [Phi]l
                else if (c == 'i') return SQMOD_SKIN_PHIL;
            }
            // [Pim][p]
            else if (b == 'i' && ((c == 'm') || d == 'p')) return SQMOD_SKIN_PIMP;
            // [Piz]zama[n]
            else if (b == 'i' && ((c == 'z') || d == 'n')) return SQMOD_SKIN_PIZZAMAN;
            // [Pr]ostitute (#1|A)/(#2|B)/(#2|C)/(#2|D)/(#3|D)/(#4|D)
            else if (b == 'r')
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_PROSTITUTE_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_PROSTITUTE_B;
                    case 'c': return SQMOD_SKIN_PROSTITUTE_C;
                    case 'd': return SQMOD_SKIN_PROSTITUTE_D;
                    case '3':
                    case 'e': return SQMOD_SKIN_PROSTITUTE_E;
                    case '4':
                    case 'f': return SQMOD_SKIN_PROSTITUTE_F;
                }
            }
            // [Pu]nk (#1|A)/(#2|B)/(#3|C)
            else if (b == 'u')
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_PUNK_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_PUNK_B;
                    case '3':
                    case 'c': return SQMOD_SKIN_PUNK_C;
                }
            }
        // [R]ich guy, [R]ockstar guy
        case 'r':
            // [Ri]ch guy
            if (b == 'i') return SQMOD_SKIN_RICH_GUY;
            // [Ro]ckstar guy
            else if (b == 'o') return SQMOD_SKIN_ROCKSTAR_GUY;
        // [S]ailor (#1|A)/(#2|B)/(#3|C)
        // [S]hark (#1|A)/(#2|B)
        // [S]hopper (#1|A)/(#2|B)
        // [S]kate guy, [S]kate lady, [S]onny
        // [S]onny guy (#1|A)/(#2|B)/(#3|C)
        // [S]pandEx (#1|A)/(#2|B)
        // [S]panish guy
        // [S]panish lady (#1|A)/(#2|B)/(#3|C)/(#4|D)
        // [S]tore clerk
        // [S]tripper (#1|A)/(#2|B)/(#3|C)
        // [S]wat
        case 's':
            // [Sa]ilor (#1|A)/(#2|B)/(#3|C)
            if (b == 'a')
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_SAILOR_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_SAILOR_B;
                    case '3':
                    case 'c': return SQMOD_SKIN_SAILOR_C;
                }
            }
            // [S]hark (#1|A)/(#2|B)
            else if (b == 'h' && c == 'a')
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_SHARK_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_SHARK_B;
                }
            }
            // [S]hopper (#1|A)/(#2|B)
            else if (b == 'h' && c == 'o')
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_SHOPPER_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_SHOPPER_B;
                }
            }
            // [Sk]ate [g]uy
            else if (b == 'k' && ((c == 'g') || (len > 4 && str[5] == 'g')))
                return SQMOD_SKIN_SKATE_GUY;
            // [Sk]ate [l]ady
            else if (b == 'k' && ((c == 'l') || (len > 4 && str[5] == 'l')))
                return SQMOD_SKIN_SKATE_LADY;
            // [So]nny
            // [So]nny guy (#1|A)/(#2|B)/(#3|C)
            else if (b == 'o')
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_SONNY_GUY_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_SONNY_GUY_B;
                    case '3':
                    case 'c': return SQMOD_SKIN_SONNY_GUY_C;
                }
            }
            else if (b == 'g')
            {
                                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_SONNY_GUY_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_SONNY_GUY_B;
                    case '3':
                    case 'c': return SQMOD_SKIN_SONNY_GUY_C;
                }
            }
            // [Sp]andE[x] (#1|A)/(#2|B)
            else if (b == 'p' && ((c == 'x') || (len > 5 && str[6] == 'x')))
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_SPANDEX_GUY_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_SPANDEX_GUY_B;
                }
            }
            // [Sp]anish [g]uy
            else if (b == 'p' && ((c == 'g') || (len > 6 && str[7] == 'g')))
                return SQMOD_SKIN_SPANISH_GUY;
            // [Sp]anish [l]ady (#1|A)/(#2|B)/(#3|C)/(#4|D)
            else if (b == 'p' && ((c == 'l') || (len > 6 && str[7] == 'l')))
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_SPANISH_LADY_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_SPANISH_LADY_B;
                    case '3':
                    case 'c': return SQMOD_SKIN_SPANISH_LADY_C;
                    case '4':
                    case 'd': return SQMOD_SKIN_SPANISH_LADY_D;
                }
            }
            // [Sto]re clerk
            else if ((b == 't') && (c == 'o')) return SQMOD_SKIN_STORE_CLERK;
            // [Str]ipper (#1|A)/(#2|B)/(#3|C)
            else if ((b == 't') && (c == 'r'))
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_STRIPPER_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_STRIPPER_B;
                    case '3':
                    case 'c': return SQMOD_SKIN_STRIPPER_C;
                }
            }
            // [Sw]at
            else if (b == 'w') return SQMOD_SKIN_SWAT;
        // [T]axi driver (#1|A)/(#1|B)/(#2|C)/(#2|D)
        // [T]hug (#1|A)/(#2|B)
        // [T]ommy Vercetti
        // [T]ourist (#1|A)/(#2|B)
        // [T]ranny
        case 't':
            switch (b)
            {
                // [Ta]xi driver (#1|A)/(#1|B)/(#2|C)/(#2|D)
                case 'a':
                    switch (d)
                    {
                        case '1':
                        case 'a': return SQMOD_SKIN_TAXI_DRIVER_A;
                        case '2':
                        case 'b': return SQMOD_SKIN_TAXI_DRIVER_B;
                        case 'c': return SQMOD_SKIN_TAXI_DRIVER_C;
                        case 'd': return SQMOD_SKIN_TAXI_DRIVER_D;
                    }
                // [Th]ug (#1|A)/(#2|B)
                case 'h':
                    switch (d)
                    {
                        case '1':
                        case 'a': return SQMOD_SKIN_THUG_A;
                        case '5':
                        case 'b': return SQMOD_SKIN_THUG_B;
                    }
                // [To]mmy [V]ercetti
                // [To]urist (#1|A)/(#2|B)
                case 'v': return SQMOD_SKIN_TOMMY_VERCETTI;
                case 'o':
                    if (c == 'm') return SQMOD_SKIN_TOMMY_VERCETTI;
                    else if (c == 'u' && (d == '1' || d == 'a')) return SQMOD_SKIN_TOURIST_A;
                    else if (c == 'u' && (d == '2' || d == 'b')) return SQMOD_SKIN_TOURIST_B;
                case 'r': return SQMOD_SKIN_TRANNY;
            }
        // [U]ndercover cop (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)/(#6|F)
        case 'u':
            switch (d)
            {
                case '1':
                case 'a': return SQMOD_SKIN_UNDERCOVER_COP_A;
                case '2':
                case 'b': return SQMOD_SKIN_UNDERCOVER_COP_B;
                case '3':
                case 'c': return SQMOD_SKIN_UNDERCOVER_COP_C;
                case '4':
                case 'd': return SQMOD_SKIN_UNDERCOVER_COP_D;
                case '5':
                case 'e': return SQMOD_SKIN_UNDERCOVER_COP_E;
                case '6':
                case 'f': return SQMOD_SKIN_UNDERCOVER_COP_F;
            }
        // [V]ercetti guy (#1|A)/(#2|B)
        case 'v':
            switch (d)
            {
                case '1':
                case 'a': return SQMOD_SKIN_VERCETTI_GUY_A;
                case '2':
                case 'b': return SQMOD_SKIN_VERCETTI_GUY_B;
            }
        // [W]aitress (#1|A)/(#2|B)
        case 'w':
            switch (d)
            {
                case '1':
                case 'a': return SQMOD_SKIN_WAITRESS_A;
                case '2':
                case 'b': return SQMOD_SKIN_WAITRESS_B;
            }
        // Default to unknown
        default: return SQMOD_UNKNOWN;
    }
}

// ------------------------------------------------------------------------------------------------
bool IsSkinValid(Int32 id)
{
    CSStr name = GetSkinName(id);
    return (name && *name != '\0');
}

} // Namespace:: SqMod
