// ------------------------------------------------------------------------------------------------
#include "Misc/Functions.hpp"
#include "Base/Vector3.hpp"
#include "Library/Numeric.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstring>
#include <algorithm>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
static ServerSettings g_SvSettings;
static SQChar g_SvNameBuff[SQMOD_SVNAMELENGTH] = {0};
static SQChar g_PasswdBuff[SQMOD_PASSWDLENGTH] = {0};
static SQChar g_GmNameBuff[SQMOD_GMNAMELENGTH] = {0};

// ------------------------------------------------------------------------------------------------
static String CS_Keycode_Names[] = {"", /* index 0 is not used */
"Left Button **",    "Right Button **",   "Break",             "Middle Button **",  "X Button 1 **",
"X Button 2 **",     "",                  "Backspace",         "Tab",               "",
"",                  "Clear",             "Enter",             "",                  "",
"",                  "",                  "",                  "Pause",             "Caps Lock",
"Kana",              "",                  "Junja",             "Final",             "Kanji",
"",                  "Esc",               "Convert",           "Non Convert",       "Accept",
"Mode Change",       "Space",             "Page Up",           "Page Down",         "End",
"Home",              "Arrow Left",        "Arrow Up",          "Arrow Right",       "Arrow Down",
"Select",            "Print",             "Execute",           "Print Screen",      "Insert",
"Delete",            "Help",              "0",                 "1",                 "2",
"3",                 "4",                 "5",                 "6",                 "7",
"8",                 "9",                 "",                  "",                  "",
"",                  "",                  "",                  "",                  "A",
"B",                 "C",                 "D",                 "E",                 "F",
"G",                 "H",                 "I",                 "J",                 "K",
"L",                 "M",                 "N",                 "O",                 "P",
"Q",                 "R",                 "S",                 "T",                 "U",
"V",                 "W",                 "X",                 "Y",                 "Z",
"Left Win",          "Right Win",         "Context Menu",      "",                  "Sleep",
"Numpad 0",          "Numpad 1",          "Numpad 2",          "Numpad 3",          "Numpad 4",
"Numpad 5",          "Numpad 6",          "Numpad 7",          "Numpad 8",          "Numpad 9",
"Numpad *",          "Numpad +",          "Separator",         "Num -",             "Numpad .",
"Numpad /",          "F1",                "F2",                "F3",                "F4",
"F5",                "F6",                "F7",                "F8",                "F9",
"F10",               "F11",               "F12",               "F13",               "F14",
"F15",               "F16",               "F17",               "F18",               "F19",
"F20",               "F21",               "F22",               "F23",               "F24",
"",                  "",                  "",                  "",                  "",
"",                  "",                  "",                  "Num Lock",          "Scrol Lock",
"Jisho",             "Mashu",             "Touroku",           "Loya",              "Roya",
"",                  "",                  "",                  "",                  "",
"",                  "",                  "",                  "",                  "Left Shift",
"Right Shift",       "Left Ctrl",         "Right Ctrl",        "Left Alt",          "Right Alt",
"Browser Back",      "Browser Forward",   "Browser Refresh",   "Browser Stop",      "Browser Search",
"Browser Favorites", "Browser Home",      "Volume Mute",       "Volume Down",       "Volume Up",
"Next Track",        "Previous Track",    "Stop",              "Play / Pause",      "Mail",
"Media",             "App1",              "App2",              "",                  "",
"OEM_1 (: ;)",       "OEM_PLUS (+ =)",    "OEM_COMMA (< ,)",   "OEM_MINUS (_ -)",   "OEM_PERIOD (> .)",
"OEM_2 (? /)",       "OEM_3 (~ `)",       "Abnt C1",           "Abnt C2",           "",
"",                  "",                  "",                  "",                  "",
"",                  "",                  "",                  "",                  "",
"",                  "",                  "",                  "",                  "",
"",                  "",                  "",                  "",                  "",
"",                  "",                  "",                  "OEM_4 ({ [)",       "OEM_5 (| \\)",
"OEM_6 (} ])",       "OEM_7 (\" ')",      "OEM_8 (? !)",       "",                  "Ax",
"OEM_102 (> <)",     "IcoHlp",            "Ico00 *",           "Process",           "IcoClr",
"Packet",            "",                  "Reset",             "Jump",              "OemPa1",
"OemPa2",            "OemPa3",            "WsCtrl",            "Cu Sel",            "Oem Attn",
"Finish",            "Copy",              "Auto",              "Enlw",              "Back Tab",
"Attn",              "Cr Sel",            "Ex Sel",            "Er Eof",            "Play",
"Zoom",              "NoName",            "Pa1",               "OemClr",            "no mapping"
};

