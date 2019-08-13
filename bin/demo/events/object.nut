
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ObjectCreated
*/
SqCore.On().ObjectCreated.Connect(function(/*SqObject*/ object, /*int*/ header, /*object*/ payload) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ObjectDestroyed
*/
SqCore.On().ObjectDestroyed.Connect(function(/*SqObject*/ object, /*int*/ header, /*object*/ payload) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ObjectCustom
*/
SqCore.On().ObjectCustom.Connect(function(/*SqObject*/ object, /*int*/ header, /*object*/ payload) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ObjectStream
*/
SqCore.On().ObjectStream.Connect(function(/*SqPlayer*/ client, /*SqObject*/ object, /*bool*/ is_deleted) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ObjectShot
*/
SqCore.On().ObjectShot.Connect(function(/*SqObject*/ object, /*SqPlayer*/ player, /*int*/ weapon_id) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ObjectTouched
*/
SqCore.On().ObjectTouched.Connect(function(/*SqObject*/ object, /*SqPlayer*/ player) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ObjectWorld
*/
SqCore.On().ObjectWorld.Connect(function(/*SqObject*/ object, /*int*/ old_world, /*int*/ new_world) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ObjectAlpha
*/
SqCore.On().ObjectAlpha.Connect(function(/*SqObject*/ object, /*int*/ old_alpha, /*int*/ new_alpha, /*int*/ time) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ObjectReport
*/
SqCore.On().ObjectReport.Connect(function(/*SqObject*/ object, /*bool*/ old_status, /*bool*/ new_status, /*bool*/ touched) {

});