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
     * Construct a reference from a base reference.
    */
    CPlayer(const Reference< CPlayer > & o);

    /* --------------------------------------------------------------------------------------------
     * See if the referenced player instance is streamed for the specified player.
    */
    bool IsStreamedFor(const Reference < CPlayer > & player) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the class of the referenced player instance.
    */
    SQInt32 GetClass() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance has administrator privileges.
    */
    bool GetAdmin() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced player instance has administrator privileges.
    */
    void SetAdmin(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the ip address of the referenced player instance.
    */
    const SQChar * GetIP() const;

    /* --------------------------------------------------------------------------------------------
     * Kick the referenced player instance from the server.
    */
    void Kick() const;

    /* --------------------------------------------------------------------------------------------
     * Ban the referenced player instance from the server.
    */
    void Ban() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance is connected.
    */
    bool IsConnected() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance is spawned.
    */
    bool IsSpawned() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the key of the referenced player instance.
    */
    SQUnsignedInteger GetKey() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the world in which the referenced player instance exists.
    */
    SQInt32 GetWorld() const;

    /* --------------------------------------------------------------------------------------------
     * Change the world in which the referenced player instance exists.
    */
    void SetWorld(SQInt32 world) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the secondary world of the referenced player instance.
    */
    SQInt32 GetSecWorld() const;

    /* --------------------------------------------------------------------------------------------
     * Change the secondary world of the referenced player instance.
    */
    void SetSecWorld(SQInt32 world) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the unique world of the referenced player instance.
    */
    SQInt32 GetUniqueWorld() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance is compatible with the specified world.
    */
    bool IsWorldCompatible(SQInt32 world) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current state of the referenced player instance.
    */
    SQInt32 GetState() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the nick name of the referenced player instance.
    */
    const SQChar * GetName() const;

    /* --------------------------------------------------------------------------------------------
     * Change the nick name of the referenced player instance.
    */
    void SetName(const SQChar * name) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the team of the referenced player instance.
    */
    SQInt32 GetTeam() const;

    /* --------------------------------------------------------------------------------------------
     * Change the team of the referenced player instance.
    */
    void SetTeam(SQInt32 team) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the skin of the referenced player instance.
    */
    const CSkin & GetSkin() const;

    /* --------------------------------------------------------------------------------------------
     * Change the skin of the referenced player instance.
    */
    void SetSkin(const CSkin & skin) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the skin identifier of the referenced player instance.
    */
    SQInt32 GetSkinID() const;

    /* --------------------------------------------------------------------------------------------
     * Change the skin identifier of the referenced player instance.
    */
    void SetSkinID(SQInt32 skin) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the color of the referenced player instance.
    */
    const Color3 & GetColor() const;

    /* --------------------------------------------------------------------------------------------
     * Change the color of the referenced player instance.
    */
    void SetColor(const Color3 & color) const;

    /* --------------------------------------------------------------------------------------------
     * Change the color of the referenced player instance.
    */
    void SetColorEx(uint8 r, uint8 g, uint8 b) const;

    /* --------------------------------------------------------------------------------------------
     * Force the referenced player instance to spawn in the game.
    */
    void ForceSpawn() const;

    /* --------------------------------------------------------------------------------------------
     * Force the referenced player instance to select a class.
    */
    void ForceSelect() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the money amount of the referenced player instance.
    */
    SQInt32 GetMoney() const;

    /* --------------------------------------------------------------------------------------------
     * Change the money amount of the referenced player instance.
    */
    void SetMoney(SQInt32 amount) const;

    /* --------------------------------------------------------------------------------------------
     * Give a certain amount of money to the referenced player instance.
    */
    void GiveMoney(SQInt32 amount) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the score of the referenced player instance.
    */
    SQInt32 GetScore() const;

    /* --------------------------------------------------------------------------------------------
     * Change the score of the referenced player instance.
    */
    void SetScore(SQInt32 score) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the connection latency of the referenced player instance.
    */
    SQInt32 GetPing() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the frames per second of the referenced player instance.
    */
    SQFloat GetFPS() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance is typing.
    */
    bool IsTyping() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the unique user identifier of the referenced player instance.
    */
    const SQChar * GetUID() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the unique user identifier version 2 of the referenced player instance.
    */
    const SQChar * GetUID2() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current health of the referenced player instance.
    */
    SQFloat GetHealth() const;

    /* --------------------------------------------------------------------------------------------
     * Change the health of the referenced player instance.
    */
    void SetHealth(SQFloat amount) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current health of the referenced player instance.
    */
    SQFloat GetArmour() const;

    /* --------------------------------------------------------------------------------------------
     * Change the health of the referenced player instance.
    */
    void SetArmour(SQFloat amount) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the immunity flags of the referenced player instance.
    */
    SQInt32 GetImmunity() const;

    /* --------------------------------------------------------------------------------------------
     * Change the immunity flags of the referenced player instance.
    */
    void SetImmunity(SQInt32 flags) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the position of the referenced player instance.
    */
    const Vector3 & GetPosition() const;

    /* --------------------------------------------------------------------------------------------
     * Change the position of the referenced player instance.
    */
    void SetPosition(const Vector3 & pos) const;

    /* --------------------------------------------------------------------------------------------
     * Change the position of the referenced player instance.
    */
    void SetPositionEx(SQFloat x, SQFloat y, SQFloat z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the speed of the referenced player instance.
    */
    const Vector3 & GetSpeed() const;

    /* --------------------------------------------------------------------------------------------
     * Change the speed of the referenced player instance.
    */
    void SetSpeed(const Vector3 & vel) const;

    /* --------------------------------------------------------------------------------------------
     * Change the speed of the referenced player instance.
    */
    void SetSpeedEx(SQFloat x, SQFloat y, SQFloat z) const;

    /* --------------------------------------------------------------------------------------------
     * Change the speed of the referenced player instance.
    */
    void AddSpeed(const Vector3 & vel) const;

    /* --------------------------------------------------------------------------------------------
     * Change the speed of the referenced player instance.
    */
    void AddSpeedEx(SQFloat x, SQFloat y, SQFloat z) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the heading angle of the referenced player instance.
    */
    SQFloat GetHeading() const;

    /* --------------------------------------------------------------------------------------------
     * Change the heading angle of the referenced player instance.
    */
    void SetHeading(SQFloat angle) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the alpha of the referenced player instance.
    */
    SQInt32 GetAlpha() const;

    /* --------------------------------------------------------------------------------------------
     * Change the alpha of the referenced player instance.
    */
    void SetAlpha(SQInt32 alpha, SQInt32 fade) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the vehicle status of the referenced player instance.
    */
    SQInt32 GetVehicleStatus() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the occupied vehicle slot by the referenced player instance.
    */
    SQInt32 GetOccupiedSlot() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the vehicle in which the referenced player instance is embarked.
    */
    Reference < CVehicle > GetVehicle() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the vehicle identifier in which the referenced player instance is embarked.
    */
    SQInt32 GetVehicleID() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance can be controlled.
    */
    bool GetControllable() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced player instance can be controlled.
    */
    void SetControllable(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance can driveby.
    */
    bool GetDriveby() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced player instance can driveby.
    */
    void SetDriveby(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance has white scanlines.
    */
    bool GetWhiteScanlines() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced player instance has white scanlines.
    */
    void SetWhiteScanlines(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance has green scanlines.
    */
    bool GetGreenScanlines() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced player instance has green scanlines.
    */
    void SetGreenScanlines(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance has widescreen.
    */
    bool GetWidescreen() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced player instance has widescreen.
    */
    void SetWidescreen(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance displays markers.
    */
    bool GetShowMarkers() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced player instance displays markers.
    */
    void SetShowMarkers(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance has attacking privileges.
    */
    bool GetAttackPriv() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced player instance has attacking privileges.
    */
    void SetAttackPriv(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance has markers.
    */
    bool GetHasMarker() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced player instance has markers.
    */
    void SetHasMarker(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance has chat tags.
    */
    bool GetChatTags() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced player instance has chat tags.
    */
    void SetChatTags(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance is under drunk effects.
    */
    bool GetDrunkEffects() const;

    /* --------------------------------------------------------------------------------------------
     * Set whether the referenced player instance is under drunk effects.
    */
    void SetDrunkEffects(bool toggle) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the weapon of the referenced player instance.
    */
    const CWeapon & GetWeapon() const;

    /* --------------------------------------------------------------------------------------------
     * Change the weapon of the referenced player instance.
    */
    void SetWeapon(const CWeapon & wep) const;

    /* --------------------------------------------------------------------------------------------
     * Change the weapon of the referenced player instance.
    */
    void SetWeaponEx(const CWeapon & wep, SQInt32 ammo) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the weapon identifier of the referenced player instance.
    */
    SQInt32 GetWeaponID() const;

    /* --------------------------------------------------------------------------------------------
     * Change the weapon of the referenced player instance.
    */
    void SetWeaponID(SQInt32 wep) const;

    /* --------------------------------------------------------------------------------------------
     * Change the weapon of the referenced player instance.
    */
    void SetWeaponIDEx(SQInt32 wep, SQInt32 ammo) const;

    /* --------------------------------------------------------------------------------------------
     * Give a weapon of the referenced player instance.
    */
    void GiveWeapon(const CWeapon & wep) const;

    /* --------------------------------------------------------------------------------------------
     * Give a weapon of the referenced player instance.
    */
    void GiveWeaponEx(const CWeapon & wep, SQInt32 ammo) const;

    /* --------------------------------------------------------------------------------------------
     * Give a weapon of the referenced player instance.
    */
    void GiveWeaponIDEx(SQInt32 wep, SQInt32 ammo) const;

    /* --------------------------------------------------------------------------------------------
     * Strip the referenced player instance of all weapons.
    */
    void StripWeapons() const;

    /* --------------------------------------------------------------------------------------------
     * Change the camera position of the referenced player instance.
    */
    void SetCameraPosition(const Vector3 & pos, const Vector3 & aim) const;

    /* --------------------------------------------------------------------------------------------
     * Restore the camera position of the referenced player instance.
    */
    void RestoreCamera() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance has camera locked.
    */
    bool IsCameraLocked() const;

    /* --------------------------------------------------------------------------------------------
     * Change the animation of the referenced player instance.
    */
    void SetAnimation(SQInt32 group, SQInt32 anim) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the wanted level of the referenced player instance.
    */
    SQInt32 GetWantedLevel() const;

    /* --------------------------------------------------------------------------------------------
     * Change the wanted level of the referenced player instance.
    */
    void SetWantedLevel(SQInt32 level) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the vehicle that the referenced player instance is standing on.
    */
    Reference < CVehicle > StandingOnVehicle() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the object that the referenced player instance is standing on.
    */
    Reference < CObject > StandingOnObject() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance is away.
    */
    bool IsAway() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the player that the referenced player instance is spectating.
    */
    Reference < CPlayer > Spectating() const;

    /* --------------------------------------------------------------------------------------------
     * Set the referenced player instance to spectate the specified player instance.
    */
    void Spectate(const Reference < CPlayer > & target) const;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance is burning.
    */
    bool IsBurning() const;

    /* --------------------------------------------------------------------------------------------
     * See whether the referenced player instance is crouched.
    */
    bool IsCrouched() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the current action of the referenced player instance.
    */
    SQInt32 GetAction() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the game keys of the referenced player instance.
    */
    SQInt32 GetGameKeys() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the aim position of the referenced player instance.
    */
    const Vector3 & GetAimPos() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the aim direction of the referenced player instance.
    */
    const Vector3 & GetAimDir() const;

    /* --------------------------------------------------------------------------------------------
     * Embark the referenced player instance into the specified vehicle instance.
    */
    void Embark(const Reference < CVehicle > & vehicle) const;

    /* --------------------------------------------------------------------------------------------
     * Embark the referenced player instance into the specified vehicle instance.
    */
    void Embark(const Reference < CVehicle > & vehicle, SQInt32 slot, bool allocate, bool warp) const;

    /* --------------------------------------------------------------------------------------------
     * Disembark the referenced player instance from the currently embarked vehicle instance.
    */
    void Disembark() const;

    /* --------------------------------------------------------------------------------------------
     * Redirect the referenced player instance to the specified server.
    */
    bool Redirect(const SQChar * ip, SQUnsignedInteger port, const SQChar * nick, \
                    const SQChar * pass, const SQChar * user);
};

} // Namespace:: SqMod

#endif // _ENTITY_PLAYER_HPP_