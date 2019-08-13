
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ServerStartup
*/
SqCore.On().ServerStartup.Connect(function(/*...*/) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ServerShutdown
*/
SqCore.On().ServerShutdown.Connect(function(/*...*/) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ServerFrame
*/
SqCore.On().ServerFrame.Connect(function(/*float*/ elapsed_time) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: IncomingConnection
*/
SqCore.On().IncomingConnection.Connect(function(/*string*/ player_name, /*int*/ name_buffer_size, /*string*/ user_password, /*string*/ ip_address) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: EntityPool
*/
SqCore.On().EntityPool.Connect(function(/*int*/ entity_type, /*Sq[Entity]*/ entity, /*bool*/ is_deleted) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: EntityStream
*/
SqCore.On().EntityStream.Connect(function(/*SqPlayer*/ player, /*Sq[Entity]*/ entity, /*int*/ entity_type, /*bool*/ is_deleted) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ServerOption
*/
SqCore.On().ServerOption.Connect(function(/*int*/ option, /*bool*/ value, /*int*/ header, /*object*/ payload) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ScriptReload
*/
SqCore.On().ScriptReload.Connect(function(/*int*/ header, /*object*/ payload) {

});
/* --------------------------------------------------------------------------------------------------------------------
 * Bind to global event: ScriptLoaded
*/
SqCore.On().ScriptLoaded.Connect(function(/*...*/) {

});
