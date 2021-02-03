// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"
#include "Base/Color3.hpp"
#include "Core/Utility.hpp"
#include "Library/String.hpp"
#include "Library/Numeric/Random.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
static const Color3 SQ_Color_List[] =
{
    Color3(240, 248, 255),
    Color3(250, 235, 215),
    Color3(0, 255, 255),
    Color3(127, 255, 212),
    Color3(240, 255, 255),
    Color3(245, 245, 220),
    Color3(255, 228, 196),
    Color3(0, 0, 0),
    Color3(255, 235, 205),
    Color3(0, 0, 255),
    Color3(138, 43, 226),
    Color3(165, 42, 42),
    Color3(222, 184, 135),
    Color3(95, 158, 160),
    Color3(127, 255, 0),
    Color3(210, 105, 30),
    Color3(255, 127, 80),
    Color3(100, 149, 237),
    Color3(255, 248, 220),
    Color3(220, 20, 60),
    Color3(0, 255, 255),
    Color3(0, 0, 139),
    Color3(0, 139, 139),
    Color3(184, 134, 11),
    Color3(169, 169, 169),
    Color3(0, 100, 0),
    Color3(189, 183, 107),
    Color3(139, 0, 139),
    Color3(85, 107, 47),
    Color3(255, 140, 0),
    Color3(153, 50, 204),
    Color3(139, 0, 0),
    Color3(233, 150, 122),
    Color3(143, 188, 143),
    Color3(72, 61, 139),
    Color3(47, 79, 79),
    Color3(0, 206, 209),
    Color3(148, 0, 211),
    Color3(255, 20, 147),
    Color3(0, 191, 255),
    Color3(105, 105, 105),
    Color3(30, 144, 255),
    Color3(178, 34, 34),
    Color3(255, 250, 240),
    Color3(34, 139, 34),
    Color3(255, 0, 255),
    Color3(220, 220, 220),
    Color3(248, 248, 255),
    Color3(255, 215, 0),
    Color3(218, 165, 32),
    Color3(128, 128, 128),
    Color3(0, 128, 0),
    Color3(173, 255, 47),
    Color3(240, 255, 240),
    Color3(255, 105, 180),
    Color3(205, 92, 92),
    Color3(75, 0, 130),
    Color3(255, 255, 240),
    Color3(240, 230, 140),
    Color3(230, 230, 250),
    Color3(255, 240, 245),
    Color3(124, 252, 0),
    Color3(255, 250, 205),
    Color3(173, 216, 230),
    Color3(240, 128, 128),
    Color3(224, 255, 255),
    Color3(250, 250, 210),
    Color3(211, 211, 211),
    Color3(144, 238, 144),
    Color3(255, 182, 193),
    Color3(255, 160, 122),
    Color3(32, 178, 170),
    Color3(135, 206, 250),
    Color3(119, 136, 153),
    Color3(176, 196, 222),
    Color3(255, 255, 224),
    Color3(0, 255, 0),
    Color3(50, 205, 50),
    Color3(250, 240, 230),
    Color3(255, 0, 255),
    Color3(128, 0, 0),
    Color3(102, 205, 170),
    Color3(0, 0, 205),
    Color3(186, 85, 211),
    Color3(147, 112, 219),
    Color3(60, 179, 113),
    Color3(123, 104, 238),
    Color3(0, 250, 154),
    Color3(72, 209, 204),
    Color3(199, 21, 133),
    Color3(25, 25, 112),
    Color3(245, 255, 250),
    Color3(255, 228, 225),
    Color3(255, 228, 181),
    Color3(255, 222, 173),
    Color3(0, 0, 128),
    Color3(253, 245, 230),
    Color3(128, 128, 0),
    Color3(107, 142, 35),
    Color3(255, 165, 0),
    Color3(255, 69, 0),
    Color3(218, 112, 214),
    Color3(238, 232, 170),
    Color3(152, 251, 152),
    Color3(175, 238, 238),
    Color3(219, 112, 147),
    Color3(255, 239, 213),
    Color3(255, 218, 185),
    Color3(205, 133, 63),
    Color3(255, 192, 203),
    Color3(221, 160, 221),
    Color3(176, 224, 230),
    Color3(128, 0, 128),
    Color3(255, 0, 0),
    Color3(188, 143, 143),
    Color3(65, 105, 225),
    Color3(139, 69, 19),
    Color3(250, 128, 114),
    Color3(244, 164, 96),
    Color3(46, 139, 87),
    Color3(255, 245, 238),
    Color3(160, 82, 45),
    Color3(192, 192, 192),
    Color3(135, 206, 235),
    Color3(106, 90, 205),
    Color3(112, 128, 144),
    Color3(255, 250, 250),
    Color3(0, 255, 127),
    Color3(70, 130, 180),
    Color3(210, 180, 140),
    Color3(0, 128, 128),
    Color3(216, 191, 216),
    Color3(255, 99, 71),
    Color3(64, 224, 208),
    Color3(238, 130, 238),
    Color3(245, 222, 179),
    Color3(255, 255, 255),
    Color3(245, 245, 245),
    Color3(255, 255, 0),
    Color3(154, 205, 50)
};

// ------------------------------------------------------------------------------------------------
const Color3 & GetRandomColor()
{
    return SQ_Color_List[GetRandomUint8(0, (sizeof(SQ_Color_List) / sizeof(Color3)) - 1)];
}

// ------------------------------------------------------------------------------------------------
Color3 GetColor(StackStrF & name)
{
    return name.mLen <= 0 ? Color3() : GetColorStr(name.mPtr);
}

