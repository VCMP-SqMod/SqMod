
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerCreated
*/
SqCore.On().PlayerCreated.Connect(function(/*SqPlayer*/ player, /*int*/ header, /*object*/ payload) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerDestroyed
*/
SqCore.On().PlayerDestroyed.Connect(function(/*SqPlayer*/ player, /*int*/ header, /*object*/ payload) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerCustom
*/
SqCore.On().PlayerCustom.Connect(function(/*SqPlayer*/ player, /*int*/ header, /*object*/ payload) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerStream
*/
SqCore.On().PlayerStream.Connect(function(/*SqPlayer*/ client, /*SqPlayer*/ player, /*bool*/ is_deleted) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerRequestClass
*/
SqCore.On().PlayerRequestClass.Connect(function(/*SqPlayer*/ player, offset) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerRequestSpawn
*/
SqCore.On().PlayerRequestSpawn.Connect(function(/*SqPlayer*/ player) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerSpawn
*/
SqCore.On().PlayerSpawn.Connect(function(/*SqPlayer*/ player) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerWasted
*/
SqCore.On().PlayerWasted.Connect(function(/*SqPlayer*/ player, /*int*/ reason) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerKilled
*/
SqCore.On().PlayerKilled.Connect(function(/*SqPlayer*/ player, /*SqPlayer*/ killer, /*int*/ reason, /*int*/ body_part, /*bool*/ team_kill) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerEmbarking
*/
SqCore.On().PlayerEmbarking.Connect(function(/*SqPlayer*/ player, /*SqVehicle*/ vehicle, /*int*/ slot) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerEmbarked
*/
SqCore.On().PlayerEmbarked.Connect(function(/*SqPlayer*/ player, /*SqVehicle*/ vehicle, /*int*/ slot) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerDisembark
*/
SqCore.On().PlayerDisembark.Connect(function(/*SqPlayer*/ player, /*SqVehicle*/ vehicle) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerRename
*/
SqCore.On().PlayerRename.Connect(function(/*SqPlayer*/ player, /*string*/ old_name, /*string*/ new_name) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerState
*/
SqCore.On().PlayerState.Connect(function(/*SqPlayer*/ player, /*int*/ old_state, /*int*/ new_state) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: StateNone
*/
SqCore.On().StateNone.Connect(function(/*SqPlayer*/ player, /*int*/ old_state) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: StateNormal
*/
SqCore.On().StateNormal.Connect(function(/*SqPlayer*/ player, /*int*/ old_state) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: StateAim
*/
SqCore.On().StateAim.Connect(function(/*SqPlayer*/ player, /*int*/ old_state) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: StateDriver
*/
SqCore.On().StateDriver.Connect(function(/*SqPlayer*/ player, /*int*/ old_state) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: StatePassenger
*/
SqCore.On().StatePassenger.Connect(function(/*SqPlayer*/ player, /*int*/ old_state) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: StateEnterDriver
*/
SqCore.On().StateEnterDriver.Connect(function(/*SqPlayer*/ player, /*int*/ old_state) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: StateEnterPassenger
*/
SqCore.On().StateEnterPassenger.Connect(function(/*SqPlayer*/ player, /*int*/ old_state) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: StateExit
*/
SqCore.On().StateExit.Connect(function(/*SqPlayer*/ player, /*int*/ old_state) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: StateUnspawned
*/
SqCore.On().StateUnspawned.Connect(function(/*SqPlayer*/ player, /*int*/ old_state) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerAction
*/
SqCore.On().PlayerAction.Connect(function(/*SqPlayer*/ player, /*int*/ old_action, /*int*/ new_action) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ActionNone
*/
SqCore.On().ActionNone.Connect(function(/*SqPlayer*/ player, /*int*/ old_action) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ActionNormal
*/
SqCore.On().ActionNormal.Connect(function(/*SqPlayer*/ player, /*int*/ old_action) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ActionAiming
*/
SqCore.On().ActionAiming.Connect(function(/*SqPlayer*/ player, /*int*/ old_action) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ActionShooting
*/
SqCore.On().ActionShooting.Connect(function(/*SqPlayer*/ player, /*int*/ old_action) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ActionJumping
*/
SqCore.On().ActionJumping.Connect(function(/*SqPlayer*/ player, /*int*/ old_action) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ActionLieDown
*/
SqCore.On().ActionLieDown.Connect(function(/*SqPlayer*/ player, /*int*/ old_action) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ActionGettingUp
*/
SqCore.On().ActionGettingUp.Connect(function(/*SqPlayer*/ player, /*int*/ old_action) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ActionJumpVehicle
*/
SqCore.On().ActionJumpVehicle.Connect(function(/*SqPlayer*/ player, /*int*/ old_action) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ActionDriving
*/
SqCore.On().ActionDriving.Connect(function(/*SqPlayer*/ player, /*int*/ old_action) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ActionDying
*/
SqCore.On().ActionDying.Connect(function(/*SqPlayer*/ player, /*int*/ old_action) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ActionWasted
*/
SqCore.On().ActionWasted.Connect(function(/*SqPlayer*/ player, /*int*/ old_action) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ActionEmbarking
*/
SqCore.On().ActionEmbarking.Connect(function(/*SqPlayer*/ player, /*int*/ old_action) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ActionDisembarking
*/
SqCore.On().ActionDisembarking.Connect(function(/*SqPlayer*/ player, /*int*/ old_action) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerBurning
*/
SqCore.On().PlayerBurning.Connect(function(/*SqPlayer*/ player, /*bool*/ is_on_fire) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerCrouching
*/
SqCore.On().PlayerCrouching.Connect(function(/*SqPlayer*/ player, /*bool*/ is_crouching) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerGameKeys
*/
SqCore.On().PlayerGameKeys.Connect(function(/*SqPlayer*/ player, /*int*/ old_keys, /*int*/ new_keys) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerStartTyping
*/
SqCore.On().PlayerStartTyping.Connect(function(/*SqPlayer*/ player) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerStopTyping
*/
SqCore.On().PlayerStopTyping.Connect(function(/*SqPlayer*/ player) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerAway
*/
SqCore.On().PlayerAway.Connect(function(/*SqPlayer*/ player, /*bool*/ is_away) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerMessage
*/
SqCore.On().PlayerMessage.Connect(function(/*SqPlayer*/ player, /*string*/ message) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerCommand
*/
SqCore.On().PlayerCommand.Connect(function(/*SqPlayer*/ player, /*string*/ message) {
    g_Cmd.Run(player, command); // Forward this command to the command manager
});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerPrivateMessage
*/
SqCore.On().PlayerPrivateMessage.Connect(function(/*SqPlayer*/ player, /*SqPlayer*/ target_player, /*string*/ message) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerKeyPress
*/
SqCore.On().PlayerKeyPress.Connect(function(/*SqPlayer*/ player, /*SqKeybind*/ bind) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerKeyRelease
*/
SqCore.On().PlayerKeyRelease.Connect(function(/*SqPlayer*/ player, /*SqKeybind*/ bind) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerSpectate
*/
SqCore.On().PlayerSpectate.Connect(function(/*SqPlayer*/ player, /*SqPlayer*/ target_player) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerUnspectate
*/
SqCore.On().PlayerUnspectate.Connect(function(/*SqPlayer*/ player) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerCrashreport
*/
SqCore.On().PlayerCrashreport.Connect(function(/*SqPlayer*/ player, /*string*/ report) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerModuleList
*/
SqCore.On().PlayerModuleList.Connect(function(/*SqPlayer*/ player, /*string*/ list) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ClientScriptData
*/
SqCore.On().ClientScriptData.Connect(function(/*SqPlayer*/ player, /*SqBuffer*/ data, /*int*/ size) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerUpdate
*/
SqCore.On().PlayerUpdate.Connect(function(/*SqPlayer*/ player, /*int*/ update_type) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerHealth
*/
SqCore.On().PlayerHealth.Connect(function(/*SqPlayer*/ player, /*float*/ old_health, /*float*/ new_health) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerArmour
*/
SqCore.On().PlayerArmour.Connect(function(/*SqPlayer*/ player, /*float*/ old_armour, /*float*/ new_armour) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerWeapon
*/
SqCore.On().PlayerWeapon.Connect(function(/*SqPlayer*/ player, /*int*/ old_weapon, /*int*/ new_weapon) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerHeading
*/
SqCore.On().PlayerHeading.Connect(function(/*SqPlayer*/ player, /*float*/ old_heading, /*float*/ new_heading) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerPosition
*/
SqCore.On().PlayerPosition.Connect(function(/*SqPlayer*/ player, /*int*/ header, /*object*/ payload) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerOption
*/
SqCore.On().PlayerOption.Connect(function(/*SqPlayer*/ player, /*int*/ option_id, /*bool*/ value, /*int*/ header, /*object*/ payload) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerAdmin
*/
SqCore.On().PlayerAdmin.Connect(function(/*SqPlayer*/ player, /*bool*/ old_status, /*bool*/ new_status) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerWorld
*/
SqCore.On().PlayerWorld.Connect(function(/*SqPlayer*/ player, /*int*/ old_world, /*int*/ new_world, /*bool*/ secondary) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerTeam
*/
SqCore.On().PlayerTeam.Connect(function(/*SqPlayer*/ player, /*int*/ old_team, /*int*/ new_team) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerSkin
*/
SqCore.On().PlayerSkin.Connect(function(/*SqPlayer*/ player, /*int*/ old_skin, /*int*/ new_skin) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerMoney
*/
SqCore.On().PlayerMoney.Connect(function(/*SqPlayer*/ player, /*int*/ old_money, /*int*/ new_money) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerScore
*/
SqCore.On().PlayerScore.Connect(function(/*SqPlayer*/ player, /*int*/ old_score, /*int*/ new_score) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerWantedLevel
*/
SqCore.On().PlayerWantedLevel.Connect(function(/*SqPlayer*/ player, /*int*/ old_level, /*int*/ new_level) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerImmunity
*/
SqCore.On().PlayerImmunity.Connect(function(/*SqPlayer*/ player, /*int*/ old_immunity, /*int*/ new_immunity) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerAlpha
*/
SqCore.On().PlayerAlpha.Connect(function(/*SqPlayer*/ player, /*int*/ old_alpha, /*int*/ new_alpha, /*int*/ fade) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerEnterArea
*/
SqCore.On().PlayerEnterArea.Connect(function(/*SqPlayer*/ player, /*SqArea*/ area) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PlayerLeaveArea
*/
SqCore.On().PlayerLeaveArea.Connect(function(/*SqPlayer*/ player, /*SqArea*/ area) {

});