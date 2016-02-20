#ifndef _MISC_PLAYER_HPP_
#define _MISC_PLAYER_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
void SetUseClasses(bool toggle);
bool GetUseClasses(void);

// ------------------------------------------------------------------------------------------------
Int32 AddPlayerClass(Int32 team, const Color3 & color, Int32 skin, const Vector3 & pos, Float32 angle,
                    Int32 w1, Int32 a1, Int32 w2, Int32 a2, Int32 w3, Int32 a3);

// ------------------------------------------------------------------------------------------------
void SetSpawnPlayerPos(const Vector3 & pos);
void SetSpawnPlayerPosEx(Float32 x, Float32 y, Float32 z);

// ------------------------------------------------------------------------------------------------
void SetSpawnCameraPos(const Vector3 & pos);
void SetSpawnCameraPosEx(Float32 x, Float32 y, Float32 z);

// ------------------------------------------------------------------------------------------------
void SetSpawnCameraLookAt(const Vector3 & pos);
void SetSpawnCameraLookAtEx(Float32 x, Float32 y, Float32 z);

// ------------------------------------------------------------------------------------------------
CCStr GetSkinName(Uint32 id);
void SetSkinName(Uint32 id, CCStr name);
Int32 GetSkinID(CCStr name);
bool IsSkinValid(Int32 id);

// ------------------------------------------------------------------------------------------------
Object & FindPlayer(Object & by);

} // Namespace:: SqMod

#endif // _MISC_PLAYER_HPP_
