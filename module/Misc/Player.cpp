// ------------------------------------------------------------------------------------------------
#include "Misc/Player.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
#include <algorithm>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
static String CS_Skin_Names[] = { // NOLINT(cert-err58-cpp)
    /* 0   */ "Tommy Vercetti",           /* 1   */ "Cop",
    /* 2   */ "SWAT",                     /* 3   */ "FBI",
    /* 4   */ "Army",                     /* 5   */ "Paramedic",
    /* 6   */ "Firefighter",              /* 7   */ "Golf Guy #1",
    /* 8   */ "",                         /* 9   */ "Bum Lady #1",
    /* 10  */ "Bum Lady #2",              /* 11  */ "Punk #1",
    /* 12  */ "Lawyer",                   /* 13  */ "Spanish Lady #1",
    /* 14  */ "Spanish Lady #2",          /* 15  */ "Cool Guy #1",
    /* 16  */ "Arabic Guy",               /* 17  */ "Beach Lady #1",
    /* 18  */ "Beach Lady #2",            /* 19  */ "Beach Guy #1",
    /* 20  */ "Beach Guy #2",             /* 21  */ "Office Lady #1",
    /* 22  */ "Waitress #1",              /* 23  */ "Food Lady",
    /* 24  */ "Prostitute #1",            /* 25  */ "Bum Lady #3",
    /* 26  */ "Bum Guy #1",               /* 27  */ "Garbageman #1",
    /* 28  */ "Taxi Driver #1",           /* 29  */ "Haitian #1",
    /* 30  */ "Criminal #1",              /* 31  */ "Hood Lady",
    /* 32  */ "Granny #1",                /* 33  */ "Businessman #1",
    /* 34  */ "Church Guy",               /* 35  */ "Club Lady",
    /* 36  */ "Church Lady",              /* 37  */ "Pimp",
    /* 38  */ "Beach Lady #3",            /* 39  */ "Beach Guy #3",
    /* 40  */ "Beach Lady #4",            /* 41  */ "Beach Guy #4",
    /* 42  */ "Businessman #2",           /* 43  */ "Prostitute #2",
    /* 44  */ "Bum Lady #4",              /* 45  */ "Bum Guy #2",
    /* 46  */ "Haitian #2",               /* 47  */ "Construction Worker #1",
    /* 48  */ "Punk #2",                  /* 49  */ "Prostitute #3",
    /* 50  */ "Granny #2",                /* 51  */ "Punk #3",
    /* 52  */ "Businessman #3",           /* 53  */ "Spanish Lady #3",
    /* 54  */ "Spanish Lady #4",          /* 55  */ "Cool Guy #2",
    /* 56  */ "Businessman #4",           /* 57  */ "Beach Lady #5",
    /* 58  */ "Beach Guy #5",             /* 59  */ "Beach Lady #6",
    /* 60  */ "Beach Guy #6",             /* 61  */ "Construction Worker #2",
    /* 62  */ "Golf Guy #2",              /* 63  */ "Golf Lady",
    /* 64  */ "Golf Guy #3",              /* 65  */ "Beach Lady #7",
    /* 66  */ "Beach Guy #7",             /* 67  */ "Office Lady #2",
    /* 68  */ "Businessman #5",           /* 69  */ "Businessman #6",
    /* 70  */ "Prostitute #2",            /* 71  */ "Bum Lady #4",
    /* 72  */ "Bum Guy #3",               /* 73  */ "Spanish Guy",
    /* 74  */ "Taxi Driver #2",           /* 75  */ "Gym Lady",
    /* 76  */ "Gym Guy",                  /* 77  */ "Skate Lady",
    /* 78  */ "Skate Guy",                /* 79  */ "Shopper #1",
    /* 80  */ "Shopper #2",               /* 81  */ "Tourist #1",
    /* 82  */ "Tourist #2",               /* 83  */ "Cuban #1",
    /* 84  */ "Cuban #2",                 /* 85  */ "Haitian #3",
    /* 86  */ "Haitian #4",               /* 87  */ "Shark #1",
    /* 88  */ "Shark #2",                 /* 89  */ "Diaz Guy #1",
    /* 90  */ "Diaz Guy #2",              /* 91  */ "DBP Security #1",
    /* 92  */ "DBP Security #2",          /* 93  */ "Biker #1",
    /* 94  */ "Biker #2",                 /* 95  */ "Vercetti Guy #1",
    /* 96  */ "Vercetti Guy #2",          /* 97  */ "Undercover Cop #1",
    /* 98  */ "Undercover Cop #2",        /* 99  */ "Undercover Cop #3",
    /* 100 */ "Undercover Cop #4",        /* 101 */ "Undercover Cop #5",
    /* 102 */ "Undercover Cop #6",        /* 103 */ "Rich Guy",
    /* 104 */ "Cool Guy #3",              /* 105 */ "Prostitute #3",
    /* 106 */ "Prostitute #4",            /* 107 */ "Love Fist #1",
    /* 108 */ "Ken Rosenburg",            /* 109 */ "Candy Suxx",
    /* 110 */ "Hilary",                   /* 111 */ "Love Fist #2",
    /* 112 */ "Phil",                     /* 113 */ "Rockstar Guy",
    /* 114 */ "Sonny",                    /* 115 */ "Lance",
    /* 116 */ "Mercedes",                 /* 117 */ "Love Fist #3",
    /* 118 */ "Alex Shrub",               /* 119 */ "Lance (Cop)",
    /* 120 */ "Lance",                    /* 121 */ "Cortez",
    /* 122 */ "Love Fist #4",             /* 123 */ "Columbian Guy #1",
    /* 124 */ "Hilary (Robber)",          /* 125 */ "Mercedes",
    /* 126 */ "Cam",                      /* 127 */ "Cam (Robber)",
    /* 128 */ "Phil (One Arm)",           /* 129 */ "Phil (Robber)",
    /* 130 */ "Cool Guy #4",              /* 131 */ "Pizza Man",
    /* 132 */ "Taxi Driver #1",           /* 133 */ "Taxi Driver #2",
    /* 134 */ "Sailor #1",                /* 135 */ "Sailor #2",
    /* 136 */ "Sailor #3",                /* 137 */ "Chef",
    /* 138 */ "Criminal #2",              /* 139 */ "French Guy",
    /* 140 */ "Garbageman #2",            /* 141 */ "Haitian #5",
    /* 142 */ "Waitress #2",              /* 143 */ "Sonny Guy #1",
    /* 144 */ "Sonny Guy #2",             /* 145 */ "Sonny Guy #3",
    /* 146 */ "Columbian Guy #2",         /* 147 */ "Haitian #6",
    /* 148 */ "Beach Guy #8",             /* 149 */ "Garbageman #3",
    /* 150 */ "Garbageman #4",            /* 151 */ "Garbageman #5",
    /* 152 */ "Tranny",                   /* 153 */ "Thug #5",
    /* 154 */ "SpandEx Guy #1",           /* 155 */ "SpandEx Guy #2",
    /* 156 */ "Stripper #1",              /* 157 */ "Stripper #2",
    /* 158 */ "Stripper #3",              /* 159 */ "Store Clerk",
    /* 160 */ ""
};