// ------------------------------------------------------------------------------------------------
Color3 GetColorStr(const SQChar * name)
{
    // See if we actually have something to search for
    if(!name || *name == '\0')
    {
        return Color3::NIL; // Use default color
    }
    // Clone the string into an editable version
    const SQChar * str = StrJustAlphaNum(name);
    str = StrToLowercase(str);
    // See if we still have a valid name after the cleanup
    if(!str || *str == '\0')
    {
        return Color3::NIL; // Use default color
    }
    // Calculate the name length
    const size_t len = std::strlen(str);
    // Get the most significant characters used to identify a weapon
    SQChar a = str[0], b = 0, c = 0, d = str[len-1];
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
        // [A]liceBlue
        // [A]ntiqueWhite
        // [A]qua
        // [A]quamarine
        // [A]zure
        case 'a':
            switch (b)
            {
                // [Al]iceBlue
                case 'l': return {240, 248, 255};
                // [Aq]ua[m]arine
                case 'm': return {127, 255, 212};
                // [An]tiqueWhite
                case 'n': return {250, 235, 215};
                // [Aq]ua
                // [Aq]uamarine
                case 'q':
                    // [Aq]u[a]
                    if (d == 'a') return {0, 255, 255};
                    // [Aq]ua[m]arin[e]
                    else if (d == 'e' || (len > 4 && str[4] == 'm')) return {127, 255, 212};
                    // Default to blank
                    else return Color3::NIL;
                // [Az]ure
                case 'z': return {240, 255, 255};
                // Default to blank
                default: return Color3::NIL;
            }
        // [B]eige
        // [B]isque
        // [B]lack
        // [B]lanchedAlmond
        // [B]lue
        // [B]lueViolet
        // [B]rown
        // [B]urlyWood
        case 'b':
            switch (b)
            {
                // [B]lanched[A]lmond
                case 'a': return {255, 235, 205};
                // [Be]ige
                case 'e': return {245, 245, 220};
                // [Bi]sque
                case 'i': return {255, 228, 196};
                // [Bl]ack
                // [Bl]anchedAlmond
                // [Bl]ue
                // [Bl]ueViolet
                case 'l':
                    // [Bl]a[ck]
                    if (d == 'k' || d == 'c') return {0, 0, 0};
                    // [Bl]anched[A]lmon[d]
                    else if (d == 'd' || (len > 8 && str[8] == 'a')) return {255, 235, 205};
                    // [Bl]u[e]
                    else if (d == 'e') return {0, 0, 255};
                    // [Bl]ue[V]iole[t]
                    else if (d == 't' || (len > 4 && str[4] == 'v')) return {138, 43, 226};
                    // Default to blank
                    else return Color3::NIL;
                // [Br]own
                case 'r': return {165, 42, 42};
                // [Bu]rlyWood
                case 'u': return {222, 184, 135};
                // [B]lue[V]iolet
                case 'v': return {138, 43, 226};
                // Default to blank
                default: return Color3::NIL;
            }
        // [C]adetBlue
        // [C]hartreuse
        // [C]hocolate
        // [C]oral
        // [C]ornflowerBlue
        // [C]ornsilk
        // [C]rimson
        // [C]yan
        case 'c':
            switch (b)
            {
                // [Ca]detBlue
                case 'a': return {95, 158, 160};
                // [Ch]artreuse
                // [Ch]ocolate
                case 'h':
                    // [Ch]artreuse
                    if (c == 'a') return {127, 255, 0};
                    // [Ch]ocolate
                    else if (c == 'o') return {210, 105, 30};
                    // Default to blank
                    else return Color3::NIL;
                // [Co]ral
                // [Co]rnflowerBlue
                // [Co]rnsilk
                case 'o':
                    // [Co]ra[l]
                    if (d == 'l') return {255, 127, 80};
                    // [Co]rnflower[B]lu[e]
                    else if (d == 'e' || (len > 10 && str[10] == 'b')) return {100, 149, 237};
                    // [Co]rnsil[k]
                    else if (d == 'k') return {255, 248, 220};
                    // Default to blank
                    else return Color3::NIL;
                // [Cr]imson
                case 'r': return {220, 20, 60};
                // [Cy]an
                case 'y': return {0, 255, 255};
                // Default to blank
                default: return Color3::NIL;
            }
        // [D]arkBlue
        // [D]arkCyan
        // [D]arkGoldenRod
        // [D]arkGray
        // [D]arkGreen
        // [D]arkKhaki
        // [D]arkMagenta
        // [D]arkOliveGreen
        // [D]arkOrange
        // [D]arkOrchid
        // [D]arkRed
        // [D]arkSalmon
        // [D]arkSeaGreen
        // [D]arkSlateBlue
        // [D]arkSlateGray
        // [D]arkTurquoise
        // [D]arkViolet
        // [D]eepPink
        // [D]eepSkyBlue
        // [D]imGray
        // [D]odgerBlue
        case 'd':
            // [Di]mGray
            if (b == 'i' || b == 'g') return {105, 105, 105};
            // [Do]dgerBlue
            else if (b == 'o' || b == 'b') return {30, 144, 255};
            // [De]ep[P]in[k]
            else if (b == 'e' && (d == 'k' || (len > 4 && str[4] == 'p'))) return {255, 20, 147};
            // [De]ep[S]kyBlu[e]
            else if (b == 'e' && (d == 'e' || (len > 4 && str[4] == 's'))) return {0, 191, 255};
            // [Da]rkBlue
            // [Da]rkCyan
            // [Da]rkGoldenRod
            // [Da]rkGray
            // [Da]rkGreen
            // [Da]rkKhaki
            // [Da]rkMagenta
            // [Da]rkOliveGreen
            // [Da]rkOrange
            // [Da]rkOrchid
            // [Da]rkRed
            // [Da]rkSalmon
            // [Da]rkSeaGreen
            // [Da]rkSlateBlue
            // [Da]rkSlateGray
            // [Da]rkTurquoise
            // [Da]rkViolet
            else if (b == 'a') {
                // [Da]rk[B]lue
                if (c == 'b' || (len > 4 && str[4] == 'b')) return {0, 0, 139};
                // [Da]rk[C]yan
                else if (c == 'c' || (len > 4 && str[4] == 'c')) return {0, 139, 139};
                // [Da]rk[Go]ldenRo[d]
                else if ((len > 4 && str[4] == 'g') && (d == 'd' || d == 'o')) return {184, 134, 11};
                // [Da]rk[G]r[ay]
                else if ((len > 4 && str[4] == 'g') && (d == 'y' || d == 'a')) return {169, 169, 169};
                // [Da]rk[G]r[een]
                else if ((len > 4 && str[4] == 'g') && (d == 'n' || d == 'e')) return {0, 100, 0};
                // [Da]rk[K]hak[i]
                else if (d == 'i' || c == 'k' || (len > 4 && str[4] == 'k')) return {189, 183, 107};
                // [Da]rk[M]agent[a]
                else if (d == 'a' || c == 'm' || (len > 4 && str[4] == 'm')) return {139, 0, 139};
                // [Da]rk[O]liveGr[een]
                else if ((len > 4 && str[4] == 'o') && (d == 'n' || d == 'e')) return {85, 107, 47};
                // [Da]rk[O]r[a]ng[e]
                else if ((len > 4 && str[4] == 'o') && (d == 'e' || d == 'a')) return {255, 140, 0};
                // [Da]rk[O]r[c]hi[d]
                else if ((len > 4 && str[4] == 'o') && (d == 'd' || d == 'c')) return {153, 50, 204};
                // [Da]rk[R]ed
                else if (len > 4 && str[4] == 'r') return {139, 0, 0};
                // [Da]rk[Sa]lmon
                else if (len > 5 && str[4] == 's' && str[5] == 'a') return {233, 150, 122};
                // [Da]rk[Se]aGreen
                else if (len > 5 && str[4] == 's' && str[5] == 'e') return {143, 188, 143};
                // [Da]rk[S]lateBlu[e]
                else if ((len > 4 && str[4] == 's') && (d == 'e' || d == 'b')) return {72, 61, 139};
                // [Da]rk[S]lateGra[y]
                else if ((len > 4 && str[4] == 's') && (d == 'y' || d == 'g')) return {47, 79, 79};
                // [Da]rk[T]urquoise
                else if (c == 't' || (len > 4 && str[4] == 't')) return {0, 206, 209};
                // [Da]rk[V]iolet
                else if (c == 'v' || (len > 4 && str[4] == 'v')) return {148, 0, 211};
                // Default to blank
                else return Color3::NIL;
            // Default to blank
            } else return Color3::NIL;
        // [F]ireBrick
        // [F]loralWhite
        // [F]orestGreen
        // [F]uchsia
        case 'f':
            switch (b)
            {
                // [Fi]re[B]rick
                case 'i':
                case 'b': return {178, 34, 34};
                // [Fl]oral[W]hite
                case 'l':
                case 'w': return {255, 250, 240};
                // [Fo]rest[G]reen
                case 'o':
                case 'g': return {34, 139, 34};
                // [Fu]chsia
                case 'u': return {255, 0, 255};
                // Default to blank
                default: return Color3::NIL;
            }
        // [G]ainsboro
        // [G]hostWhite
        // [G]old
        // [G]oldenRod
        // [G]ray
        // [G]reen
        // [G]reenYellow
        case 'g':
            // [Ga]insboro
            if (b == 'a') return {220, 220, 220};
            // [Gh]ost[W]hite
            else if (b == 'h' || b == 'w') return {248, 248, 255};
            // [Go]ld[e]n[R]od
            else if (len > 4 && (str[4] == 'e' || str[4] == 'r')) return {218, 165, 32};
            // [Go]l[d]
            else if (b == 'o' && d == 'd') return {255, 215, 0};
            // [Gray]
            else if (b == 'r' && (d == 'y' || d == 'a')) return {128, 128, 128};
            // [Gr]een
            else if (b == 'r' && d == 'n') return {0, 128, 0};
            // [Gr]eenYellow
            else if (b == 'r' && (d == 'w' || (len > 5 && str[5] == 'y'))) return {173, 255, 47};
            // Default to blank
            else return Color3::NIL;
        // [H]oneyDew
        // [H]otPink
        case 'h':
            // [H]o[n]ey[D]e[w]
            if (d == 'w' || c == 'n' || (len > 5 && str[5] == 'd')) return {240, 255, 240};
            // [H]o[tP]in[k]
            else if (d == 'k' || c == 't' || (len > 3 && str[3] == 'p')) return {255, 105, 180};
            // Default to blank
            else return Color3::NIL;
        // [I]ndianRed
        // [I]ndigo
        // [I]vory
        case 'i':
            // [In]dian[R]e[d]
            if (b == 'n' && (d == 'd' || d == 'r')) return {205, 92, 92};
            // [In]di[go]
            else if (b == 'n' && (d == 'o' || d == 'g')) return {75, 0, 130};
            // [I]vory
            else if (b == 'v') return {255, 255, 240};
            // Default to blank
            else return Color3::NIL;
        // [K]haki
        case 'k': return {240, 230, 140};
        // [L]avender
        // [L]avenderBlush
        // [L]awnGreen
        // [L]emonChiffon
        // [L]ightBlue
        // [L]ightCoral
        // [L]ightCyan
        // [L]ightGoldenRodYellow
        // [L]ightGray
        // [L]ightGreen
        // [L]ightPink
        // [L]ightSalmon
        // [L]ightSeaGreen
        // [L]ightSkyBlue
        // [L]ightSlateGray
        // [L]ightSteelBlue
        // [L]ightYellow
        // [L]ime
        // [L]imeGreen
        // [L]inen
        case 'l':
            // [La]vende[r]
            if (b == 'a' && d == 'r') return {230, 230, 250};
            // [La]vender[B]lus[h]
            else if (b == 'a' && (d == 'h' || d == 'b')) return {255, 240, 245};
            // [Law]n[G]ree[n]
            else if (b == 'g' || (b == 'a' && (c == 'w' || d == 'n'))) return {124, 252, 0};
            // [Le]mon[C]hiffon
            else if (b == 'e' || b == 'c') return {255, 250, 205};
            // [Li]me[G]reen
            else if (b == 'g' || (b == 'i' && (len > 4 && str[4] == 'g'))) return {50, 205, 50};
            // [Lime]
            else if (b == 'i' && c == 'm' && d == 'e') return {0, 255, 0};
            // [Lin]e[n]
            else if (b == 'i' && (c == 'n' || d == 'n')) return {250, 240, 230};
            // [Li]ghtBlue
            // [Li]ghtCoral
            // [Li]ghtCyan
            // [Li]ghtGoldenRodYellow
            // [Li]ghtGray
            // [Li]ghtGreen
            // [Li]ghtPink
            // [Li]ghtSalmon
            // [Li]ghtSeaGreen
            // [Li]ghtSkyBlue
            // [Li]ghtSlateGray
            // [Li]ghtSteelBlue
            // [Li]ghtYellow
            else if (b == 'i') {
                // [Li]ght[B]lue
                if (len > 5 && str[5] == 'b') return {173, 216, 230};
                // [Li]ght[Co]ra[l]
                else if ((len > 5 && str[5] == 'c') && (d == 'l' || d == 'o')) return {240, 128, 128};
                // [Li]ght[Cy]a[n]
                else if ((len > 5 && str[5] == 'c') && (d == 'n' || d == 'y')) return {224, 255, 255};
                // [Li]ght[Go]ldenRodYello[w]
                else if ((len > 5 && str[5] == 'g') && (d == 'w' || d == 'o')) return {250, 250, 210};
                // [Li]ght[G]r[ay]
                else if ((len > 5 && str[5] == 'g') && (d == 'y' || d == 'a')) return {211, 211, 211};
                // [Li]ght[G]r[een]
                else if ((len > 5 && str[5] == 'g') && (d == 'n' || d == 'e')) return {144, 238, 144};
                // [Li]ght[P]ink
                else if (len > 5 && str[5] == 'p') return {255, 182, 193};
                // [Li]ght[Sa]lmon
                else if (len > 6 && str[5] == 's' && str[5] == 'a') return {255, 160, 122};
                // [Li]ght[Se]aGreen
                else if (len > 6 && str[5] == 's' && str[5] == 'e') return {32, 178, 170};
                // [Li]ght[Sk]yBlue
                else if (len > 6 && str[5] == 's' && str[5] == 'k') return {135, 206, 250};
                // [Li]ght[Sl]ateGray
                else if (len > 6 && str[5] == 's' && str[5] == 'l') return {119, 136, 153};
                // [Li]ght[St]eelBlue
                else if (len > 6 && str[5] == 's' && str[5] == 't') return {176, 196, 222};
                // [Li]ght[Y]ellow
                else if (len > 5 && str[5] == 'y') return {255, 255, 224};
                // Default to blank
                else return Color3::NIL;
            // Default to blank
            } else return Color3::NIL;
        // [M]agenta
        // [M]aroon
        // [M]ediumAquaMarine
        // [M]ediumBlue
        // [M]ediumOrchid
        // [M]ediumPurple
        // [M]ediumSeaGreen
        // [M]ediumSlateBlue
        // [M]ediumSpringGreen
        // [M]ediumTurquoise
        // [M]ediumVioletRed
        // [M]idnightBlue
        // [M]intCream
        // [M]istyRose
        // [M]occasin
        case 'm':
            // [Ma]genta
            if (b == 'a' && (c == 'a' || d == 'a')) return {255, 0, 255};
            // [Ma]roon
            else if (b == 'a' && (c == 'r' || d == 'n' || d == 'o')) return {128, 0, 0};
            // [Me]diumAquaMarine
            // [Me]diumBlue
            // [Me]diumOrchid
            // [Me]diumPurple
            // [Me]diumSeaGreen
            // [Me]diumSlateBlue
            // [Me]diumSpringGreen
            // [Me]diumTurquoise
            // [Me]diumVioletRed
            else if (b == 'e') {
                // [Me]dium[A]quaMarine
                if (c == 'a' || (len > 6 && str[6] == 'a')) return {102, 205, 170};
                // [Me]dium[B]lue
                else if (c == 'b' || (len > 6 && str[6] == 'b')) return {0, 0, 205};
                // [Me]dium[O]rchid
                else if (c == 'o' || (len > 6 && str[6] == 'o')) return {186, 85, 211};
                // [Me]dium[P]urple
                else if (c == 'p' || (len > 6 && str[6] == 'p')) return {147, 112, 219};
                // [Me]dium[T]urquoise
                else if (c == 't' || (len > 6 && str[6] == 't')) return {72, 209, 204};
                // [Me]dium[V]ioletRed
                else if (c == 'v' || (len > 6 && str[6] == 'v')) return {199, 21, 133};
                // [Me]dium[Se]aGreen
                else if (len > 7 && str[6] == 's' && str[7] == 'e') return {60, 179, 113};
                // [Me]dium[Sl]ateBlue
                else if (len > 7 && str[6] == 's' && str[7] == 'l') return {123, 104, 238};
                // [Me]dium[Sp]ringGreen
                else if (len > 7 && str[6] == 's' && str[7] == 'p') return {0, 250, 154};
                // Default to blank
                else return Color3::NIL;
            }
            // [Mi]dnightBlue
            else if (b == 'i' && c == 'd') return {25, 25, 112};
            // [Mi]ntCream
            else if (b == 'i' && c == 'n') return {245, 255, 250};
            // [Mi]styRose
            else if (b == 'i' && c == 's') return {255, 228, 225};
            // [Mo]ccasin
            else if (b == 'o') return {255, 228, 181};
            // Default to blank
            else return Color3::NIL;
        // [N]avajoWhite
        // [N]avy
        case 'n':
            // [Na]vajo[W]hite
            if (c == 'v' || c == 'w') return {255, 222, 173};
            // [Na]v[y]
            else if (c == 'a' || d == 'y') return {0, 0, 128};
            // Default to blank
            else return Color3::NIL;
        // [O]ldLace
        // [O]live
        // [O]liveDrab
        // [O]range
        // [O]rangeRed
        // [O]rchid
        case 'o':
            // [Old]Lace
            if (b == 'l' && c == 'd') return {253, 245, 230};
            // [Ol]ive[D]ra[b]
            else if (b == 'l' && (d == 'b' || d == 'd')) return {107, 142, 35};
            // [Ol]iv[e]
            else if (b == 'l' && d == 'e') return {128, 128, 0};
            // [Or]ange[R]e[d]
            else if (b == 'r' && (d == 'd' || d == 'r')) return {255, 69, 0};
            // [Or]ang[e]
            else if (b == 'r' && d == 'e') return {255, 165, 0};
            // [Orc]hid
            else if (d == 'c') return {218, 112, 214};
            // Default to blank
            else return Color3::NIL;
        // [P]aleGoldenRod
        // [P]aleGreen
        // [P]aleTurquoise
        // [P]aleVioletRed
        // [P]apayaWhip
        // [P]eachPuff
        // [P]eru
        // [P]ink
        // [P]lum
        // [P]owderBlue
        // [P]urple
        case 'p':
            // [Pu]rple
            if (b == 'u') return {128, 0, 128};
            // [Po]wderBlue
            else if (b == 'o') return {176, 224, 230};
            // [Pi]nk
            else if (b == 'i') return {255, 192, 203};
            // [Pl]um
            else if (b == 'l') return {221, 160, 221};
            // [Pea]chPuff
            else if (b == 'e' && c == 'a') return {255, 218, 185};
            // [Per]u
            else if (b == 'e' && c == 'r') return {205, 133, 63};
            // [Pa]payaWhip
            else if (b == 'a' && c == 'p') return {255, 239, 213};
            // [Pa]le[Go]ldenRod
            else if (b == 'a' && (len > 5 && str[4] == 'g' && str[5] == 'o')) return {238, 232, 170};
            // [Pa]le[Gr]een
            else if (b == 'a' && (len > 5 && str[4] == 'g' && str[5] == 'r')) return {152, 251, 152};
            // [Pa]le[T]urquoise
            else if (b == 'a' && (len > 4 && str[4] == 't')) return {175, 238, 238};
            // [Pa]le[V]ioletRed
            else if (b == 'a' && (len > 4 && str[4] == 'v')) return {219, 112, 147};
            // Default to blank
            else return Color3::NIL;
        // [R]ed
        // [R]osyBrown
        // [R]oyalBlue
        case 'r':
            // [Re]d
            if (b == 'e') return {255, 0, 0};
            // [Ros]yBrown
            else if (b == 'o' && c == 's') return {188, 143, 143};
            // [Roy]alBlue
            else if (b == 'o' && c == 'y') return {65, 105, 225};
            // Default to blank
            else return Color3::NIL;
        // [S]addleBrown
        // [S]almon
        // [S]andyBrown
        // [S]eaGreen
        // [S]eaShell
        // [S]ienna
        // [S]ilver
        // [S]kyBlue
        // [S]lateBlue
        // [S]lateGray
        // [S]now
        // [S]pringGreen
        // [S]teelBlue
        case 's':
            // [Sad]dleBrown
            if (b == 'a' && c == 'd') return {139, 69, 19};
            // [Sal]mon
            else if (b == 'a' && c == 'l') return {250, 128, 114};
            // [San]dyBrown
            else if (b == 'a' && c == 'n') return {244, 164, 96};
            // [Se]a[G]reen
            else if (b == 'e' && d == 'g') return {46, 139, 87};
            // [Se]a[S]hell
            else if (b == 'e' && d == 's') return {255, 245, 238};
            // [Sie]nna
            else if (b == 'i' && c == 'e') return {160, 82, 45};
            // [Sil]ver
            else if (b == 'i' && c == 'l') return {192, 192, 192};
            // [Sk]yBlue
            else if (b == 'k') return {135, 206, 235};
            // [Sl]ateBlue
            else if (b == 'l' && (d == 'e' || (len > 5 && str[5] == 'b'))) return {106, 90, 205};
            // [Sl]ateGray
            else if (b == 'l' && (d == 'y' || (len > 5 && str[5] == 'g'))) return {112, 128, 144};
            // [Sn]ow
            else if (b == 'n') return {255, 250, 250};
            // [Sp]ringGreen
            else if (b == 'p') return {0, 255, 127};
            // [St]eelBlue
            else if (b == 't') return {70, 130, 180};
            // Default to blank
            else return Color3::NIL;
        // [T]an
        // [T]eal
        // [T]histle
        // [T]omato
        // [T]urquoise
        case 't':
            switch(b)
            {
                // [Ta]n
                case 'a': return {210, 180, 140};
                // [Te]al
                case 'e': return {0, 128, 128};
                // [Th]istle
                case 'h': return {216, 191, 216};
                // [To]mato
                case 'o': return {255, 99, 71};
                // [Tu]rquoise
                case 'u': return {64, 224, 208};
                // Default to blank
                default: return Color3::NIL;
            }
        // [V]iolet
        case 'v': return {238, 130, 238};
        // [W]heat
        // [W]hite
        // [W]hiteSmoke
        case 'w':
            // [Wh]eat
            if (b == 'h' && c == 'e') return {245, 222, 179};
            // [Wh]ite[S]moke
            else if (b == 'h' && (len > 5 && str[5] == 's')) return {245, 245, 245};
            // [Whi]te
            else if (b == 'h' && c == 'i') return {255, 255, 255};
            // Default to blank
            else return Color3::NIL;
        // [Y]ellow
        // [Y]ellowGreen
        case 'y':
            // [Ye]llow[G]reen
            if (b == 'e' && (len > 6 && str[6] == 'g')) return {154, 205, 50};
            // [Yel]low
            else if (b == 'e' && c == 'l') return {255, 255, 0};
            // Default to blank
            else return Color3::NIL;
        // Default to blank
        default: return Color3::NIL;
    }
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqPackRGB(SQInteger r, SQInteger g, SQInteger b)
{
    return static_cast< int32_t >(SQMOD_PACK_RGB(
            ConvTo< uint8_t >::From(r), // NOLINT(hicpp-signed-bitwise)
            ConvTo< uint8_t >::From(g),
            ConvTo< uint8_t >::From(b)
        ));
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqPackRGBA(SQInteger r, SQInteger g, SQInteger b, SQInteger a)
{
    return static_cast< int32_t >(SQMOD_PACK_RGBA(
            ConvTo< uint8_t >::From(r), // NOLINT(hicpp-signed-bitwise)
            ConvTo< uint8_t >::From(g),
            ConvTo< uint8_t >::From(b),
            ConvTo< uint8_t >::From(a)
        ));
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqPackARGB(SQInteger r, SQInteger g, SQInteger b, SQInteger a)
{
    return static_cast< int32_t >(SQMOD_PACK_ARGB(
            ConvTo< uint8_t >::From(a), // NOLINT(hicpp-signed-bitwise)
            ConvTo< uint8_t >::From(r),
            ConvTo< uint8_t >::From(g),
            ConvTo< uint8_t >::From(b)
        ));
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqNameFilterCheck(HSQUIRRELVM vm)
{
    const int32_t top = sq_gettop(vm);
    // Was the filter string specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing filter string");
    }
    // Was the name string specified?
    else if (top <= 2)
    {
        return sq_throwerror(vm, "Missing name string");
    }
    // Attempt to generate the string value
    StackStrF filter(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(filter.Proc(false)))
    {
        return filter.mRes; // Propagate the error!
    }
    // Attempt to generate the string value
    StackStrF name(vm, 3);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(name.Proc(true)))
    {
        return name.mRes; // Propagate the error!
    }
    // Make the comparison and push the result on the stack
    sq_pushbool(vm, static_cast< SQBool >(NameFilterCheck(filter.mPtr, name.mPtr)));
    // Specify that we have a return value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
struct CmpEQ {
    static const String FSTR;
    inline bool operator() (SQInteger r) const noexcept { return (r == 0); }
};
struct CmpNE {
    static const String FSTR;
    inline bool operator() (SQInteger r) const noexcept { return (r != 0); }
};
struct CmpLT {
    static const String FSTR;
    inline bool operator() (SQInteger r) const noexcept { return (r < 0); }
};
struct CmpGT {
    static const String FSTR;
    inline bool operator() (SQInteger r) const noexcept { return (r > 0); }
};
struct CmpLE {
    static const String FSTR;
    inline bool operator() (SQInteger r) const noexcept { return (r <= 0); }
};
struct CmpGE {
    static const String FSTR;
    inline bool operator() (SQInteger r) const noexcept { return (r >= 0); }
};

// ------------------------------------------------------------------------------------------------
const String CmpEQ::FSTR("Assertion passed: {0} == {1}"); // NOLINT(cert-err58-cpp)
const String CmpNE::FSTR("Assertion passed: {0} != {1}"); // NOLINT(cert-err58-cpp)
const String CmpLT::FSTR("Assertion passed: {0} < {1}"); // NOLINT(cert-err58-cpp)
const String CmpGT::FSTR("Assertion passed: {0} > {1}"); // NOLINT(cert-err58-cpp)
const String CmpLE::FSTR("Assertion passed: {0} <= {1}"); // NOLINT(cert-err58-cpp)
const String CmpGE::FSTR("Assertion passed: {0} >= {1}"); // NOLINT(cert-err58-cpp)

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqAssertRes, _SC("SqAssertResult"))

// ------------------------------------------------------------------------------------------------
struct SqAssertResult
{
    /* --------------------------------------------------------------------------------------------
     * First assert argument.
    */
    String  mA{};

    /* --------------------------------------------------------------------------------------------
     * Second assert argument.
    */
    String  mB{};

    /* --------------------------------------------------------------------------------------------
     * Assert source file.
    */
    String  mSource{};

    /* --------------------------------------------------------------------------------------------
     * Assert location.
    */
    SQInteger mLine{0};

    /* --------------------------------------------------------------------------------------------
     * Second assert argument.
    */
    const String & mFStr{};

    /* --------------------------------------------------------------------------------------------
     * Assert location.
    */
    SQInteger mResult{SQ_OK};

    // --------------------------------------------------------------------------------------------
    static const String FSTR;

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    SqAssertResult(HSQUIRRELVM vm, SQInteger top, const String & fstr = FSTR)
        : mA(), mB(), mSource(), mLine(0), mFStr(fstr), mResult(SQ_OK)
    {
        // Used to acquire stack information
        SQStackInfos si;
        // Obtain information about the current stack level
        mResult = sq_stackinfos(vm, 1, &si);
        // Validate result
        if (SQ_FAILED(mResult))
        {
            return; // Abort
        }
        // Store the source
        mSource.assign(si.source ? si.source : _SC("unknown"));
        // Store the line
        mLine = si.line;
        // First argument
        StackStrF a(vm, 2);
        // Get the first value as string
        if (SQ_FAILED(a.Proc(false)))
        {
            mResult = a.mRes;
            return; // Abort
        }
        // Store the first argument
        mA.assign(a.mPtr, a.GetSize());
        // Has second parameter?
        if (top > 2)
        {
            // Second argument
            StackStrF b(vm, 3);
            // Get the second value as string
            if (SQ_FAILED(b.Proc(false)))
            {
                mResult = a.mRes;
                return; // Abort
            }
            // Store the first argument
            mB.assign(b.mPtr, b.GetSize());
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    SqAssertResult(const SqAssertResult &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor (disabled).
    */
    SqAssertResult(SqAssertResult &&) noexcept = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~SqAssertResult() = default;

    /* --------------------------------------------------------------------------------------------
     * Assignment operator (disabled).
    */
    SqAssertResult & operator = (const SqAssertResult &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment (disabled).
    */
    SqAssertResult & operator = (SqAssertResult &&) noexcept = delete;

    /* --------------------------------------------------------------------------------------------
     * Log what happened. Can be custom or default message.
    */
    SqAssertResult & What(StackStrF & s)
    {
        // Default?
        if (s.mLen <= 0)
        {
            LogScs(fmt::format(mFStr, mA, mB).c_str());
        }
        else
        {
            LogScs(fmt::format(s.ToStr(), mA, mB).c_str());
        }
        // Allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Log where it happened.
    */
    SqAssertResult & Where()
    {
        LogSInf("=>In: %s : %d", mSource.c_str(), static_cast< int >(mLine));
        // Allow chaining
        return *this;
    }
};

// ------------------------------------------------------------------------------------------------
const String SqAssertResult::FSTR("Assertion passed ({0})."); // NOLINT(cert-err58-cpp)

// ------------------------------------------------------------------------------------------------
static SQInteger SqNameFilterCheckInsensitive(HSQUIRRELVM vm)
{
    const int32_t top = sq_gettop(vm);
    // Was the filter string specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing filter string");
    }
    // Was the name string specified?
    else if (top <= 2)
    {
        return sq_throwerror(vm, "Missing name string");
    }
    // Attempt to generate the string value
    StackStrF filter(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(filter.Proc(false)))
    {
        return filter.mRes; // Propagate the error!
    }
    // Attempt to generate the string value
    StackStrF name(vm, 3);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(name.Proc(true)))
    {
        return name.mRes; // Propagate the error!
    }
    // Make the comparison and push the result on the stack
    sq_pushbool(vm, static_cast< SQBool >(NameFilterCheckInsensitive(filter.mPtr, name.mPtr)));
    // Specify that we have a return value on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqAssert(HSQUIRRELVM vm)
{
    // Get the number of arguments
    SQInteger top = sq_gettop(vm);
    // Make sure we have enough
    if (top < 2)
    {
        return sq_throwerror(vm, "Not enough parameters.");
    }
    // We only validate boolean values
    SQBool b = SQFalse;
    // Retrieve the argument as boolean
    sq_tobool(vm, 2, &b);
    // Check assertion result
    if (b != SQTrue)
    {
        return sq_throwerror(vm, "Assertion failed.");
    }
    // Try to return assert result
    try
    {
        // Create the instance and guard it to make sure it gets deleted in case of exceptions
        DeleteGuard< SqAssertResult > instance(new SqAssertResult(vm, top));
        // Push the instance on the stack
        ClassType< SqAssertResult >::PushInstance(vm, instance);
        // Stop guarding the instance
        instance.Release();
        // Specify that we returned a value
        return 1;
    }
    catch (const std::exception & e)
    {
        return sq_throwerror(vm, e.what());
    }
    SQ_UNREACHABLE
    // Unreachable
    return 0;
}

// ------------------------------------------------------------------------------------------------
template < class C > static SQInteger SqAssertValue(HSQUIRRELVM vm)
{
    // Get the number of arguments
    SQInteger top = sq_gettop(vm);
    // Make sure we have enough
    if (top < 3)
    {
        return sq_throwerror(vm, "Not enough parameters.");
    }
    StackStrF a(vm, 2);
    StackStrF b(vm, 3);
    // Get the first value as string
    if (SQ_FAILED(a.Proc(false)))
    {
        return a.mRes; // Propagate the error
    }
    // Get the second value as string
    if (SQ_FAILED(b.Proc(false)))
    {
        return b.mRes; // Propagate the error
    }
    // Compare values
    SQInteger r = sq_cmp(vm);
    // Validate result
    if (C{}(r) == false)
    {
        // Throw the error
        return sq_throwerrorf(vm, "Assertion failed. Value mismatch: %s != %s", a.mPtr, b.mPtr);
    }
    // Try to return assert result
    try
    {
        // Create the instance and guard it to make sure it gets deleted in case of exceptions
        DeleteGuard< SqAssertResult > instance(new SqAssertResult(vm, top, C::FSTR));
        // Push the instance on the stack
        ClassType< SqAssertResult >::PushInstance(vm, instance);
        // Stop guarding the instance
        instance.Release();
        // Specify that we returned a value
        return 1;
    }
    catch (const std::exception & e)
    {
        return sq_throwerror(vm, e.what());
    }
    SQ_UNREACHABLE
    // Unreachable
    return 0;
}

// ------------------------------------------------------------------------------------------------
template < class C > static SQInteger SqAssertSame(HSQUIRRELVM vm)
{
    // Get the number of arguments
    SQInteger top = sq_gettop(vm);
    // Make sure we have enough
    if (top < 3)
    {
        return sq_throwerror(vm, "Not enough parameters.");
    }
    // Validate types
    if (sq_gettype(vm, 2) != sq_gettype(vm, 3))
    {
        return sq_throwerrorf(vm, "Assertion failed. Type mismatch: %s != %s",
                                SqTypeName(vm, 2).c_str(), SqTypeName(vm, 3).c_str());
    }
    // Try to return assert result
    try
    {
        // Create the instance and guard it to make sure it gets deleted in case of exceptions
        DeleteGuard< SqAssertResult > instance(new SqAssertResult(vm, top, C::FSTR));
        // Push the instance on the stack
        ClassType< SqAssertResult >::PushInstance(vm, instance);
        // Stop guarding the instance
        instance.Release();
        // Specify that we returned a value
        return 1;
    }
    catch (const std::exception & e)
    {
        return sq_throwerror(vm, e.what());
    }
    SQ_UNREACHABLE
    // Unreachable
    return 0;
}

// ================================================================================================
void Register_Base(HSQUIRRELVM vm)
{
    // --------------------------------------------------------------------------------------------
    RootTable(vm).Bind(SqAssertRes::Str,
        Class< SqAssertResult, NoCopy< SqAssertResult > >(vm, SqAssertRes::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqAssertRes::Fn)
        // Member Methods
        .Func(_SC("What"), &SqAssertResult::What)
        .Func(_SC("Where"), &SqAssertResult::Where)
    );

    RootTable(vm)
    .Func(_SC("EpsEq"), &EpsEq< SQFloat, SQFloat >)
    .Func(_SC("EpsLt"), &EpsLt< SQFloat, SQFloat >)
    .Func(_SC("EpsGt"), &EpsGt< SQFloat, SQFloat >)
    .Func(_SC("EpsLtEq"), &EpsLtEq< SQFloat, SQFloat >)
    .Func(_SC("EpsGtEq"), &EpsGtEq< SQFloat, SQFloat >)
    .Func(_SC("ClampI"), &Clamp< SQInteger, SQInteger, SQInteger >)
    .Func(_SC("ClampF"), &Clamp< SQFloat, SQFloat, SQFloat >)
    .Func(_SC("ClampMinI"), &ClampMin< SQInteger, SQInteger >)
    .Func(_SC("ClampMinF"), &ClampMin< SQFloat, SQFloat >)
    .Func(_SC("ClampMaxI"), &ClampMax< SQInteger, SQInteger >)
    .Func(_SC("ClampMaxF"), &ClampMax< SQFloat, SQFloat >)
    .Func(_SC("NextPow2"), &NextPow2)
    .Func(_SC("SToB"), &SToB)
    .Func(_SC("PackRGB"), &SqPackRGB)
    .Func(_SC("PackRGBA"), &SqPackRGBA)
    .Func(_SC("PackARGB"), &SqPackARGB)
    .SquirrelFunc(_SC("NameFilterCheck"), &SqNameFilterCheck)
    .SquirrelFunc(_SC("NameFilterCheckInsensitive"), &SqNameFilterCheckInsensitive)
    .SquirrelFunc(_SC("SqAssert"), &SqAssert)
    .SquirrelFunc(_SC("SqAssertEQ"), &SqAssertValue< CmpEQ >)
    .SquirrelFunc(_SC("SqAssertNE"), &SqAssertValue< CmpNE >)
    .SquirrelFunc(_SC("SqAssertLT"), &SqAssertValue< CmpLT >)
    .SquirrelFunc(_SC("SqAssertGT"), &SqAssertValue< CmpGT >)
    .SquirrelFunc(_SC("SqAssertLE"), &SqAssertValue< CmpLE >)
    .SquirrelFunc(_SC("SqAssertGE"), &SqAssertValue< CmpGE >)
    .SquirrelFunc(_SC("SqAssertEQ_"), &SqAssertSame< CmpEQ >)
    .SquirrelFunc(_SC("SqAssertNE_"), &SqAssertSame< CmpNE >)
    .SquirrelFunc(_SC("SqAssertLT_"), &SqAssertSame< CmpLT >)
    .SquirrelFunc(_SC("SqAssertGT_"), &SqAssertSame< CmpGT >)
    .SquirrelFunc(_SC("SqAssertLE_"), &SqAssertSame< CmpLE >)
    .SquirrelFunc(_SC("SqAssertGE_"), &SqAssertSame< CmpGE >);
}

} // Namespace:: SqMod
