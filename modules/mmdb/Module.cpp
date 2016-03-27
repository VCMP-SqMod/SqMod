// --------------------------------------------------------------------------------------------
#include "Module.hpp"
#include "Common.hpp"

// --------------------------------------------------------------------------------------------
#include <sqrat.h>

// --------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// --------------------------------------------------------------------------------------------
#if defined(WIN32) || defined(_WIN32)
    #include <Windows.h>
#endif

namespace SqMod {

// --------------------------------------------------------------------------------------------
PluginFuncs*        _Func = NULL;
PluginCallbacks*    _Clbk = NULL;
PluginInfo*         _Info = NULL;

// --------------------------------------------------------------------------------------------
HSQAPI              _SqAPI = NULL;
HSQEXPORTS          _SqMod = NULL;
HSQUIRRELVM         _SqVM = NULL;

/* ------------------------------------------------------------------------------------------------
 * Bind speciffic functions to certain server events.
*/
void BindCallbacks();

/* ------------------------------------------------------------------------------------------------
 * Undo changes made with BindCallbacks().
*/
void UnbindCallbacks();

/* --------------------------------------------------------------------------------------------
 * Register the module API under the specified virtual machine.
*/
void RegisterAPI(HSQUIRRELVM vm);

/* --------------------------------------------------------------------------------------------
 * Initialize the plugin by obtaining the API provided by the host plugin.
*/
void OnSquirrelInitialize()
{
    // Attempt to import the plugin API exported by the host plugin
    _SqMod = sq_api_import(_Func);
    // Did we failed to obtain the plugin exports?
    if(!_SqMod)
        OutputError("Failed to attach [%s] on host plugin.", SQMMDB_NAME);
    else
    {
        // Obtain the Squirrel API
        _SqAPI = _SqMod->GetSquirrelAPI();
        // Expand the Squirrel API into global functions
        sq_api_expand(_SqAPI);
    }
}

/* --------------------------------------------------------------------------------------------
 * Load the module on the virtual machine provided by the host module.
*/
void OnSquirrelLoad()
{
    // Make sure that we have a valid plugin API
    if (!_SqMod)
        return; /* Unable to proceed. */
    // Obtain the Squirrel API and VM
    _SqVM = _SqMod->GetSquirrelVM();
    // Make sure that a valid virtual machine exists
    if (!_SqVM)
        return; /* Unable to proceed. */
    // Set this as the default database
    DefaultVM::Set(_SqVM);
    // Register the module API
    RegisterAPI(_SqVM);
    // Notify about the current status
    OutputMessage("Registered: %s", SQMMDB_NAME);
}

/* --------------------------------------------------------------------------------------------
 * The virtual machine is about to be terminated and script resources should be released.
*/
void OnSquirrelTerminate()
{
    OutputMessage("Terminating: %s", SQMMDB_NAME);
    // Release the current database (if any)
    DefaultVM::Set(NULL);
    // Release script resources...
}

/* --------------------------------------------------------------------------------------------
 * Validate the module API to make sure we don't run into issues.
*/
bool CheckAPIVer(CCStr ver)
{
    // Obtain the numeric representation of the API version
    long vernum = strtol(ver, NULL, 10);
    // Check against version mismatch
    if (vernum == SQMOD_API_VER)
        return true;
    // Log the incident
    OutputError("API version mismatch on %s", SQMMDB_NAME);
    OutputMessage("=> Requested: %ld Have: %ld", vernum, SQMOD_API_VER);
    // Invoker should not attempt to communicate through the module API
    return false;
}

/* --------------------------------------------------------------------------------------------
 * React to command sent by other plugins.
*/
static int OnInternalCommand(unsigned int type, const char * text)
{
    switch(type)
    {
        case SQMOD_INITIALIZE_CMD:
            if (CheckAPIVer(text))
                OnSquirrelInitialize();
        break;
        case SQMOD_LOAD_CMD:
            OnSquirrelLoad();
        break;
        case SQMOD_TERMINATE_CMD:
            OnSquirrelTerminate();
        break;
        default: break;
    }
    return 1;
}

/* --------------------------------------------------------------------------------------------
 * The server was initialized and this plugin was loaded successfully.
*/
static int OnInitServer()
{
    return 1;
}

static void OnShutdownServer(void)
{
    // The server may still send callbacks
    UnbindCallbacks();
}

// ------------------------------------------------------------------------------------------------
void BindCallbacks()
{
    _Clbk->OnInitServer             = OnInitServer;
    _Clbk->OnInternalCommand        = OnInternalCommand;
    _Clbk->OnShutdownServer         = OnShutdownServer;
}

// ------------------------------------------------------------------------------------------------
void UnbindCallbacks()
{
    _Clbk->OnInitServer             = NULL;
    _Clbk->OnInternalCommand        = NULL;
    _Clbk->OnShutdownServer         = NULL;
}

// --------------------------------------------------------------------------------------------
void RegisterAPI(HSQUIRRELVM vm)
{

}

// --------------------------------------------------------------------------------------------
void OutputMessageImpl(const char * msg, va_list args)
{
#if defined(WIN32) || defined(_WIN32)
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csb_before;
    GetConsoleScreenBufferInfo( hstdout, &csb_before);
    SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN);
    printf("[SQMOD] ");

    SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
    vprintf(msg, args);
    puts("");

    SetConsoleTextAttribute(hstdout, csb_before.wAttributes);
#else
    printf("%c[0;32m[SQMOD]%c[0;37m", 27, 27);
    vprintf(msg, args);
    puts("");
#endif
}

// --------------------------------------------------------------------------------------------
void OutputErrorImpl(const char * msg, va_list args)
{
#if defined(WIN32) || defined(_WIN32)
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csb_before;
    GetConsoleScreenBufferInfo( hstdout, &csb_before);
    SetConsoleTextAttribute(hstdout, FOREGROUND_RED | FOREGROUND_INTENSITY);
    printf("[SQMOD] ");

    SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
    vprintf(msg, args);
    puts("");

    SetConsoleTextAttribute(hstdout, csb_before.wAttributes);
#else
    printf("%c[0;32m[SQMOD]%c[0;37m", 27, 27);
    vprintf(msg, args);
    puts("");
#endif
}

// --------------------------------------------------------------------------------------------
void OutputDebug(const char * msg, ...)
{
#ifdef _DEBUG
    // Initialize the arguments list
    va_list args;
    va_start(args, msg);
    // Call the output function
    OutputMessageImpl(msg, args);
    // Finalize the arguments list
    va_end(args);
#else
    SQMOD_UNUSED_VAR(msg);
#endif
}

// --------------------------------------------------------------------------------------------
void OutputMessage(const char * msg, ...)
{
    // Initialize the arguments list
    va_list args;
    va_start(args, msg);
    // Call the output function
    OutputMessageImpl(msg, args);
    // Finalize the arguments list
    va_end(args);
}

// --------------------------------------------------------------------------------------------
void OutputError(const char * msg, ...)
{
    // Initialize the arguments list
    va_list args;
    va_start(args, msg);
    // Call the output function
    OutputErrorImpl(msg, args);
    // Finalize the arguments list
    va_end(args);
}

} // Namespace:: SqMod

// --------------------------------------------------------------------------------------------
SQMOD_API_EXPORT unsigned int VcmpPluginInit(PluginFuncs* functions, PluginCallbacks* callbacks, PluginInfo* info)
{
    using namespace SqMod;
    // Output plugin header
    puts("");
    OutputMessage("--------------------------------------------------------------------");
    OutputMessage("Plugin: %s", SQMMDB_NAME);
    OutputMessage("Author: %s", SQMMDB_AUTHOR);
    OutputMessage("Legal: %s", SQMMDB_COPYRIGHT);
    OutputMessage("--------------------------------------------------------------------");
    puts("");
    // Attempt to find the host plugin ID
    int host_plugin_id = functions->FindPlugin((char *)(SQMOD_HOST_NAME));
    // See if our plugin was loaded after the host plugin
    if (host_plugin_id < 0)
    {
        OutputError("%s could find the host plugin", SQMMDB_NAME);
        // Don't load!
        return SQMOD_FAILURE;
    }
    // Should never reach this point but just in case
    else if (host_plugin_id > (info->nPluginId))
    {
        OutputError("%s loaded after the host plugin", SQMMDB_NAME);
        // Don't load!
        return SQMOD_FAILURE;
    }
    // Store server proxies
    _Func = functions;
    _Clbk = callbacks;
    _Info = info;
    // Assign plugin information
    _Info->uPluginVer = SQMMDB_VERSION;
    strcpy(_Info->szName, SQMMDB_HOST_NAME);
    // Bind callbacks
    BindCallbacks();
    // Notify that the plugin was successfully loaded
    OutputMessage("Successfully loaded %s", SQMMDB_NAME);
    // Dummy spacing
    puts("");
    // Done!
    return SQMOD_SUCCESS;
}
