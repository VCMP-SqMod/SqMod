// ------------------------------------------------------------------------------------------------
#include "Common.hpp"
#include "Manager.hpp"
#include "Connection.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <cstdlib>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Bind specific functions to certain server events.
*/
void BindCallbacks();

/* ------------------------------------------------------------------------------------------------
 * Undo changes made with BindCallbacks().
*/
void UnbindCallbacks();

/* ------------------------------------------------------------------------------------------------
 * Register the module API under the specified virtual machine.
*/
void RegisterAPI(HSQUIRRELVM vm);

/* ------------------------------------------------------------------------------------------------
 * Initialize the plug-in by obtaining the API provided by the host plug-in.
*/
void OnSquirrelInitialize()
{
    // Attempt to import the plug-in API exported by the host plug-in
    _SqMod = sq_api_import(_Func);
    // Did we failed to obtain the plug-in exports?
    if (!_SqMod)
    {
        OutputError("Failed to attach [%s] on host plug-in.", SQMG_NAME);
    }
    else
    {
        // Obtain the Squirrel API
        _SqAPI = _SqMod->GetSquirrelAPI();
        // Expand the Squirrel API into global functions
        sq_api_expand(_SqAPI);
    }
}

/* ------------------------------------------------------------------------------------------------
 * Load the module on the virtual machine provided by the host module.
*/
void OnSquirrelLoad()
{
    // Make sure that we have a valid plug-in API
    if (!_SqMod)
    {
        return; // Unable to proceed.
    }
    // Obtain the Squirrel API and VM
    _SqVM = _SqMod->GetSquirrelVM();
    // Make sure that a valid virtual machine exists
    if (!_SqVM)
    {
        return; // Unable to proceed.
    }
    // Set this as the default database
    DefaultVM::Set(_SqVM);
    // Register the module API
    RegisterAPI(_SqVM);
    // Notify about the current status
    OutputMessage("Registered: %s", SQMG_NAME);
}

/* ------------------------------------------------------------------------------------------------
 * The virtual machine is about to be terminated and script resources should be released.
*/
void OnSquirrelTerminate()
{
    OutputMessage("Terminating: %s", SQMG_NAME);
    // Release null objects just in case
    NullObject().Release();
    NullTable().Release();
    NullArray().Release();
    NullFunction().ReleaseGently();
}

/* ------------------------------------------------------------------------------------------------
 * The virtual machined was closed and all memory associated with it was released.
*/
void OnSquirrelReleased()
{
    // Release the current virtual machine, if any
    DefaultVM::Set(nullptr);
}

/* ------------------------------------------------------------------------------------------------
 * Validate the module API to make sure we don't run into issues.
*/
bool CheckAPIVer(CCStr ver)
{
    // Obtain the numeric representation of the API version
    const LongI vernum = std::strtol(ver, nullptr, 10);
    // Check against version mismatch
    if (vernum == SQMOD_API_VER)
    {
        return true;
    }
    // Log the incident
    OutputError("API version mismatch on %s", SQMG_NAME);
    OutputMessage("=> Requested: %ld Have: %ld", vernum, SQMOD_API_VER);
    // Invoker should not attempt to communicate through the module API
    return false;
}

/* ------------------------------------------------------------------------------------------------
 * React to command sent by other plug-ins.
*/
static uint8_t OnPluginCommand(uint32_t command_identifier, CCStr message)
{
    switch(command_identifier)
    {
        case SQMOD_INITIALIZE_CMD:
            if (CheckAPIVer(message))
            {
                OnSquirrelInitialize();
            }
        break;
        case SQMOD_LOAD_CMD:
            OnSquirrelLoad();
        break;
        case SQMOD_TERMINATE_CMD:
            OnSquirrelTerminate();
        break;
        case SQMOD_RELEASED_CMD:
            OnSquirrelReleased();
        break;
        default: break;
    }
    return 1;
}

/* ------------------------------------------------------------------------------------------------
 * The server was initialized and this plug-in was loaded successfully.
*/
static uint8_t OnServerInitialise()
{
    return 1;
}

static void OnServerShutdown(void)
{
    // The server may still send callbacks
    UnbindCallbacks();
}

