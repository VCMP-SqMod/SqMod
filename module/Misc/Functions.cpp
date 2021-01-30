// ------------------------------------------------------------------------------------------------
#include "Misc/Functions.hpp"
#include "Core.hpp"
#include "Base/Color3.hpp"
#include "Base/Vector2.hpp"
#include "Entity/Player.hpp"
#include "Library/Numeric/Long.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Helper structure used to represent the boundaries of a certain district.
*/
struct District
{
    // District boundaries.
    float mMinX, mMinY, mMaxX, mMaxY;
    // District name
    const SQChar * mName;
};

// ------------------------------------------------------------------------------------------------
static ServerSettings   g_SvSettings;
static PluginInfo       g_PluginInfo;

// ------------------------------------------------------------------------------------------------
static SQChar g_SvNameBuff[SQMOD_SVNAMELENGTH] = {0};
static SQChar g_PasswdBuff[SQMOD_PASSWDLENGTH] = {0};
static SQChar g_GmNameBuff[SQMOD_GMNAMELENGTH] = {0};

// ------------------------------------------------------------------------------------------------
static const District g_Districts[] = {
    {-1613.03f,     413.218f,   -213.73f,   1677.32f,   _SC("Downtown Vice City")},
    {163.656f,      -351.153f,  1246.03f,   1398.85f,   _SC("Vice Point")},
    {-103.97f,      -930.526f,  1246.03f,   -351.153f,  _SC("Washington Beach")},
    {-253.206f,     -1805.37f,  1254.9f,    -930.526f,  _SC("Ocean Beach")},
    {-1888.21f,     -1779.61f,  -1208.21f,  230.39f,    _SC("Escobar International Airport")},
    {-748.206f,     -818.266f,  -104.505f,  -241.467f,  _SC("Starfish Island")},
    {-213.73f,      797.605f,   163.656f,   1243.47f,   _SC("Prawn Island")},
    {-213.73f,      -241.429f,  163.656f,   797.605f,   _SC("Leaf Links")},
    {-1396.76f,     -42.9113f,  -1208.21f,  230.39f,    _SC("Junkyard")},
    {-1208.21f,     -1779.61f,  -253.206f,  -898.738f,  _SC("Viceport")},
    {-1208.21f,     -898.738f,  -748.206f,  -241.467f,  _SC("Little Havana")},
    {-1208.21f,     -241.467f,  -578.289f,  412.66f,    _SC("Little Haiti")}
};

