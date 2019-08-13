
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: VehicleCreated
*/
SqCore.On().VehicleCreated.Connect(function(/*SqVehicle*/ vehicle, /*int*/ header, /*object*/ payload) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: VehicleDestroyed
*/
SqCore.On().VehicleDestroyed.Connect(function(/*SqVehicle*/ vehicle, /*int*/ header, /*object*/ payload) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: VehicleCustom
*/
SqCore.On().VehicleCustom.Connect(function(/*SqVehicle*/ vehicle, /*int*/ header, /*object*/ payload) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: VehicleStream
*/
SqCore.On().VehicleStream.Connect(function(/*SqPlayer*/ client, /*SqVehicle*/ vehicle, /*bool*/ is_deleted) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: VehicleExplode
*/
SqCore.On().VehicleExplode.Connect(function(/*SqVehicle*/ vehicle) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: VehicleRespawn
*/
SqCore.On().VehicleRespawn.Connect(function(/*SqVehicle*/ vehicle) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: VehicleUpdate
*/
SqCore.On().VehicleUpdate.Connect(function(/*SqVehicle*/ vehicle, /*int*/ update_type) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: VehicleColor
*/
SqCore.On().VehicleColor.Connect(function(/*SqVehicle*/ vehicle, /*int*/ changed) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: VehicleHealth
*/
SqCore.On().VehicleHealth.Connect(function(/*SqVehicle*/ vehicle, /*float*/ old_health, /*float*/ new_health) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: VehiclePosition
*/
SqCore.On().VehiclePosition.Connect(function(/*SqVehicle*/ vehicle) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: VehicleRotation
*/
SqCore.On().VehicleRotation.Connect(function(/*SqVehicle*/ vehicle) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: VehicleOption
*/
SqCore.On().VehicleOption.Connect(function(/*SqVehicle*/ vehicle, /*int*/ option_id, /*bool*/ value, /*int*/ header, /*object*/ payload) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: VehicleWorld
*/
SqCore.On().VehicleWorld.Connect(function(/*SqVehicle*/ vehicle, /*int*/ old_world, /*int*/ new_world) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: VehicleImmunity
*/
SqCore.On().VehicleImmunity.Connect(function(/*SqVehicle*/ vehicle, /*int*/ old_immunity, /*int*/ new_immunity) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: VehiclePartStatus
*/
SqCore.On().VehiclePartStatus.Connect(function(/*SqVehicle*/ vehicle, /*int*/ part, /*int*/ old_status, /*int*/ new_status) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: VehicleTyreStatus
*/
SqCore.On().VehicleTyreStatus.Connect(function(/*SqVehicle*/ vehicle, /*int*/ tyre, /*int*/ old_status, /*int*/ new_status) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: VehicleDamageData
*/
SqCore.On().VehicleDamageData.Connect(function(/*SqVehicle*/ vehicle, /*int*/ old_data, /*int*/ new_data) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: VehicleRadio
*/
SqCore.On().VehicleRadio.Connect(function(/*SqVehicle*/ vehicle, /*int*/ old_radio, /*int*/ new_radio) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: VehicleHandlingRule
*/
SqCore.On().VehicleHandlingRule.Connect(function(/*SqVehicle*/ vehicle, /*int*/ rule, /*float*/ old_data, /*float*/ new_data) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: VehicleEnterArea
*/
SqCore.On().VehicleEnterArea.Connect(function(/*SqVehicle*/ vehicle, /*SqArea*/ area) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: VehicleLeaveArea
*/
SqCore.On().VehicleLeaveArea.Connect(function(/*SqVehicle*/ vehicle, /*SqArea*/ area) {

});