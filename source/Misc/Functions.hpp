#ifndef _MISC_FUNCTIONS_HPP_
#define _MISC_FUNCTIONS_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
CCStr GetKeyCodeName(Uint8 keycode);
void SetKeyCodeName(Uint8 keycode, CCStr name);

// ------------------------------------------------------------------------------------------------
Uint32 GetPluginVersion();
CCStr GetPluginVersionStr();
CCStr GetPluginName();
CCStr GetPluginAuthor();
Int32 GetPluginID();
Uint32 GetNumberOfPlugins();
Int32 FindPlugin(CCStr name);

// ------------------------------------------------------------------------------------------------
Uint32 GetServerVersion();

// ------------------------------------------------------------------------------------------------
Uint32 GetServerPort();
Uint32 GetServerFlags();

// ------------------------------------------------------------------------------------------------
void SetServerName(CCStr name);
CCStr GetServerName();

// ------------------------------------------------------------------------------------------------
ULongInt GetTime();

// ------------------------------------------------------------------------------------------------
void SendCustomCommand(Uint32 type, CCStr cmd);

// ------------------------------------------------------------------------------------------------
void SetMaxPlayers(Int32 max);
Int32 GetMaxPlayers(void);

// ------------------------------------------------------------------------------------------------
void SetServerPassword(CCStr passwd);
CCStr GetServerPassword();

// ------------------------------------------------------------------------------------------------
void SetGameModeText(CCStr text);
CCStr GetGameModeText();

// ------------------------------------------------------------------------------------------------
Int32 PlaySound(Int32 world, Int32 sound, const Vector3 & pos);
Int32 PlaySoundEx(Int32 world, Int32 sound, Float32 x, Float32 y, Float32 z);

// ------------------------------------------------------------------------------------------------
Int32 AddRadioStream(Int32 id, CCStr name, CCStr url, bool listed);
Int32 RemoveRadioStream(Int32 id);

} // Namespace:: SqMod

#endif // _MISC_FUNCTIONS_HPP_
