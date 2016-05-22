// --------------------------------------------------------------------------------------------
#include "Module.hpp"
#include "Common.hpp"
#include "Entries.hpp"
#include "Document.hpp"

// --------------------------------------------------------------------------------------------
#include <cstdio>
#include <cstdlib>
#include <cstdarg>

// --------------------------------------------------------------------------------------------
#include <sqrat.h>

// --------------------------------------------------------------------------------------------
#if defined(WIN32) || defined(_WIN32)
    #include <Windows.h>
#endif

namespace SqMod {

// --------------------------------------------------------------------------------------------
PluginFuncs*        _Func = nullptr;
PluginCallbacks*    _Clbk = nullptr;
PluginInfo*         _Info = nullptr;

// --------------------------------------------------------------------------------------------
HSQAPI              _SqAPI = nullptr;
HSQEXPORTS          _SqMod = nullptr;
HSQUIRRELVM         _SqVM = nullptr;

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
    {
        OutputError("Failed to attach [%s] on host plugin.", SQINI_NAME);
    }
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
    // Register the module API
    RegisterAPI(_SqVM);
    // Notify about the current status
    OutputMessage("Registered: %s", SQINI_NAME);
}

/* --------------------------------------------------------------------------------------------
 * The virtual machine is about to be terminated and script resources should be released.
*/
void OnSquirrelTerminate()
{
    OutputMessage("Terminating: %s", SQINI_NAME);
    // Release the current virtual machine, if any
    DefaultVM::Set(nullptr);
}

/* --------------------------------------------------------------------------------------------
 * Validate the module API to make sure we don't run into issues.
*/
bool CheckAPIVer(CCStr ver)
{
    // Obtain the numeric representation of the API version
    long vernum = std::strtol(ver, nullptr, 10);
    // Check against version mismatch
    if (vernum == SQMOD_API_VER)
    {
        return true;
    }
    // Log the incident
    OutputError("API version mismatch on %s", SQINI_NAME);
    OutputMessage("=> Requested: %ld Have: %ld", vernum, SQMOD_API_VER);
    // Invoker should not attempt to communicate through the module API
    return false;
}

