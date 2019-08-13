
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PickupCreated
*/
SqCore.On().PickupCreated.Connect(function(/*SqPickup*/ pickup, /*int*/ header, /*object*/ payload) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PickupDestroyed
*/
SqCore.On().PickupDestroyed.Connect(function(/*SqPickup*/ pickup, /*int*/ header, /*object*/ payload) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PickupCustom
*/
SqCore.On().PickupCustom.Connect(function(/*SqPickup*/ pickup, /*int*/ header, /*object*/ payload) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PickupStream
*/
SqCore.On().PickupStream.Connect(function(/*SqPlayer*/ client, /*SqPickup*/ pickup, /*bool*/ is_deleted) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PickupClaimed
*/
SqCore.On().PickupClaimed.Connect(function(/*SqPickup*/ pickup, /*SqPlayer*/ player) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PickupCollected
*/
SqCore.On().PickupCollected.Connect(function(/*SqPickup*/ pickup, /*SqPlayer*/ player) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PickupRespawn
*/
SqCore.On().PickupRespawn.Connect(function(/*SqPickup*/ pickup) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PickupWorld
*/
SqCore.On().PickupWorld.Connect(function(/*SqPickup*/ pickup, /*int*/ old_world, /*int*/ new_world) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PickupAlpha
*/
SqCore.On().PickupAlpha.Connect(function(/*SqPickup*/ pickup, /*int*/ old_alpha, /*int*/ new_alpha) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PickupAutomatic
*/
SqCore.On().PickupAutomatic.Connect(function(/*SqPickup*/ pickup, /*bool*/ old_status, /*bool*/ new_status) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PickupAutoTimer
*/
SqCore.On().PickupAutoTimer.Connect(function(/*SqPickup*/ pickup, /*int*/ old_timer, /*int*/ new_timer) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: PickupOption
*/
SqCore.On().PickupOption.Connect(function(/*SqPickup*/ pickup, /*int*/ option_id, /*bool*/ value, /*int*/ header, /*object*/ payload) {

});