// ------------------------------------------------------------------------------------------------
CCStr GetKeyCodeName(Uint8 keycode)
{ return CS_Keycode_Names[keycode].c_str(); }
void SetKeyCodeName(Uint8 keycode, CCStr name)
{ CS_Keycode_Names[keycode].assign(name); }

// ------------------------------------------------------------------------------------------------
Uint32 GetPluginVersion()
{ return SQMOD_VERSION; }
CCStr GetPluginVersionStr()
{ return SQMOD_VERSION_STR; }
CCStr GetPluginName()
{ return SQMOD_NAME; }
CCStr GetPluginAuthor()
{ return SQMOD_AUTHOR; }
Int32 GetPluginID()
{ return _Info->nPluginId; }
Uint32 GetNumberOfPlugins()
{ return _Func->GetNumberOfPlugins(); }
Int32 FindPlugin(CCStr name)
{ return _Func->FindPlugin(const_cast< CStr >(name)); }

// ------------------------------------------------------------------------------------------------
Uint32 GetServerVersion()
{ return _Func->GetServerVersion(); }

// ------------------------------------------------------------------------------------------------
Uint32 GetServerPort()
{
    _Func->GetServerSettings(&g_SvSettings);
    return g_SvSettings.uPort;
}

Uint32 GetServerFlags()
{
    _Func->GetServerSettings(&g_SvSettings);
    return g_SvSettings.uFlags;
}

// ------------------------------------------------------------------------------------------------
void SetServerName(CCStr name)
{ _Func->SetServerName(name); }
CCStr GetServerName()
{
    _Func->GetServerName(g_SvNameBuff, SQMOD_SVNAMELENGTH);
    return g_SvNameBuff;
}

// ------------------------------------------------------------------------------------------------
ULongInt GetTime()
{
    std::uint64_t time = 0;
    _Func->GetTime(&time);
    return ULongInt(time);
}

// ------------------------------------------------------------------------------------------------
void SendCustomCommand(Uint32 type, CCStr cmd)
{
    _Func->SendCustomCommand(type, cmd);
}

// ------------------------------------------------------------------------------------------------
void SetMaxPlayers(Int32 max)
{
    _Func->SetMaxPlayers(max);
}

Int32 GetMaxPlayers(void)
{
    return _Func->GetMaxPlayers();
}

// ------------------------------------------------------------------------------------------------
void SetServerPassword(CCStr passwd)
{ _Func->SetServerPassword(const_cast< CStr >(passwd)); }
CCStr GetServerPassword()
{
    _Func->GetServerPassword(g_PasswdBuff, SQMOD_PASSWDLENGTH);
    return g_PasswdBuff;
}

// ------------------------------------------------------------------------------------------------
void SetGameModeText(CCStr text)
{ _Func->SetGameModeText(text); }
CCStr GetGameModeText()
{
    _Func->GetGameModeText(g_GmNameBuff, SQMOD_GMNAMELENGTH);
    return g_GmNameBuff;
}

// ------------------------------------------------------------------------------------------------
Int32 PlaySound(Int32 world, Int32 sound, const Vector3 & pos)
{ return _Func->PlaySound(world, sound, pos.x, pos.y, pos.z); }
Int32 PlaySoundEx(Int32 world, Int32 sound, Float32 x, Float32 y, Float32 z)
{ return _Func->PlaySound(world, sound, x, y, z); }

// ------------------------------------------------------------------------------------------------
Int32 AddRadioStream(Int32 id, CCStr name, CCStr url, bool listed)
{  return _Func->AddRadioStream(id, name, url, listed); }
Int32 RemoveRadioStream(Int32 id)
{ return _Func->RemoveRadioStream(id); }

} // Namespace:: SqMod