/* --------------------------------------------------------------------------------------------
 * React to command sent by other plugins.
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
        default: break;
    }
    return 1;
}

/* --------------------------------------------------------------------------------------------
 * The server was initialized and this plugin was loaded successfully.
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

// --------------------------------------------------------------------------------------------
void RegisterAPI(HSQUIRRELVM vm)
{
    Table inins(vm);

    inins.Bind(_SC("Result"), Class< IniResult >(vm, _SC("SqIniResult"))
        // Constructors
        .Ctor()
        .Ctor< CSStr, SQInteger >()
        .Ctor< const IniResult & >()
        // Core Metamethods
        .Func(_SC("_cmp"), &IniResult::Cmp)
        .SquirrelFunc(_SC("_typename"), &IniResult::Typename)
        .Func(_SC("_tostring"), &IniResult::ToString)
        // Properties
        .Prop(_SC("Valid"), &IniResult::IsValid)
        .Prop(_SC("Action"), &IniResult::GetAction)
        .Prop(_SC("Result"), &IniResult::GetResult)
        // Member Methods
        .Func(_SC("Check"), &IniResult::Check)
    );

    inins.Bind(_SC("Entries"), Class< Entries >(vm, _SC("SqIniEntries"))
        // Constructors
        .Ctor()
        .Ctor< const Entries & >()
        // Core Metamethods
        .Func(_SC("_cmp"), &Entries::Cmp)
        .SquirrelFunc(_SC("_typename"), &Entries::Typename)
        .Func(_SC("_tostring"), &Entries::ToString)
        // Properties
        .Prop(_SC("Valid"), &Entries::IsValid)
        .Prop(_SC("Empty"), &Entries::IsEmpty)
        .Prop(_SC("References"), &Entries::GetRefCount)
        .Prop(_SC("Size"), &Entries::GetSize)
        .Prop(_SC("Item"), &Entries::GetItem)
        .Prop(_SC("Comment"), &Entries::GetComment)
        .Prop(_SC("Order"), &Entries::GetOrder)
        // Member Methods
        .Func(_SC("Reset"), &Entries::Reset)
        .Func(_SC("Next"), &Entries::Next)
        .Func(_SC("Prev"), &Entries::Prev)
        .Func(_SC("Advance"), &Entries::Advance)
        .Func(_SC("Retreat"), &Entries::Retreat)
        .Func(_SC("Sort"), &Entries::Sort)
        .Func(_SC("SortByKeyOrder"), &Entries::SortByKeyOrder)
        .Func(_SC("SortByLoadOrder"), &Entries::SortByLoadOrder)
    );

    inins.Bind(_SC("Document"), Class< Document, NoCopy< Document > >(vm, _SC("SqIniDocument"))
        // Constructors
        .Ctor()
        .Ctor< bool >()
        .Ctor< bool, bool >()
        .Ctor< bool, bool, bool >()
        // Core Metamethods
        .Func(_SC("_cmp"), &Document::Cmp)
        .SquirrelFunc(_SC("_typename"), &Document::Typename)
        .Func(_SC("_tostring"), &Document::ToString)
        // Properties
        .Prop(_SC("Valid"), &Document::IsValid)
        .Prop(_SC("Empty"), &Document::IsEmpty)
        .Prop(_SC("References"), &Document::GetRefCount)
        .Prop(_SC("Unicode"), &Document::GetUnicode, &Document::SetUnicode)
        .Prop(_SC("MultiKey"), &Document::GetMultiKey, &Document::SetMultiKey)
        .Prop(_SC("MultiLine"), &Document::GetMultiLine, &Document::SetMultiLine)
        .Prop(_SC("Spaces"), &Document::GetSpaces, &Document::SetSpaces)
        // Member Methods
        .Func(_SC("Reset"), &Document::Reset)
        .Func(_SC("LoadFile"), &Document::LoadFile)
        .Overload< IniResult (Document::*)(CSStr) >(_SC("LoadString"), &Document::LoadData)
        .Overload< IniResult (Document::*)(CSStr, Int32) >(_SC("LoadString"), &Document::LoadData)
        .Overload< IniResult (Document::*)(CSStr) >(_SC("SaveFile"), &Document::SaveFile)
        .Overload< IniResult (Document::*)(CSStr, bool) >(_SC("SaveFile"), &Document::SaveFile)
        .Func(_SC("SaveData"), &Document::SaveData)
        .Func(_SC("GetSections"), &Document::GetAllSections)
        .Func(_SC("GetKeys"), &Document::GetAllKeys)
        .Func(_SC("GetValues"), &Document::GetAllValues)
        .Func(_SC("GetSectionSize"), &Document::GetSectionSize)
        .Func(_SC("HasMultipleKeys"), &Document::HasMultipleKeys)
        .Func(_SC("GetValue"), &Document::GetValue)
        .Func(_SC("GetInteger"), &Document::GetInteger)
        .Func(_SC("GetFloat"), &Document::GetFloat)
        .Func(_SC("GetBoolean"), &Document::GetBoolean)
        .Overload< IniResult (Document::*)(CSStr, CSStr, CSStr) >(_SC("SetValue"), &Document::SetValue)
        .Overload< IniResult (Document::*)(CSStr, CSStr, CSStr, bool) >(_SC("SetValue"), &Document::SetValue)
        .Overload< IniResult (Document::*)(CSStr, CSStr, CSStr, bool, CSStr) >(_SC("SetValue"), &Document::SetValue)
        .Overload< IniResult (Document::*)(CSStr, CSStr, SQInteger) >(_SC("SetInteger"), &Document::SetInteger)
        .Overload< IniResult (Document::*)(CSStr, CSStr, SQInteger, bool) >(_SC("SetInteger"), &Document::SetInteger)
        .Overload< IniResult (Document::*)(CSStr, CSStr, SQInteger, bool, bool) >(_SC("SetInteger"), &Document::SetInteger)
        .Overload< IniResult (Document::*)(CSStr, CSStr, SQInteger, bool, bool, CSStr) >(_SC("SetInteger"), &Document::SetInteger)
        .Overload< IniResult (Document::*)(CSStr, CSStr, SQFloat) >(_SC("SetFloat"), &Document::SetFloat)
        .Overload< IniResult (Document::*)(CSStr, CSStr, SQFloat, bool) >(_SC("SetFloat"), &Document::SetFloat)
        .Overload< IniResult (Document::*)(CSStr, CSStr, SQFloat, bool, CSStr) >(_SC("SetFloat"), &Document::SetFloat)
        .Overload< IniResult (Document::*)(CSStr, CSStr, bool) >(_SC("SetBoolean"), &Document::SetBoolean)
        .Overload< IniResult (Document::*)(CSStr, CSStr, bool, bool) >(_SC("SetBoolean"), &Document::SetBoolean)
        .Overload< IniResult (Document::*)(CSStr, CSStr, bool, bool, CSStr) >(_SC("SetBoolean"), &Document::SetBoolean)
        .Overload< bool (Document::*)(CSStr) >(_SC("DeleteValue"), &Document::DeleteValue)
        .Overload< bool (Document::*)(CSStr, CSStr) >(_SC("DeleteValue"), &Document::DeleteValue)
        .Overload< bool (Document::*)(CSStr, CSStr, CSStr) >(_SC("DeleteValue"), &Document::DeleteValue)
        .Overload< bool (Document::*)(CSStr, CSStr, CSStr, bool) >(_SC("DeleteValue"), &Document::DeleteValue)
    );

    RootTable(vm).Bind(_SC("SqIni"), inins);

    ConstTable(vm).Enum(_SC("SqIniError"), Enumeration(vm)
        .Const(_SC("Ok"),                       Int32(SI_OK))
        .Const(_SC("Updated"),                  Int32(SI_UPDATED))
        .Const(_SC("Inserted"),                 Int32(SI_INSERTED))
        .Const(_SC("Fail"),                     Int32(SI_FAIL))
        .Const(_SC("NoMem"),                    Int32(SI_NOMEM))
        .Const(_SC("File"),                     Int32(SI_FILE))
    );
}

// --------------------------------------------------------------------------------------------
void OutputMessageImpl(const char * msg, va_list args)
{
#if defined(WIN32) || defined(_WIN32)
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csb_before;
    GetConsoleScreenBufferInfo( hstdout, &csb_before);
    SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN);
    std::printf("[SQMOD] ");

    SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
    std::vprintf(msg, args);
    std::puts("");

    SetConsoleTextAttribute(hstdout, csb_before.wAttributes);
#else
    std::printf("%c[0;32m[SQMOD]%c[0m", 27, 27);
    std::vprintf(msg, args);
    std::puts("");
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
    std::printf("[SQMOD] ");

    SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
    std::vprintf(msg, args);
    std::puts("");

    SetConsoleTextAttribute(hstdout, csb_before.wAttributes);
#else
    std::printf("%c[0;91m[SQMOD]%c[0m", 27, 27);
    std::vprintf(msg, args);
    std::puts("");
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
    OutputMessage("Plugin: %s", SQINI_NAME);
    OutputMessage("Author: %s", SQINI_AUTHOR);
    OutputMessage("Legal: %s", SQINI_COPYRIGHT);
    OutputMessage("--------------------------------------------------------------------");
    puts("");
    // Attempt to find the host plugin ID
    int host_plugin_id = functions->FindPlugin((char *)(SQMOD_HOST_NAME));
    // See if our plugin was loaded after the host plugin
    if (host_plugin_id < 0)
    {
        OutputError("%s could find the host plugin", SQINI_NAME);
        // Don't load!
        return SQMOD_FAILURE;
    }
    // Should never reach this point but just in case
    else if (static_cast< Uint32 >(host_plugin_id) > info->pluginId)
    {
        OutputError("%s loaded after the host plugin", SQINI_NAME);
        // Don't load!
        return SQMOD_FAILURE;
    }
    // Store server proxies
    _Func = functions;
    _Clbk = callbacks;
    _Info = info;
    // Assign plugin version
    _Info->pluginVersion = SQINI_VERSION;
    _Info->apiMajorVersion = PLUGIN_API_MAJOR;
    _Info->apiMinorVersion = PLUGIN_API_MINOR;
    // Assign the plugin name
    std::snprintf(_Info->name, sizeof(_Info->name), "%s", SQINI_HOST_NAME);
    // Bind callbacks
    BindCallbacks();
    // Notify that the plugin was successfully loaded
    OutputMessage("Successfully loaded %s", SQINI_NAME);
    // Dummy spacing
    puts("");
    // Done!
    return SQMOD_SUCCESS;
}
