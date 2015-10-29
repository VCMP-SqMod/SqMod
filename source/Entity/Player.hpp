#ifndef _ENTITY_PLAYER_HPP_
#define _ENTITY_PLAYER_HPP_

// ------------------------------------------------------------------------------------------------
#include "Entity.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class responsible for managing the referenced player instance.
*/
class CPlayer : public Reference< CPlayer >
{
    // --------------------------------------------------------------------------------------------
    static CSkin    s_Skin;
    static CWeapon  s_Weapon;

    // --------------------------------------------------------------------------------------------
    static Color3   s_Color3;
    static Vector3  s_Vector3;

    // --------------------------------------------------------------------------------------------
    static SQChar   s_Buffer[128];

public:

    /* --------------------------------------------------------------------------------------------
     * Import the constructors, destructors and assignment operators from the base class.
    */
    using RefType::Reference;

    /* --------------------------------------------------------------------------------------------
     * See if the referenced player instance is streamed for the specified player.
    */
    bool IsStreamedFor(const Reference < CPlayer > & player) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the class of the referenced player instance.
    */
    SQInt32 GetClass() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance has administrator privileges.
    */
    bool GetAdmin() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced player instance has administrator privileges.
    */
    void SetAdmin(bool toggle) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the ip address of the referenced player instance.
    */
    const SQChar * GetIP() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Kick the referenced player instance from the server.
    */
    void Kick() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Ban the referenced player instance from the server.
    */
    void Ban() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance is connected.
    */
    bool IsConnected() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance is spawned.
    */
    bool IsSpawned() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the key of the referenced player instance.
    */
    SQUnsignedInteger GetKey() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the world in which the referenced player instance exists.
    */
    SQInt32 GetWorld() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the world in which the referenced player instance exists.
    */
    void SetWorld(SQInt32 world) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the secondary world of the referenced player instance.
    */
    SQInt32 GetSecWorld() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the secondary world of the referenced player instance.
    */
    void SetSecWorld(SQInt32 world) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the unique world of the referenced player instance.
    */
    SQInt32 GetUniqueWorld() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance is compatible with the specified world.
    */
    bool IsWorldCompatible(SQInt32 world) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current state of the referenced player instance.
    */
    SQInt32 GetState() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the nick name of the referenced player instance.
    */
    const SQChar * GetName() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the nick name of the referenced player instance.
    */
    void SetName(const SQChar * name) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the team of the referenced player instance.
    */
    SQInt32 GetTeam() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the team of the referenced player instance.
    */
    void SetTeam(SQInt32 team) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the skin of the referenced player instance.
    */
    const CSkin & GetSkin() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the skin of the referenced player instance.
    */
    void SetSkin(const CSkin & skin) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the skin identifier of the referenced player instance.
    */
    SQInt32 GetSkinID() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the skin identifier of the referenced player instance.
    */
    void SetSkinID(SQInt32 skin) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the color of the referenced player instance.
    */
    const Color3 & GetColor() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the color of the referenced player instance.
    */
    void SetColor(const Color3 & color) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the color of the referenced player instance.
    */
    void SetColorEx(uint8 r, uint8 g, uint8 b) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Force the referenced player instance to spawn in the game.
    */
    void ForceSpawn() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Force the referenced player instance to select a class.
    */
    void ForceSelect() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the money amount of the referenced player instance.
    */
    SQInt32 GetMoney() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the money amount of the referenced player instance.
    */
    void SetMoney(SQInt32 amount) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Give a certain amount of money to the referenced player instance.
    */
    void GiveMoney(SQInt32 amount) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the score of the referenced player instance.
    */
    SQInt32 GetScore() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the score of the referenced player instance.
    */
    void SetScore(SQInt32 score) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection latency of the referenced player instance.
    */
    SQInt32 GetPing() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the frames per second of the referenced player instance.
    */
    SQFloat GetFPS() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance is typing.
    */
    bool IsTyping() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the unique user identifier of the referenced player instance.
    */
    const SQChar * GetUID() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the unique user identifier version 2 of the referenced player instance.
    */
    const SQChar * GetUID2() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current health of the referenced player instance.
    */
    SQFloat GetHealth() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the health of the referenced player instance.
    */
    void SetHealth(SQFloat amount) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current health of the referenced player instance.
    */
    SQFloat GetArmour() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the health of the referenced player instance.
    */
    void SetArmour(SQFloat amount) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the immunity flags of the referenced player instance.
    */
    SQInt32 GetImmunity() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the immunity flags of the referenced player instance.
    */
    void SetImmunity(SQInt32 flags) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position of the referenced player instance.
    */
    const Vector3 & GetPosition() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the position of the referenced player instance.
    */
    void SetPosition(const Vector3 & pos) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the position of the referenced player instance.
    */
    void SetPositionEx(SQFloat x, SQFloat y, SQFloat z) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the speed of the referenced player instance.
    */
    const Vector3 & GetSpeed() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the speed of the referenced player instance.
    */
    void SetSpeed(const Vector3 & vel) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the speed of the referenced player instance.
    */
    void SetSpeedEx(SQFloat x, SQFloat y, SQFloat z) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the speed of the referenced player instance.
    */
    void AddSpeed(const Vector3 & vel) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the speed of the referenced player instance.
    */
    void AddSpeedEx(SQFloat x, SQFloat y, SQFloat z) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the heading angle of the referenced player instance.
    */
    SQFloat GetHeading() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the heading angle of the referenced player instance.
    */
    void SetHeading(SQFloat angle) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the alpha of the referenced player instance.
    */
    SQInt32 GetAlpha() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the alpha of the referenced player instance.
    */
    void SetAlpha(SQInt32 alpha, SQInt32 fade) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the vehicle status of the referenced player instance.
    */
    SQInt32 GetVehicleStatus() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the occupied vehicle slot by the referenced player instance.
    */
    SQInt32 GetOccupiedSlot() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the vehicle in which the referenced player instance is embarked.
    */
    Reference < CVehicle > GetVehicle() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the vehicle identifier in which the referenced player instance is embarked.
    */
    SQInt32 GetVehicleID() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance can be controlled.
    */
    bool GetControllable() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced player instance can be controlled.
    */
    void SetControllable(bool toggle) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance can driveby.
    */
    bool GetDriveby() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced player instance can driveby.
    */
    void SetDriveby(bool toggle) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance has white scanlines.
    */
    bool GetWhiteScanlines() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced player instance has white scanlines.
    */
    void SetWhiteScanlines(bool toggle) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance has green scanlines.
    */
    bool GetGreenScanlines() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced player instance has green scanlines.
    */
    void SetGreenScanlines(bool toggle) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance has widescreen.
    */
    bool GetWidescreen() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced player instance has widescreen.
    */
    void SetWidescreen(bool toggle) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance displays markers.
    */
    bool GetShowMarkers() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced player instance displays markers.
    */
    void SetShowMarkers(bool toggle) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance has attacking privileges.
    */
    bool GetAttackPriv() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced player instance has attacking privileges.
    */
    void SetAttackPriv(bool toggle) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance has markers.
    */
    bool GetHasMarker() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced player instance has markers.
    */
    void SetHasMarker(bool toggle) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance has chat tags.
    */
    bool GetChatTags() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced player instance has chat tags.
    */
    void SetChatTags(bool toggle) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance is under drunk effects.
    */
    bool GetDrunkEffects() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced player instance is under drunk effects.
    */
    void SetDrunkEffects(bool toggle) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the weapon of the referenced player instance.
    */
    const CWeapon & GetWeapon() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the weapon of the referenced player instance.
    */
    void SetWeapon(const CWeapon & wep) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the weapon of the referenced player instance.
    */
    void SetWeaponEx(const CWeapon & wep, SQInt32 ammo) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the weapon identifier of the referenced player instance.
    */
    SQInt32 GetWeaponID() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the weapon of the referenced player instance.
    */
    void SetWeaponID(SQInt32 wep) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the weapon of the referenced player instance.
    */
    void SetWeaponIDEx(SQInt32 wep, SQInt32 ammo) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Give a weapon of the referenced player instance.
    */
    void GiveWeapon(const CWeapon & wep) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Give a weapon of the referenced player instance.
    */
    void GiveWeaponEx(const CWeapon & wep, SQInt32 ammo) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Give a weapon of the referenced player instance.
    */
    void GiveWeaponIDEx(SQInt32 wep, SQInt32 ammo) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Strip the referenced player instance of all weapons.
    */
    void StripWeapons() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the camera position of the referenced player instance.
    */
    void SetCameraPosition(const Vector3 & pos, const Vector3 & aim) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Restore the camera position of the referenced player instance.
    */
    void RestoreCamera() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance has camera locked.
    */
    bool IsCameraLocked() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the animation of the referenced player instance.
    */
    void SetAnimation(SQInt32 group, SQInt32 anim) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the wanted level of the referenced player instance.
    */
    SQInt32 GetWantedLevel() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Change the wanted level of the referenced player instance.
    */
    void SetWantedLevel(SQInt32 level) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the vehicle that the referenced player instance is standing on.
    */
    Reference < CVehicle > StandingOnVehicle() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the object that the referenced player instance is standing on.
    */
    Reference < CObject > StandingOnObject() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance is away.
    */
    bool IsAway() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the player that the referenced player instance is spectating.
    */
    Reference < CPlayer > Spectating() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Set the referenced player instance to spectate the specified player instance.
    */
    void Spectate(const Reference < CPlayer > & target) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance is burning.
    */
    bool IsBurning() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance is crouched.
    */
    bool IsCrouched() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current action of the referenced player instance.
    */
    SQInt32 GetAction() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the game keys of the referenced player instance.
    */
    SQInt32 GetGameKeys() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the aim position of the referenced player instance.
    */
    const Vector3 & GetAimPos() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the aim direction of the referenced player instance.
    */
    const Vector3 & GetAimDir() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Embark the referenced player instance into the specified vehicle instance.
    */
    void Embark(const Reference < CVehicle > & vehicle) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Embark the referenced player instance into the specified vehicle instance.
    */
    void Embark(const Reference < CVehicle > & vehicle, SQInt32 slot, bool allocate, bool warp) const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Disembark the referenced player instance from the currently embarked vehicle instance.
    */
    void Disembark() const noexcept;

    /* --------------------------------------------------------------------------------------------
     * Redirect the referenced player instance to the specified server.
    */
    bool Redirect(const SQChar * ip, SQUnsignedInteger port, const SQChar * nick, \
                    const SQChar * pass, const SQChar * user) noexcept;
};

} // Namespace:: SqMod

#endif // _ENTITY_PLAYER_HPP_