// ------------------------------------------------------------------------------------------------
static String CS_Keycode_Names[] = {"", /* index 0 is not used */ // NOLINT(cert-err58-cpp)
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
const SQChar * GetKeyCodeName(uint8_t keycode)
{
    return CS_Keycode_Names[keycode].c_str();
}

// ------------------------------------------------------------------------------------------------
void SetKeyCodeName(uint8_t keycode, StackStrF & name)
{
    CS_Keycode_Names[keycode].assign(name.mPtr);
}

// ------------------------------------------------------------------------------------------------
uint32_t GetServerVersion()
{
    return _Func->GetServerVersion();
}

// ------------------------------------------------------------------------------------------------
Table GetServerSettings()
{
    // Update the server settings structure
    _Func->GetServerSettings(&g_SvSettings);
    // Allocate a script table
    Table tbl;
    // Add the structure members to the script table
    tbl.SetValue(_SC("Name"),       g_SvSettings.serverName);
    tbl.SetValue(_SC("MaxPlayers"), g_SvSettings.maxPlayers);
    tbl.SetValue(_SC("Port"),       g_SvSettings.port);
    tbl.SetValue(_SC("Flags"),      g_SvSettings.flags);
    // Return the resulted table
    return tbl;
}

// ------------------------------------------------------------------------------------------------
uint32_t GetNumberOfPlugins()
{
    return _Func->GetNumberOfPlugins();
}

// ------------------------------------------------------------------------------------------------
Table GetPluginInfo(int32_t plugin_id)
{
    // Attempt to update the plug-in info structure
    if (_Func->GetPluginInfo(plugin_id, &g_PluginInfo) == vcmpErrorNoSuchEntity)
    {
        STHROWF("Unknown plug-in identifier: %d", plugin_id);
    }
    // Allocate a script table
    Table tbl;
    // Add the structure members to the script table
    tbl.SetValue(_SC("Id"),         g_PluginInfo.pluginId);
    tbl.SetValue(_SC("Name"),       g_PluginInfo.name);
    tbl.SetValue(_SC("Version"),    g_PluginInfo.pluginVersion);
    tbl.SetValue(_SC("MajorAPI"),   g_PluginInfo.apiMajorVersion);
    tbl.SetValue(_SC("MinorAPI"),   g_PluginInfo.apiMinorVersion);
    // Return the resulted table
    return tbl;
}

// ------------------------------------------------------------------------------------------------
int32_t FindPlugin(StackStrF & name)
{
    return _Func->FindPlugin(name.mPtr);
}

// ------------------------------------------------------------------------------------------------
void SendPluginCommand(uint32_t identifier, StackStrF & payload)
{
    _Func->SendPluginCommand(identifier, payload.mPtr);
}

// ------------------------------------------------------------------------------------------------
const ULongInt & GetTime()
{
    return GetULongInt(_Func->GetTime());
}

// ------------------------------------------------------------------------------------------------
void SendLogMessage(StackStrF & msg)
{
    if (_Func->LogMessage("%s", msg.mPtr) == vcmpErrorTooLargeInput)
    {
        STHROWF("Input is too big");
    }
}

// ------------------------------------------------------------------------------------------------
int32_t GetLastError()
{
    return _Func->GetLastError();
}

// ------------------------------------------------------------------------------------------------
uint32_t GetPluginVersion()
{
    return SQMOD_VERSION;
}

// ------------------------------------------------------------------------------------------------
const SQChar * GetPluginVersionStr()
{
    return SQMOD_VERSION_STR;
}

// ------------------------------------------------------------------------------------------------
const SQChar * GetPluginName()
{
    return SQMOD_NAME;
}

// ------------------------------------------------------------------------------------------------
const SQChar * GetPluginAuthor()
{
    return SQMOD_AUTHOR;
}

// ------------------------------------------------------------------------------------------------
int32_t GetPluginID()
{
    return _Info->pluginId;
}

// ------------------------------------------------------------------------------------------------
uint32_t GetServerPort()
{
    // Update the server settings structure
    _Func->GetServerSettings(&g_SvSettings);
    // Return the requested information
    return g_SvSettings.port;
}

// ------------------------------------------------------------------------------------------------
uint32_t GetServerFlags()
{
    // Update the server settings structure
    _Func->GetServerSettings(&g_SvSettings);
    // Return the requested information
    return g_SvSettings.flags;
}

// ------------------------------------------------------------------------------------------------
int32_t GetMaxPlayers()
{
    return _Func->GetMaxPlayers();
}

// ------------------------------------------------------------------------------------------------
void SetMaxPlayers(int32_t max)
{
    _Func->SetMaxPlayers(static_cast< uint32_t >(max));
}

// ------------------------------------------------------------------------------------------------
const SQChar * GetServerName()
{
	// The server is retarded and returns `vcmpErrorBufferTooSmall` regardless of the buffer size.
    // Populate the buffer
    //if (_Func->GetServerName(g_SvNameBuff, SQMOD_SVNAMELENGTH) == vcmpErrorBufferTooSmall)
    //{
    //    STHROWF("Server name was too big for the available buffer: %u", sizeof(g_SvNameBuff));
    //}

	// TEMPORARY WROKAROUND

	// Null initialize the buffer
	memset(g_SvNameBuff, 0, sizeof(g_SvNameBuff));
	// Forward the call to the server
	_Func->GetServerName(g_SvNameBuff, SQMOD_SVNAMELENGTH);

    // Return the result (without checking for errors!!!)
    return g_SvNameBuff;
}

// ------------------------------------------------------------------------------------------------
void SetServerName(StackStrF & name)
{
    _Func->SetServerName(name.mPtr);
}

// ------------------------------------------------------------------------------------------------
const SQChar * GetServerPassword()
{
	// The server is retarded and returns `vcmpErrorBufferTooSmall` regardless of the buffer size.
    // Populate the buffer
    //if (_Func->GetServerPassword(g_PasswdBuff, SQMOD_PASSWDLENGTH) == vcmpErrorBufferTooSmall)
    //{
    //    STHROWF("Server password was too big for the available buffer: %u", sizeof(g_PasswdBuff));
    //}

	// TEMPORARY WROKAROUND

	// Null initialize the buffer
	memset(g_PasswdBuff, 0, sizeof(g_PasswdBuff));
	// Forward the call to the server
	_Func->GetServerPassword(g_PasswdBuff, SQMOD_PASSWDLENGTH);

    // Return the result (without checking for errors!!!)
    return g_PasswdBuff;
}

// ------------------------------------------------------------------------------------------------
void SetServerPassword(StackStrF & passwd)
{
    _Func->SetServerPassword(passwd.mPtr);
}

// ------------------------------------------------------------------------------------------------
const SQChar * GetGameModeText()
{
	// The server is retarded and returns `vcmpErrorBufferTooSmall` regardless of the buffer size.
    // Populate the buffer
    //if (_Func->GetGameModeText(g_GmNameBuff, SQMOD_GMNAMELENGTH) == vcmpErrorBufferTooSmall)
    //{
    //    STHROWF("Game-mode text was too big for the available buffer: %u", sizeof(g_GmNameBuff));
    //}

	// TEMPORARY WROKAROUND

	// Null initialize the buffer
	memset(g_GmNameBuff, 0, sizeof(g_GmNameBuff));
	// Forward the call to the server
	_Func->GetGameModeText(g_GmNameBuff, SQMOD_GMNAMELENGTH);

    // Return the result (without checking for errors!!!)
    return g_GmNameBuff;
}

// ------------------------------------------------------------------------------------------------
void SetGameModeText(StackStrF & text)
{
    _Func->SetGameModeText(text.mPtr);
}

// ------------------------------------------------------------------------------------------------
SQInteger CreateRadioStream(bool listed, StackStrF & name, StackStrF & url)
{
    return static_cast< SQInteger >(_Func->AddRadioStream(-1, name.mPtr, url.mPtr, static_cast< uint8_t >(listed)));
}

// ------------------------------------------------------------------------------------------------
SQInteger CreateRadioStreamEx(int32_t id, bool listed, StackStrF & name, StackStrF & url)
{
    return static_cast< SQInteger >(_Func->AddRadioStream(id, name.mPtr, url.mPtr, static_cast< uint8_t >(listed)));
}

// ------------------------------------------------------------------------------------------------
void RemoveRadioStream(int32_t id)
{
    if (_Func->RemoveRadioStream(id) == vcmpErrorNoSuchEntity)
    {
        STHROWF("No such radio stream exists");
    }
}

// ------------------------------------------------------------------------------------------------
void ShutdownServer()
{
    _Func->ShutdownServer();
}

// ------------------------------------------------------------------------------------------------
bool GetServerOption(int32_t option_id)
{
    // Attempt to obtain the current value of the specified option
    const bool value = _Func->GetServerOption(static_cast< vcmpServerOption >(option_id));
    // Check for errors
    if (_Func->GetLastError() == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Unknown option identifier: %d", option_id);
    }
    // Return the obtained value
    return value;
}

// ------------------------------------------------------------------------------------------------
void SetServerOption(int32_t option_id, bool toggle)
{
    if (_Func->SetServerOption(static_cast< vcmpServerOption >(option_id),
                               static_cast< uint8_t >(toggle)) == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Unknown option identifier: %d", option_id);
    }
    else
    {
        Core::Get().EmitServerOption(option_id, toggle, 0, NullLightObj());
    }
}

// ------------------------------------------------------------------------------------------------
void SetServerOptionEx(int32_t option_id, bool toggle, int32_t header, LightObj & payload)
{
    if (_Func->SetServerOption(static_cast< vcmpServerOption >(option_id),
                               static_cast< uint8_t >(toggle)) == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Unknown option identifier: %d", option_id);
    }
    else
    {
    Core::Get().EmitServerOption(option_id, toggle, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
Table GetWorldBounds()
{
    Vector2 max, min;
    // Retrieve the current world bounds
    _Func->GetWorldBounds(&max.x, &min.x, &max.y, &min.y);
    // Allocate a script table
    Table tbl(SqVM(), 2);
    // Populate the table with the obtained values
    tbl.SetValue(_SC("max"), max);
    tbl.SetValue(_SC("min"), min);
    // Return the result
    return tbl;
}

// ------------------------------------------------------------------------------------------------
void SetWorldBounds(const Vector2 & max, const Vector2 & min)
{
    _Func->SetWorldBounds(max.x, min.x, max.y, min.y);
}

// ------------------------------------------------------------------------------------------------
void SetWorldBoundsEx(float max_x, float max_y, float min_x, float min_y)
{
    _Func->SetWorldBounds(max_x, min_x, max_y, min_y);
}

Table GetWastedSettings()
{
    uint32_t fc, dt, ft, cfs, cft;
    float fis, fos;
    Color3 c;
    // Retrieve the current wasted settings bounds
    _Func->GetWastedSettings(&dt, &ft, &fis, &fos, &fc, &cfs, &cft);
    // Convert the packed color
    c.SetRGB(fc);
    // Allocate a script table
    Table tbl(SqVM(), 8);
    // Populate the table with the obtained values
    tbl.SetValue(_SC("DeathTimerOut"), dt);
    tbl.SetValue(_SC("FadeTimer"), ft);
    tbl.SetValue(_SC("FadeInSpeed"), fis);
    tbl.SetValue(_SC("FadeOutSpeed"), fos);
    tbl.SetValue(_SC("FadeColor"), c);
    tbl.SetValue(_SC("FadeColour"), c);
    tbl.SetValue(_SC("CorpseFadeStart"), cfs);
    tbl.SetValue(_SC("CorpseFadeTime"), cft);
    // Return the result
    return tbl;
}

// ------------------------------------------------------------------------------------------------
void SetWastedSettings(uint32_t dt, uint32_t ft, float fis, float fos,
                        const Color3 & fc, uint32_t cfs, uint32_t cft)
{
    _Func->SetWastedSettings(dt, ft, fis, fos, fc.GetRGB(), cfs, cft);
}

// ------------------------------------------------------------------------------------------------
int32_t GetTimeRate()
{
    return _Func->GetTimeRate();
}

// ------------------------------------------------------------------------------------------------
void SetTimeRate(uint32_t rate)
{
    _Func->SetTimeRate(rate);
}

// ------------------------------------------------------------------------------------------------
int32_t GetHour()
{
    return _Func->GetHour();
}

// ------------------------------------------------------------------------------------------------
void SetHour(int32_t hour)
{
    _Func->SetHour(hour);
}

// ------------------------------------------------------------------------------------------------
int32_t GetMinute()
{
    return _Func->GetMinute();
}

// ------------------------------------------------------------------------------------------------
void SetMinute(int32_t minute)
{
    _Func->SetMinute(minute);
}

// ------------------------------------------------------------------------------------------------
int32_t GetWeather()
{
    return _Func->GetWeather();
}

// ------------------------------------------------------------------------------------------------
void SetWeather(int32_t weather)
{
    _Func->SetWeather(weather);
}

// ------------------------------------------------------------------------------------------------
float GetGravity()
{
    return _Func->GetGravity();
}

// ------------------------------------------------------------------------------------------------
void SetGravity(float gravity)
{
    _Func->SetGravity(gravity);
}

// ------------------------------------------------------------------------------------------------
float GetGameSpeed()
{
    return _Func->GetGameSpeed();
}

// ------------------------------------------------------------------------------------------------
void SetGameSpeed(float speed)
{
    _Func->SetGameSpeed(speed);
}

// ------------------------------------------------------------------------------------------------
float GetWaterLevel()
{
    return _Func->GetWaterLevel();
}

// ------------------------------------------------------------------------------------------------
void SetWaterLevel(float level)
{
    _Func->SetWaterLevel(level);
}

// ------------------------------------------------------------------------------------------------
float GetMaximumFlightAltitude()
{
    return _Func->GetMaximumFlightAltitude();
}

// ------------------------------------------------------------------------------------------------
void SetMaximumFlightAltitude(float height)
{
    _Func->SetMaximumFlightAltitude(height);
}

// ------------------------------------------------------------------------------------------------
int32_t GetKillCommandDelay()
{
    return _Func->GetKillCommandDelay();
}

// ------------------------------------------------------------------------------------------------
void SetKillCommandDelay(int32_t delay)
{
    _Func->SetKillCommandDelay(delay);
}

// ------------------------------------------------------------------------------------------------
float GetVehiclesForcedRespawnHeight()
{
    return _Func->GetVehiclesForcedRespawnHeight();
}

// ------------------------------------------------------------------------------------------------
void SetVehiclesForcedRespawnHeight(float height)
{
    _Func->SetVehiclesForcedRespawnHeight(height);
}

// ------------------------------------------------------------------------------------------------
void CreateExplosion(int32_t world, int32_t type, const Vector3 & pos, CPlayer & source, bool grounded)
{
    if (_Func->CreateExplosion(world, type, pos.x, pos.y, pos.z,
                               source.GetID(), static_cast< uint8_t >(grounded)) == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Argument value out of bounds");
    }
}

// ------------------------------------------------------------------------------------------------
void CreateExplosionEx(int32_t world, int32_t type, float x, float y, float z, CPlayer & source, bool grounded)
{
    if (_Func->CreateExplosion(world, type, x, y, z,
                               source.GetID(), static_cast< uint8_t >(grounded)) == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Argument value out of bounds");
    }
}

// ------------------------------------------------------------------------------------------------
void PlaySound(int32_t world, int32_t sound, const Vector3 & pos)
{
    if (_Func->PlaySound(world, sound, pos.x, pos.y, pos.z) == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Argument value out of bounds");
    }
}

// ------------------------------------------------------------------------------------------------
void PlaySoundEx(int32_t world, int32_t sound, float x, float y, float z)
{
    if (_Func->PlaySound(world, sound, x, y, z) == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Argument value out of bounds");
    }
}

// ------------------------------------------------------------------------------------------------
void PlaySoundForWorld(int32_t world, int32_t sound)
{
    if (_Func->PlaySound(world, sound, NAN, NAN, NAN) == vcmpErrorArgumentOutOfBounds)
    {
        STHROWF("Argument value out of bounds");
    }
}

// ------------------------------------------------------------------------------------------------
void HideMapObject(int32_t model, const Vector3 & pos)
{
    _Func->HideMapObject(model,
        static_cast< int16_t >(std::lround(std::floor(pos.x * 10.0f) + 0.5f)),
        static_cast< int16_t >(std::lround(std::floor(pos.y * 10.0f) + 0.5f)),
        static_cast< int16_t >(std::lround(std::floor(pos.z * 10.0f) + 0.5f))
    );
}

// ------------------------------------------------------------------------------------------------
void HideMapObjectEx(int32_t model, float x, float y, float z)
{
    _Func->HideMapObject(model,
        static_cast< int16_t >(std::lround(std::floor(x * 10.0f) + 0.5f)),
        static_cast< int16_t >(std::lround(std::floor(y * 10.0f) + 0.5f)),
        static_cast< int16_t >(std::lround(std::floor(z * 10.0f) + 0.5f))
    );
}

// ------------------------------------------------------------------------------------------------
void HideMapObjectRaw(int32_t model, int16_t x, int16_t y, int16_t z)
{
    _Func->HideMapObject(model, x, y, z);
}

// ------------------------------------------------------------------------------------------------
void ShowMapObject(int32_t model, const Vector3 & pos)
{
    _Func->ShowMapObject(model,
        static_cast< int16_t >(std::lround(std::floor(pos.x * 10.0f) + 0.5f)),
        static_cast< int16_t >(std::lround(std::floor(pos.y * 10.0f) + 0.5f)),
        static_cast< int16_t >(std::lround(std::floor(pos.z * 10.0f) + 0.5f))
    );
}

// ------------------------------------------------------------------------------------------------
void ShowMapObjectEx(int32_t model, float x, float y, float z)
{
    _Func->ShowMapObject(model,
        static_cast< int16_t >(std::lround(std::floor(x * 10.0f) + 0.5f)),
        static_cast< int16_t >(std::lround(std::floor(y * 10.0f) + 0.5f)),
        static_cast< int16_t >(std::lround(std::floor(z * 10.0f) + 0.5f))
    );
}

// ------------------------------------------------------------------------------------------------
void ShowMapObjectRaw(int32_t model, int16_t x, int16_t y, int16_t z)
{
    _Func->ShowMapObject(model, x, y, z);
}

// ------------------------------------------------------------------------------------------------
void ShowAllMapObjects()
{
    _Func->ShowAllMapObjects();
}

// ------------------------------------------------------------------------------------------------
SQFloat GetWeaponDataValue(int32_t weapon, int32_t field)
{
    return ConvTo< SQFloat >::From(_Func->GetWeaponDataValue(weapon, field));
}

// ------------------------------------------------------------------------------------------------
bool SetWeaponDataValue(int32_t weapon, int32_t field, SQFloat value)
{
    return (_Func->SetWeaponDataValue(weapon, field, value) != vcmpErrorArgumentOutOfBounds);
}

// ------------------------------------------------------------------------------------------------
bool ResetWeaponDataValue(int32_t weapon, int32_t field)
{
    return (_Func->ResetWeaponDataValue(weapon, field) != vcmpErrorArgumentOutOfBounds);
}

// ------------------------------------------------------------------------------------------------
bool IsWeaponDataValueModified(int32_t weapon, int32_t field)
{
    return _Func->IsWeaponDataValueModified(weapon, field);
}

// ------------------------------------------------------------------------------------------------
bool ResetWeaponData(int32_t weapon)
{
    return (_Func->ResetWeaponData(weapon) != vcmpErrorArgumentOutOfBounds);
}

// ------------------------------------------------------------------------------------------------
void ResetAllWeaponData()
{
    _Func->ResetAllWeaponData();
}

// ------------------------------------------------------------------------------------------------
int32_t AddPlayerClass(int32_t team, const Color3 & color, int32_t skin, const Vector3 & pos, float angle,
                        int32_t wep1, int32_t ammo1, int32_t wep2, int32_t ammo2, int32_t wep3, int32_t ammo3)
{
    return _Func->AddPlayerClass(team, color.GetRGB(), skin, pos.x, pos.y, pos.z, angle,
                                    wep1, ammo1, wep2, ammo2, wep3, ammo3);
}

// ------------------------------------------------------------------------------------------------
void SetSpawnPlayerPosition(const Vector3 & pos)
{
    _Func->SetSpawnPlayerPosition(pos.x, pos.y, pos.z);
}

// ------------------------------------------------------------------------------------------------
void SetSpawnCameraPosition(const Vector3 & pos)
{
    _Func->SetSpawnCameraPosition(pos.x, pos.y, pos.z);
}

// ------------------------------------------------------------------------------------------------
void SetSpawnCameraLookAt(const Vector3 & pos)
{
    _Func->SetSpawnCameraLookAt(pos.x, pos.y, pos.z);
}

// ------------------------------------------------------------------------------------------------
void SetSpawnPlayerPositionEx(float x, float y, float z)
{
    _Func->SetSpawnPlayerPosition(x, y, z);
}

// ------------------------------------------------------------------------------------------------
void SetSpawnCameraPositionEx(float x, float y, float z)
{
    _Func->SetSpawnCameraPosition(x, y, z);
}

// ------------------------------------------------------------------------------------------------
void SetSpawnCameraLookAtEx(float x, float y, float z)
{
    _Func->SetSpawnPlayerPosition(x, y, z);
}

// ------------------------------------------------------------------------------------------------
void BanIP(StackStrF & addr)
{
    _Func->BanIP(const_cast< SQChar * >(addr.mPtr));
}

// ------------------------------------------------------------------------------------------------
bool UnbanIP(StackStrF & addr)
{
    return _Func->UnbanIP(const_cast< SQChar * >(addr.mPtr));
}

// ------------------------------------------------------------------------------------------------
bool IsIPBanned(StackStrF & addr)
{
    return _Func->IsIPBanned(const_cast< SQChar * >(addr.mPtr));
}

// ------------------------------------------------------------------------------------------------
int32_t GetPlayerIdFromName(StackStrF & name)
{
    return _Func->GetPlayerIdFromName(name.mPtr);
}

// ------------------------------------------------------------------------------------------------
bool IsPlayerConnected(int32_t player_id)
{
    return _Func->IsPlayerConnected(player_id);
}

// ------------------------------------------------------------------------------------------------
void ForceAllSelect()
{
    _Func->ForceAllSelect();
}

// ------------------------------------------------------------------------------------------------
bool CheckEntityExists(int32_t type, int32_t index)
{
    return _Func->CheckEntityExists(static_cast< vcmpEntityPool >(type), index);
}

// ------------------------------------------------------------------------------------------------
const SQChar * GetDistrictName(const Vector2 & point)
{
    return GetDistrictNameEx(point.x, point.y);
}

// ------------------------------------------------------------------------------------------------
const SQChar * GetDistrictNameEx(SQFloat x, SQFloat y)
{
    // Attempt to see if the specified point is within one of the known districts
    for (uint32_t n = 0; n < (sizeof(g_Districts) / sizeof(District)); ++n) // NOLINT(modernize-loop-convert)
    {
        // Grab the district
        const District & d = g_Districts[n];
        // Check for point intersection taking into account floating point comparison issues
        if (EpsGt(x, d.mMinX) && EpsGt(y, d.mMinY) && EpsLt(x, d.mMaxX) && EpsLt(y, d.mMaxY))
        {
            return d.mName; // The specified point is within the bounds of this district
        }
    }
    // Not a particular district!
    return _SC("Vice City");
}

// ------------------------------------------------------------------------------------------------
uint16_t GetFallTimer()
{
    return _Func->GetFallTimer();
}

// ------------------------------------------------------------------------------------------------
void SetFallTimer(uint16_t rate)
{
    _Func->SetFallTimer(rate);
}
#if SQMOD_SDK_LEAST(2, 1)
// ------------------------------------------------------------------------------------------------
SQFloat GetNetworkStatisticsF(int32_t option_id)
{
    // Retrieve the requested information
    double value = _Func->GetNetworkStatistics(-1, static_cast< vcmpNetworkStatisticsOption  >(option_id));
    // Return it in the proper type
    return static_cast< SQFloat >(value);
}

// ------------------------------------------------------------------------------------------------
SQInteger GetNetworkStatisticsI(int32_t option_id)
{
    // Retrieve the requested information
    double value = _Func->GetNetworkStatistics(-1, static_cast< vcmpNetworkStatisticsOption  >(option_id));
    // Return it in the proper type
    return static_cast< SQInteger >(value);
}
#endif
} // Namespace:: SqMod
