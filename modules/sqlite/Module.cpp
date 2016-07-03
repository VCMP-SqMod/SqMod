// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

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
        OutputError("Failed to attach [%s] on host plug-in.", SQSQLITE_NAME);
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
        return; // Unable to proceed!
    }
    // Obtain the Squirrel API and VM
    _SqVM = _SqMod->GetSquirrelVM();
    // Make sure that a valid virtual machine exists
    if (!_SqVM)
    {
        return; // Unable to proceed!
    }
    // Set this as the default database
    DefaultVM::Set(_SqVM);
    // Prevent common null objects from using dead virtual machines
    NullArray() = Array();
    NullTable() = Table();
    NullObject() = Object();
    NullFunction() = Function();
    // Register the module API
    RegisterAPI(_SqVM);
    // Notify about the current status
    OutputMessage("Registered: %s", SQSQLITE_NAME);
}

/* ------------------------------------------------------------------------------------------------
 * The virtual machine is about to be terminated and script resources should be released.
*/
void OnSquirrelTerminate()
{
    OutputMessage("Terminating: %s", SQSQLITE_NAME);
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
    OutputError("API version mismatch on %s", SQSQLITE_NAME);
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
extern void Register_Constants(Table & sqlns);
extern void Register_Common(Table & sqlns);
extern void Register_Connection(Table & sqlns);
extern void Register_Statement(Table & sqlns);
extern void Register_Column(Table & sqlns);
extern void Register_Transaction(Table & sqlns);

// ------------------------------------------------------------------------------------------------
void RegisterAPI(HSQUIRRELVM vm)
{
    Table sqlns(vm);

    Register_Constants(sqlns);
    Register_Common(sqlns);
    Register_Connection(sqlns);
    Register_Statement(sqlns);
    Register_Column(sqlns);
    Register_Transaction(sqlns);

    RootTable(vm).Bind(_SC("SQLite"), sqlns);
}

} // Namespace:: SqMod

// ------------------------------------------------------------------------------------------------
SQMOD_API_EXPORT unsigned int VcmpPluginInit(PluginFuncs * functions, PluginCallbacks * callbacks, PluginInfo * info)
{
    using namespace SqMod;
    // Output plug-in header
    puts("");
    OutputMessage("--------------------------------------------------------------------");
    OutputMessage("Plug-in: %s", SQSQLITE_NAME);
    OutputMessage("Author: %s", SQSQLITE_AUTHOR);
    OutputMessage("Legal: %s", SQSQLITE_COPYRIGHT);
    OutputMessage("--------------------------------------------------------------------");
    puts("");
    // Attempt to find the host plug-in ID
    const int host_plugin_id = functions->FindPlugin(SQMOD_HOST_NAME);
    // See if our plug-in was loaded after the host plug-in
    if (host_plugin_id < 0)
    {
        OutputError("%s could find the host plug-in", SQSQLITE_NAME);
        // Don't load!
        return SQMOD_FAILURE;
    }
    // Should never reach this point but just in case
    else if (static_cast< Uint32 >(host_plugin_id) > info->pluginId)
    {
        OutputError("%s loaded after the host plug-in", SQSQLITE_NAME);
        // Don't load!
        return SQMOD_FAILURE;
    }
    // Store server proxies
    _Func = functions;
    _Clbk = callbacks;
    _Info = info;
    // Assign plug-in version
    _Info->pluginVersion = SQSQLITE_VERSION;
    _Info->apiMajorVersion = PLUGIN_API_MAJOR;
    _Info->apiMinorVersion = PLUGIN_API_MINOR;
    // Assign the plug-in name
    std::snprintf(_Info->name, sizeof(_Info->name), "%s", SQSQLITE_HOST_NAME);
    // Bind callbacks
    BindCallbacks();
    // Notify that the plug-in was successfully loaded
    OutputMessage("Successfully loaded %s", SQSQLITE_NAME);
    // Dummy spacing
    puts("");
    // Done!
    return SQMOD_SUCCESS;
}