// ------------------------------------------------------------------------------------------------
void BindCallbacks()
{
    _Clbk->OnServerInitialise       = OnServerInitialise;
    _Clbk->OnServerShutdown         = OnServerShutdown;
    _Clbk->OnPluginCommand          = OnPluginCommand;
}

// ------------------------------------------------------------------------------------------------
void UnbindCallbacks()
{
    _Clbk->OnServerInitialise       = nullptr;
    _Clbk->OnServerShutdown         = nullptr;
    _Clbk->OnPluginCommand          = nullptr;
}

// ------------------------------------------------------------------------------------------------
void RegisterAPI(HSQUIRRELVM vm)
{
    Table mgns(vm);

    mgns.Bind(_SC("Manager"), Class< Manager >(vm, _SC("SqMgManager"))
        // Constructors
        .Ctor()
        .Ctor< const Manager & >()
        // Core Meta-methods
        .Func(_SC("_cmp"), &Manager::Cmp)
        .SquirrelFunc(_SC("_typename"), &Manager::Typename)
        .Func(_SC("_tostring"), &Manager::ToString)
        // Properties
        //.Prop(_SC("Valid"), &Manager::IsValid)
        // Member Methods
        //.Func(_SC("Check"), &Manager::Check)
        // Overloaded Methods
        .Overload< Connection (Manager::*)(CSStr) const >(_SC("Bind"), &Manager::Bind)
        .Overload< Connection (Manager::*)(CSStr, Uint32) const >(_SC("Bind"), &Manager::Bind)
        .Overload< Connection (Manager::*)(CSStr) const >(_SC("Connect"), &Manager::Connect)
        .Overload< Connection (Manager::*)(CSStr, Uint32) const >(_SC("Connect"), &Manager::Connect)
    );

    mgns.Bind(_SC("Connection"), Class< Connection >(vm, _SC("SqMgConnection"))
        // Constructors
        .Ctor()
        .Ctor< const Connection & >()
        // Core Meta-methods
        .Func(_SC("_cmp"), &Connection::Cmp)
        .SquirrelFunc(_SC("_typename"), &Connection::Typename)
        .Func(_SC("_tostring"), &Connection::ToString)
        // Properties
        //.Prop(_SC("Valid"), &Connection::IsValid)
        // Member Methods
        //.Func(_SC("Check"), &Connection::Check)
    );

    RootTable(vm).Bind(_SC("SqMg"), mgns);

    ConstTable(vm).Enum(_SC("EMgF"), Enumeration(vm)
        .Const(_SC("LISTENING"),                MG_F_LISTENING)
        .Const(_SC("UDP"),                      MG_F_UDP)
        .Const(_SC("RESOLVING"),                MG_F_RESOLVING)
        .Const(_SC("CONNECTING"),               MG_F_CONNECTING)
        .Const(_SC("SSL_HANDSHAKE_DONE"),       MG_F_SSL_HANDSHAKE_DONE)
        .Const(_SC("WANT_READ"),                MG_F_WANT_READ)
        .Const(_SC("WANT_WRITE"),               MG_F_WANT_WRITE)
        .Const(_SC("IS_WEBSOCKET"),             MG_F_IS_WEBSOCKET)
        .Const(_SC("SEND_AND_CLOSE"),           MG_F_SEND_AND_CLOSE)
        .Const(_SC("CLOSE_IMMEDIATELY"),        MG_F_CLOSE_IMMEDIATELY)
        .Const(_SC("WEBSOCKET_NO_DEFRAG"),      MG_F_WEBSOCKET_NO_DEFRAG)
        .Const(_SC("DELETE_CHUNK"),             MG_F_DELETE_CHUNK)
        .Const(_SC("USER_1"),                   MG_F_USER_1)
        .Const(_SC("USER_2"),                   MG_F_USER_2)
        .Const(_SC("USER_3"),                   MG_F_USER_3)
        .Const(_SC("USER_4"),                   MG_F_USER_4)
        .Const(_SC("USER_5"),                   MG_F_USER_5)
        .Const(_SC("USER_6"),                   MG_F_USER_6)
    );

    ConstTable(vm).Enum(_SC("MgEv"), Enumeration(vm)
        .Const(_SC("UNKNOWN"),                              static_cast< Int32 >(MGEV_UNKNOWN))
        .Const(_SC("POLL"),                                 static_cast< Int32 >(MGCE_POLL))
        .Const(_SC("ACCEPT"),                               static_cast< Int32 >(MGCE_ACCEPT))
        .Const(_SC("CONNECT"),                              static_cast< Int32 >(MGCE_CONNECT))
        .Const(_SC("RECV"),                                 static_cast< Int32 >(MGCE_RECV))
        .Const(_SC("SEND"),                                 static_cast< Int32 >(MGCE_SEND))
        .Const(_SC("CLOSE"),                                static_cast< Int32 >(MGCE_CLOSE))
        .Const(_SC("TIMER"),                                static_cast< Int32 >(MGCE_TIMER))
        .Const(_SC("HTTP_REQUEST"),                         static_cast< Int32 >(MGCE_HTTP_REQUEST))
        .Const(_SC("HTTP_REPLY"),                           static_cast< Int32 >(MGCE_HTTP_REPLY))
        .Const(_SC("HTTP_CHUNK"),                           static_cast< Int32 >(MGCE_HTTP_CHUNK))
        .Const(_SC("SSI_CALL"),                             static_cast< Int32 >(MGCE_SSI_CALL))
        .Const(_SC("WEBSOCKET_HANDSHAKE_REQUEST"),          static_cast< Int32 >(MGCE_WEBSOCKET_HANDSHAKE_REQUEST))
        .Const(_SC("WEBSOCKET_HANDSHAKE_DONE"),             static_cast< Int32 >(MGCE_WEBSOCKET_HANDSHAKE_DONE))
        .Const(_SC("WEBSOCKET_FRAME"),                      static_cast< Int32 >(MGCE_WEBSOCKET_FRAME))
        .Const(_SC("WEBSOCKET_CONTROL_FRAME"),              static_cast< Int32 >(MGCE_WEBSOCKET_CONTROL_FRAME))
        .Const(_SC("HTTP_MULTIPART_REQUEST"),               static_cast< Int32 >(MGCE_HTTP_MULTIPART_REQUEST))
        .Const(_SC("HTTP_PART_BEGIN"),                      static_cast< Int32 >(MGCE_HTTP_PART_BEGIN))
        .Const(_SC("HTTP_PART_DATA"),                       static_cast< Int32 >(MGCE_HTTP_PART_DATA))
        .Const(_SC("HTTP_PART_END"),                        static_cast< Int32 >(MGCE_HTTP_PART_END))
        .Const(_SC("MQTT_CONNECT"),                         static_cast< Int32 >(MGCE_MQTT_CONNECT))
        .Const(_SC("MQTT_CONNACK"),                         static_cast< Int32 >(MGCE_MQTT_CONNACK))
        .Const(_SC("MQTT_PUBLISH"),                         static_cast< Int32 >(MGCE_MQTT_PUBLISH))
        .Const(_SC("MQTT_PUBACK"),                          static_cast< Int32 >(MGCE_MQTT_PUBACK))
        .Const(_SC("MQTT_PUBREC"),                          static_cast< Int32 >(MGCE_MQTT_PUBREC))
        .Const(_SC("MQTT_PUBREL"),                          static_cast< Int32 >(MGCE_MQTT_PUBREL))
        .Const(_SC("MQTT_PUBCOMP"),                         static_cast< Int32 >(MGCE_MQTT_PUBCOMP))
        .Const(_SC("MQTT_SUBSCRIBE"),                       static_cast< Int32 >(MGCE_MQTT_SUBSCRIBE))
        .Const(_SC("MQTT_SUBACK"),                          static_cast< Int32 >(MGCE_MQTT_SUBACK))
        .Const(_SC("MQTT_UNSUBSCRIBE"),                     static_cast< Int32 >(MGCE_MQTT_UNSUBSCRIBE))
        .Const(_SC("MQTT_UNSUBACK"),                        static_cast< Int32 >(MGCE_MQTT_UNSUBACK))
        .Const(_SC("MQTT_PINGREQ"),                         static_cast< Int32 >(MGCE_MQTT_PINGREQ))
        .Const(_SC("MQTT_PINGRESP"),                        static_cast< Int32 >(MGCE_MQTT_PINGRESP))
        .Const(_SC("MQTT_DISCONNECT"),                      static_cast< Int32 >(MGCE_MQTT_DISCONNECT))
        .Const(_SC("MQTT_CONNACK_ACCEPTED"),                static_cast< Int32 >(MGCE_MQTT_CONNACK_ACCEPTED))
        .Const(_SC("MQTT_CONNACK_UNACCEPTABLE_VERSION"),    static_cast< Int32 >(MGCE_MQTT_CONNACK_UNACCEPTABLE_VERSION))
        .Const(_SC("MQTT_CONNACK_IDENTIFIER_REJECTED"),     static_cast< Int32 >(MGCE_MQTT_CONNACK_IDENTIFIER_REJECTED))
        .Const(_SC("MQTT_CONNACK_SERVER_UNAVAILABLE"),      static_cast< Int32 >(MGCE_MQTT_CONNACK_SERVER_UNAVAILABLE))
        .Const(_SC("MQTT_CONNACK_BAD_AUTH"),                static_cast< Int32 >(MGCE_MQTT_CONNACK_BAD_AUTH))
        .Const(_SC("MQTT_CONNACK_NOT_AUTHORIZED"),          static_cast< Int32 >(MGCE_MQTT_CONNACK_NOT_AUTHORIZED))
        .Const(_SC("COAP_CON"),                             static_cast< Int32 >(MGCE_COAP_CON))
        .Const(_SC("COAP_NOC"),                             static_cast< Int32 >(MGCE_COAP_NOC))
        .Const(_SC("COAP_ACK"),                             static_cast< Int32 >(MGCE_COAP_ACK))
        .Const(_SC("COAP_RST"),                             static_cast< Int32 >(MGCE_COAP_RST))
        .Const(_SC("MAX"),                                  static_cast< Int32 >(MGCE_MAX))
    );              
}

} // Namespace:: SqMod