// ------------------------------------------------------------------------------------------------
const char * GetSkinName(uint32_t id)
{
    return (id > 159) ? _SC("") : CS_Skin_Names[id].c_str();
}

// ------------------------------------------------------------------------------------------------
void SetSkinName(uint32_t id, StackStrF & name)
{
    if (id <= 159)
    {
        CS_Skin_Names[id].assign(name.mPtr);
    }
}

// ------------------------------------------------------------------------------------------------
int32_t GetSkinID(StackStrF & name)
{
    // Clone the string into an editable version
    String str(name.mPtr, static_cast< size_t >(name.mLen));
    // Strip non-alphanumeric characters from the name
    str.erase(std::remove_if(str.begin(), str.end(), [](char c) -> bool { return std::isalnum(c) == 0; }), str.end());
    // Convert the string to lowercase
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    // See if we still have a valid name after the cleanup
    if(str.empty())
    {
        return SQMOD_UNKNOWN;
    }
    // Grab the actual length of the string
    const uint32_t len = ConvTo< uint32_t >::From(str.length());
    // Get the most significant characters used to identify a skin
    CharT a = str[0], b = 0, c = 0, d = str[len-1];
    // Look for deeper specifiers
    if (len > 2)
    {
        b = str[1];
        c = str[2];
    }
    else if (len > 1)
    {
        b = str[1];
    }
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
                default: break;
            }
        // [B]each guy (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)/(#6|F)/(#7|G)/(#8|H)
        // [B]each lady (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)/(#6|F)/(#7|G)
        // [B]iker (#1|A)/(#2|B)
        // [B]um guy (#1|A)/(#2|B)/(#3|C)
        // [B]um lady (#1|A)/(#2|B)/(#2|C)/(#3|D)/(#4|E)
        // [B]usiness man (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)/(#6|F)
        // fall through
        case 'b':
            // [Be]ach [g]uy (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)/(#6|F)/(#7|G)/(#8|H)
            if (b == 'e' && (c == 'g' || (len > 5 && str[5] == 'g')))
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
                    default: break;
                }
            }
            // [Be]ach [l]ady (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)/(#6|F)/(#7|G)
            else if (b == 'e' && (c == 'l' || (len > 5 && str[5] == 'l')))
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
                    default: break;
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
                if (c == 'g' || (len > 3 && str[3] == 'g'))
                {
                    if (d == '1' || d == 'a') return SQMOD_SKIN_BUM_GUY_A;
                    else if (d == '2' || d == 'b') return SQMOD_SKIN_BUM_GUY_B;
                    else if (d == '3' || d == 'c') return SQMOD_SKIN_BUM_GUY_C;
                }
                // [Bum] [l]ady (#1|A)/(#2|B)/(#2|C)/(#3|D)/(#4|E)
                else if (c == 'l' || (len > 3 && str[3] == 'l'))
                {
                    if (d == '1' || d == 'a') return SQMOD_SKIN_BUM_LADY_A;
                    else if (d == '2' || d == 'b') return SQMOD_SKIN_BUM_LADY_B;
                    else if (d == '2' || d == 'c') return SQMOD_SKIN_BUM_LADY_C;
                    else if (d == '3' || d == 'd') return SQMOD_SKIN_BUM_LADY_D;
                    else if (d == '4' || d == 'e') return SQMOD_SKIN_BUM_LADY_E;
                }
            }
            // [Bus]iness [m]an (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)/(#6|F)
            else if (b == 'u' && (c == 's' || (len > 9 && str[9] == 'm')))
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
                    default: break;
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
        // fall through
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
                else if (c && ((c == 'u' && len > 6 && str[6] == 'g') || (c == 'g')))
                    return SQMOD_SKIN_CHURCH_GUY;
                // [Chu]rch [l]ady
                else if (c && ((c == 'u' && len > 6 && str[6] == 'l') || (c == 'l')))
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
                if (c && ((c == 'l' && len > 9 && str[9] == 'g') || (c == 'g')))
                {
                    if (d == '1' || d == 'a') return SQMOD_SKIN_COLUMBIAN_GUY_A;
                    else if (d == '2' || d == 'b') return SQMOD_SKIN_COLUMBIAN_GUY_B;
                }
                // [Con]struction [w]orker (#1|A)/(#2|B)
                else if (c && (c == 'n' || (len > 12 && str[12] == 'g')))
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
                        default: break;
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
        // fall through
        case 'd':
            switch (b)
            {
                // [DB]P [s]ecurity (#1|A)/(#2|B)
                case 'b':
                case 's':
                    if (d == '1' || d == 'a') return SQMOD_SKIN_DBP_SECURITY_A;
                    else if (d == '2' || d == 'b') return SQMOD_SKIN_DBP_SECURITY_B;
                // [Di]a[z] [g]uy (#1|A)/(#2|B)
                // fall through
                case 'i':
                case 'z':
                case 'g':
                    if (d == '1' || d == 'a') return SQMOD_SKIN_DIAZ_GUY_A;
                    else if (d == '2' || d == 'b') return SQMOD_SKIN_DIAZ_GUY_B;
                default: break;
            }
        // [F]BI, [F]ireman, [F]ood lady, [F]rench guy
        // fall through
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
                default: break;
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
                    default: break;
                }
            }
            // [Go]lf [g]uy (#1|A)/(#2|B)/(#3|C)
            else if (b == 'o' && ((c == 'g') || (len > 4 && str[4] == 'g')))
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_GOLF_GUY_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_GOLF_GUY_B;
                    case '3':
                    case 'c': return SQMOD_SKIN_GOLF_GUY_C;
                    default: break;
                }
            }
            // [Go]lf [l]ady
            else if (b == 'o' && ((c == 'l') || (len > 4 && str[4] == 'l')))
                return SQMOD_SKIN_GOLF_LADY;
            // [Gr]anny (#1|A)/(#2|B)
            else if (b == 'r')
            {
                if (d == '1' || d == 'a') return SQMOD_SKIN_GRANNY_A;
                else if (d == '2' || d == 'b') return SQMOD_SKIN_GRANNY_B;
            }
            // [Gy]m [g]uy
            else if (b && (b == 'g' || (b == 'y' && len > 3 && str[3] == 'g')))
                return SQMOD_SKIN_GYM_GUY;
            // [Gy]m [l]ady
            else if (b && (b == 'l' || (b == 'y' && len > 3 && str[3] == 'l')))
                return SQMOD_SKIN_GYM_LADY;
        // [H]atian (#1|A)/(#2|B)/(#3|C)/(#4|D)/(#5|E)
        // [H]ilary, [H]ilary (Robber), [H]ood lady
        // fall through
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
                    default: break;
                }
            }
            // [Hi]lary ([R]obbe[r])
            else if (b && (b == 'i' || b == 'r') && d == 'r') return SQMOD_SKIN_HILARY_ROBBER;
            // [Hi]lary
            else if (b == 'i') return SQMOD_SKIN_HILARY;
            // [Ho]od [l]ady
            if (b && (b == 'o' || b == 'l')) return SQMOD_SKIN_HOOD_LADY;
        // [K]en Rosenburg
        // fall through
        case 'k':
            return SQMOD_SKIN_KEN_ROSENBURG;
        // [L]ance (#1|A)/(#1|B)
        // [L]ance (Cop)
        // [L]awyer
        // [L]ove Fist (#1|A)/(#2|B)/(#3|C)/(#3|D)
        case 'l':
            //[Lan]ce ([C]o[p])
            if ((b == 'a') && (c == 'n') && ((len > 5 && str[5] == 'c') || d == 'p'))
                return SQMOD_SKIN_LANCE_COP; // NOLINT(bugprone-branch-clone)
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
                    default: break;
                }
            }
        // [M]ercades
        // fall through
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
        // fall through
        case 'p':
            // [Pa]ramedic
            if (b == 'a') return SQMOD_SKIN_PARAMEDIC;
            // [Ph]il (One arm), [Ph]il (Robber)
            else if (b == 'h')
            {
                // [Ph]il ([O]ne ar[m])
                if (b == 'o' || (c == 'o') || (len > 4 && str[4] == 'o') || d == 'm')
                    return SQMOD_SKIN_PHIL_ONE_ARM;
                // [Ph]il ([R]obbe[r])
                else if (c && (c == 'r' || d == 'r' || (len > 4 && str[4] == 'r')))
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
                    default: break;
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
                    default: break;
                }
            }
        // [R]ich guy, [R]ockstar guy
        // fall through
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
        // fall through
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
                    default: break;
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
                    default: break;
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
                    default: break;
                }
            }
            // [Sk]ate [g]uy
            else if (b == 'k' && ((c == 'g') || (len > 5 && str[5] == 'g')))
                return SQMOD_SKIN_SKATE_GUY;
            // [Sk]ate [l]ady
            else if (b == 'k' && ((c == 'l') || (len > 5 && str[5] == 'l')))
                return SQMOD_SKIN_SKATE_LADY;
            // [So]nny
            // [So]nny guy (#1|A)/(#2|B)/(#3|C)
            else if (b == 'o')
            { // NOLINT(bugprone-branch-clone)
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_SONNY_GUY_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_SONNY_GUY_B;
                    case '3':
                    case 'c': return SQMOD_SKIN_SONNY_GUY_C;
                    default: break;
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
                    default: break;
                }
            }
            // [Sp]andE[x] (#1|A)/(#2|B)
            else if (b == 'p' && ((c == 'x') || (len > 6 && str[6] == 'x')))
            {
                switch (d)
                {
                    case '1':
                    case 'a': return SQMOD_SKIN_SPANDEX_GUY_A;
                    case '2':
                    case 'b': return SQMOD_SKIN_SPANDEX_GUY_B;
                    default: break;
                }
            }
            // [Sp]anish [g]uy
            else if (b == 'p' && ((c == 'g') || (len > 7 && str[7] == 'g')))
                return SQMOD_SKIN_SPANISH_GUY;
            // [Sp]anish [l]ady (#1|A)/(#2|B)/(#3|C)/(#4|D)
            else if (b == 'p' && ((c == 'l') || (len > 7 && str[7] == 'l')))
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
                    default: break;
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
                    default: break;
                }
            }
            // [Sw]at
            else if (b == 'w') return SQMOD_SKIN_SWAT;
        // [T]axi driver (#1|A)/(#1|B)/(#2|C)/(#2|D)
        // [T]hug (#1|A)/(#2|B)
        // [T]ommy Vercetti
        // [T]ourist (#1|A)/(#2|B)
        // [T]ranny
        // fall through
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
                        default: break;
                    }
                // [Th]ug (#1|A)/(#2|B)
                case 'h':
                    switch (d)
                    {
                        case '1':
                        case 'a': return SQMOD_SKIN_THUG_A;
                        case '5':
                        case 'b': return SQMOD_SKIN_THUG_B;
                        default: break;
                    }
                // [To]mmy [V]ercetti
                // [To]urist (#1|A)/(#2|B)
                case 'v': return SQMOD_SKIN_TOMMY_VERCETTI;
                case 'o':
                    if (c == 'm') return SQMOD_SKIN_TOMMY_VERCETTI;
                    else if (c == 'u' && (d == '1' || d == 'a')) return SQMOD_SKIN_TOURIST_A;
                    else if (c == 'u' && (d == '2' || d == 'b')) return SQMOD_SKIN_TOURIST_B;
                // fall through
                case 'r': return SQMOD_SKIN_TRANNY;
                default: break;
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
                default: break;
            }
        // [V]ercetti guy (#1|A)/(#2|B)
        case 'v':
            switch (d)
            {
                case '1':
                case 'a': return SQMOD_SKIN_VERCETTI_GUY_A;
                case '2':
                case 'b': return SQMOD_SKIN_VERCETTI_GUY_B;
                default: break;
            }
        // [W]aitress (#1|A)/(#2|B)
        case 'w':
            switch (d)
            {
                case '1':
                case 'a': return SQMOD_SKIN_WAITRESS_A;
                case '2':
                case 'b': return SQMOD_SKIN_WAITRESS_B;
                default: break;
            }
        // Default to unknown
        default: return SQMOD_UNKNOWN;
    }
}

// ------------------------------------------------------------------------------------------------
bool IsSkinValid(int32_t id)
{
    const SQChar * name = GetSkinName(static_cast<uint32_t>(id));
    return (name && *name != '\0');
}

} // Namespace:: SqMod
