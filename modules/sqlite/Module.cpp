// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <cstdlib>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
extern void Register_Constants(Table & sqlns);
extern void Register_Common(Table & sqlns);
extern void Register_Connection(Table & sqlns);
extern void Register_Statement(Table & sqlns);
extern void Register_Parameter(Table & sqlns);
extern void Register_Column(Table & sqlns);
extern void Register_Transaction(Table & sqlns);

/* ------------------------------------------------------------------------------------------------
 * Register the module API under the obtained virtual machine.
*/
static bool RegisterAPI()
{
    // Make sure there's a valid virtual machine before proceeding
    if (!DefaultVM::Get())
    {
        OutputError("%s: Cannot register API without a valid virtual machine", SQSQLITE_NAME);
        // Registration failed
        return false;
    }

    Table sqlns;

    Register_Constants(sqlns);
    Register_Common(sqlns);
    Register_Connection(sqlns);
    Register_Statement(sqlns);
    Register_Parameter(sqlns);
    Register_Column(sqlns);
    Register_Transaction(sqlns);

    RootTable().Bind(_SC("SQLite"), sqlns);

    // Registration was successful
    return true;
}

/* ------------------------------------------------------------------------------------------------
 * Load the module on the virtual machine provided by the host module.
*/
static bool OnSquirrelLoad()
{
    // Make sure that we have a valid module API
    if (!SqMod_GetSquirrelVM)
    {
        OutputError("%s: Cannot obtain the Squirrel virtual machine without the module API", SQSQLITE_NAME);
        // Unable to proceed!
        return false;
    }
    // Obtain the Squirrel virtual machine from the host plug-in
    DefaultVM::Set(SqMod_GetSquirrelVM());
    // Make sure that a valid virtual machine exists
    if (!DefaultVM::Get())
    {
        OutputError("%s: Squirrel virtual machine obtained from the host plug-in is invalid", SQSQLITE_NAME);
        // Unable to proceed!
        return false;
    }
    // Prevent common null objects from using dead virtual machines
    NullArray() = Array();
    NullTable() = Table();
    NullObject() = Object();
    NullFunction() = Function();
    // Register the module API
    if (RegisterAPI())
    {
        OutputMessage("Registered: %s", SQSQLITE_NAME);
    }
    else
    {
        return false;
    }
    // At this point, the module was successfully loaded
    return true;
}

/* ------------------------------------------------------------------------------------------------
 * The virtual machine is about to be terminated and script resources should be released.
*/
static void OnSquirrelTerminate()
{
    OutputMessage("Terminating: %s", SQSQLITE_NAME);
    // Release null objects just in case
    NullObject().Release();
    NullTable().Release();
    NullArray().Release();
    NullFunction().ReleaseGently();
    // Release script resources...
}

/* ------------------------------------------------------------------------------------------------
 * The virtual machined was closed and all memory associated with it was released.
*/
static void OnSquirrelReleased()
{
    // Release the current virtual machine, if any
    DefaultVM::Set(nullptr);
}

/* ------------------------------------------------------------------------------------------------
 * React to command sent by other plug-ins.
*/
static uint8_t OnPluginCommand(uint32_t command_identifier, CCStr message)
{
    switch(command_identifier)
    {
        case SQMOD_INITIALIZE_CMD:
        {
            if (CheckModuleAPIVer(message, SQSQLITE_NAME))
            {
                try
                {
                    ImportModuleAPI(_Func, SQSQLITE_NAME);
                }
                catch (const Sqrat::Exception & e)
                {
                    OutputError("%s", e.what());
                    // Failed to initialize
                    return 0;
                }
            }
        } break;
        case SQMOD_LOAD_CMD:
        {
            return OnSquirrelLoad();
        } break;
        case SQMOD_TERMINATE_CMD:
        {
            OnSquirrelTerminate();
        } break;
        case SQMOD_RELEASED_CMD:
        {
            OnSquirrelReleased();
        } break;
        default: break;
    }
    return 1;
}

/* ------------------------------------------------------------------------------------------------
 * The server was initialized and this plug-in was loaded successfully.
*/
static uint8_t OnServerInitialise()
{
    return 1; // Initialization was successful
}

/* ------------------------------------------------------------------------------------------------
 * The server is about to shutdown gracefully.
*/
static void OnServerShutdown(void)
{
    // The server may still send callbacks
    _Clbk->OnServerInitialise       = nullptr;
    _Clbk->OnServerShutdown         = nullptr;
    _Clbk->OnPluginCommand          = nullptr;
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
    // Make sure that the module was loaded after the host plug-in
    if (!CheckModuleOrder(functions, info->pluginId, SQSQLITE_NAME))
    {
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
    // Bind to the server callbacks
    _Clbk->OnServerInitialise       = OnServerInitialise;
    _Clbk->OnServerShutdown         = OnServerShutdown;
    _Clbk->OnPluginCommand          = OnPluginCommand;
    // Notify that the plug-in was successfully loaded
    OutputMessage("Successfully loaded %s", SQSQLITE_NAME);
    // Dummy spacing
    puts("");
    // Done!
    return SQMOD_SUCCESS;
}