// ------------------------------------------------------------------------------------------------
SQMOD_API_EXPORT unsigned int VcmpPluginInit(PluginFuncs * functions, PluginCallbacks * callbacks, PluginInfo * info)
{
    using namespace SqMod;
    // Output plug-in header
    std::puts("");
    OutputMessage("--------------------------------------------------------------------");
    OutputMessage("Plug-in: %s", SQMG_NAME);
    OutputMessage("Author: %s", SQMG_AUTHOR);
    OutputMessage("Legal: %s", SQMG_COPYRIGHT);
    OutputMessage("--------------------------------------------------------------------");
    std::puts("");
    // Attempt to find the host plug-in ID
    const int host_plugin_id = functions->FindPlugin(SQMOD_HOST_NAME);
    // See if our plug-in was loaded after the host plug-in
    if (host_plugin_id < 0)
    {
        OutputError("%s could find the host plug-in", SQMG_NAME);
        // Don't load!
        return SQMOD_FAILURE;
    }
    // Should never reach this point but just in case
    else if (static_cast< Uint32 >(host_plugin_id) > info->pluginId)
    {
        OutputError("%s loaded after the host plug-in", SQMG_NAME);
        // Don't load!
        return SQMOD_FAILURE;
    }
    // Store server proxies
    _Func = functions;
    _Clbk = callbacks;
    _Info = info;
    // Assign plug-in version
    _Info->pluginVersion = SQMG_VERSION;
    _Info->apiMajorVersion = PLUGIN_API_MAJOR;
    _Info->apiMinorVersion = PLUGIN_API_MINOR;
    // Assign the plug-in name
    std::snprintf(_Info->name, sizeof(_Info->name), "%s", SQMG_HOST_NAME);
    // Bind callbacks
    BindCallbacks();
    // Notify that the plug-in was successfully loaded
    OutputMessage("Successfully loaded %s", SQMG_NAME);
    // Dummy spacing
    std::puts("");
    // Done!
    return SQMOD_SUCCESS;
}
