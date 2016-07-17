// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <cstdlib>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
extern void Register_Account(Table & sqlns);
extern void Register_Connection(Table & sqlns);
extern void Register_ResultSet(Table & sqlns);
extern void Register_Statement(Table & sqlns);

/* ------------------------------------------------------------------------------------------------
 * Register the module API under the obtained virtual machine.
*/
static bool RegisterAPI()
{
    // Make sure there's a valid virtual machine before proceeding
    if (!DefaultVM::Get())
    {
        OutputError("%s: Cannot register API without a valid virtual machine", SQMYSQL_NAME);
        // Registration failed
        return false;
    }

    Table sqlns(DefaultVM::Get());

    Register_Account(sqlns);
    Register_Connection(sqlns);
    Register_ResultSet(sqlns);
    Register_Statement(sqlns);

    RootTable(DefaultVM::Get()).Bind(_SC("SqMySQL"), sqlns);

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
        OutputError("%s: Cannot obtain the Squirrel virtual machine without the module API", SQMYSQL_NAME);
        // Unable to proceed!
        return false;
    }
    // Obtain the Squirrel virtual machine from the host plug-in
    DefaultVM::Set(SqMod_GetSquirrelVM());
    // Make sure that a valid virtual machine exists
    if (!DefaultVM::Get())
    {
        OutputError("%s: Squirrel virtual machine obtained from the host plug-in is invalid", SQMYSQL_NAME);
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
        OutputMessage("Registered: %s", SQMYSQL_NAME);
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
    OutputMessage("Terminating: %s", SQMYSQL_NAME);
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
            if (CheckModuleAPIVer(message, SQMYSQL_NAME))
            {
                try
                {
                    ImportModuleAPI(_Func, SQMYSQL_NAME);
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
    std::puts("");
    OutputMessage("--------------------------------------------------------------------");
    OutputMessage("Plug-in: %s", SQMYSQL_NAME);
    OutputMessage("Author: %s", SQMYSQL_AUTHOR);
    OutputMessage("Legal: %s", SQMYSQL_COPYRIGHT);
    OutputMessage("--------------------------------------------------------------------");
    std::puts("");
    // Make sure that the module was loaded after the host plug-in
    if (!CheckModuleOrder(functions, info->pluginId, SQMYSQL_NAME))
    {
        return SQMOD_FAILURE;
    }
    // Store server proxies
    _Func = functions;
    _Clbk = callbacks;
    _Info = info;
    // Assign plug-in version
    _Info->pluginVersion = SQMYSQL_VERSION;
    _Info->apiMajorVersion = PLUGIN_API_MAJOR;
    _Info->apiMinorVersion = PLUGIN_API_MINOR;
    // Assign the plug-in name
    std::snprintf(_Info->name, sizeof(_Info->name), "%s", SQMYSQL_HOST_NAME);
    // Bind to the server callbacks
    _Clbk->OnServerInitialise       = OnServerInitialise;
    _Clbk->OnServerShutdown         = OnServerShutdown;
    _Clbk->OnPluginCommand          = OnPluginCommand;
    // Notify that the plug-in was successfully loaded
    OutputMessage("Successfully loaded %s", SQMYSQL_NAME);
    // Dummy spacing
    std::puts("");
    // Done!
    return SQMOD_SUCCESS;
}
