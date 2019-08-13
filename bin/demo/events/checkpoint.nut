
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: CheckpointCreated
*/
SqCore.On().CheckpointCreated.Connect(function(/*SqCheckpoint*/ checkpoint, /*int*/ header, /*object*/ payload) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: CheckpointDestroyed
*/
SqCore.On().CheckpointDestroyed.Connect(function(/*SqCheckpoint*/ checkpoint, /*int*/ header, /*object*/ payload) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: CheckpointCustom
*/
SqCore.On().CheckpointCustom.Connect(function(/*SqCheckpoint*/ checkpoint, /*int*/ header, /*object*/ payload) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: CheckpointStream
*/
SqCore.On().CheckpointStream.Connect(function(/*SqPlayer*/ client, /*SqCheckpoint*/ checkpoint, /*bool*/ is_deleted) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: CheckpointEntered
*/
SqCore.On().CheckpointEntered.Connect(function(/*SqCheckpoint*/ checkpoint, /*SqPlayer*/ player) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: CheckpointExited
*/
SqCore.On().CheckpointExited.Connect(function(/*SqCheckpoint*/ checkpoint, /*SqPlayer*/ player) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: CheckpointWorld
*/
SqCore.On().CheckpointWorld.Connect(function(/*SqCheckpoint*/ checkpoint, /*int*/ old_world, /*int*/ new_world) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: CheckpointRadius
*/
SqCore.On().CheckpointRadius.Connect(function(/*SqCheckpoint*/ checkpoint, /*float*/ old_radius, /*float*/ new_radius) {

});