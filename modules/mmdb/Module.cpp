// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <cstdlib>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
extern void Register_Database(Table & mmns);
extern void Register_Description(Table & mmns);
extern void Register_EntryData(Table & mmns);
extern void Register_EntryDataList(Table & mmns);
extern void Register_LookupResult(Table & mmns);
extern void Register_Metadata(Table & mmns);
extern void Register_SearchNode(Table & mmns);
extern void Register_SockAddr(Table & mmns);

/* ------------------------------------------------------------------------------------------------
 * Register the module API under the obtained virtual machine.
*/
static bool RegisterAPI(HSQUIRRELVM vm)
{
    // Make sure there's a valid virtual machine before proceeding
    if (!vm)
    {
        OutputError("%s: Cannot register API without a valid virtual machine", SQMMDB_NAME);
        // Registration failed
        return false;
    }

    Table mmns(vm);

    Register_Database(mmns);
    Register_Description(mmns);
    Register_EntryData(mmns);
    Register_EntryDataList(mmns);
    Register_LookupResult(mmns);
    Register_Metadata(mmns);
    Register_SearchNode(mmns);
    Register_SockAddr(mmns);

    mmns.Func(_SC("StrError"), MMDB_strerror);
    mmns.Func(_SC("LibVersion"), MMDB_lib_version);
    mmns.Func(_SC("TypeStr"), AsTypeStr);

    RootTable(vm).Bind(_SC("SqMMDB"), mmns);

    ConstTable(vm).Enum(_SC("SqMMDataType"), Enumeration(vm)
        .Const(_SC("Extended"),     MMDB_DATA_TYPE_EXTENDED)
        .Const(_SC("Pointer"),      MMDB_DATA_TYPE_POINTER)
        .Const(_SC("Utf8String"),   MMDB_DATA_TYPE_UTF8_STRING)
        .Const(_SC("Double"),       MMDB_DATA_TYPE_DOUBLE)
        .Const(_SC("Bytes"),        MMDB_DATA_TYPE_BYTES)
        .Const(_SC("Uint16"),       MMDB_DATA_TYPE_UINT16)
        .Const(_SC("Uint32"),       MMDB_DATA_TYPE_UINT32)
        .Const(_SC("Map"),          MMDB_DATA_TYPE_MAP)
        .Const(_SC("Int32"),        MMDB_DATA_TYPE_INT32)
        .Const(_SC("Uint64"),       MMDB_DATA_TYPE_UINT64)
        .Const(_SC("Uint128"),      MMDB_DATA_TYPE_UINT128)
        .Const(_SC("Array"),        MMDB_DATA_TYPE_ARRAY)
        .Const(_SC("Container"),    MMDB_DATA_TYPE_CONTAINER)
        .Const(_SC("EndMarker"),    MMDB_DATA_TYPE_END_MARKER)
        .Const(_SC("Boolean"),      MMDB_DATA_TYPE_BOOLEAN)
        .Const(_SC("Float"),        MMDB_DATA_TYPE_FLOAT)
    );

    ConstTable(vm).Enum(_SC("SqMMRecordType"), Enumeration(vm)
        .Const(_SC("SearchNode"),   MMDB_RECORD_TYPE_SEARCH_NODE)
        .Const(_SC("Empty"),        MMDB_RECORD_TYPE_EMPTY)
        .Const(_SC("Data"),         MMDB_RECORD_TYPE_DATA)
        .Const(_SC("Invalid"),      MMDB_RECORD_TYPE_INVALID)
    );

    ConstTable(vm).Enum(_SC("SqMMErrCode"), Enumeration(vm)
        .Const(_SC("Success"),                          MMDB_SUCCESS)
        .Const(_SC("FileOpenError"),                    MMDB_FILE_OPEN_ERROR)
        .Const(_SC("CorruptSearchTreeError"),           MMDB_CORRUPT_SEARCH_TREE_ERROR)
        .Const(_SC("InvalidMetadataError"),             MMDB_INVALID_METADATA_ERROR)
        .Const(_SC("IOError"),                          MMDB_IO_ERROR)
        .Const(_SC("OutOfMemoryError"),                 MMDB_OUT_OF_MEMORY_ERROR)
        .Const(_SC("UnknownDatabaseFormatError"),       MMDB_UNKNOWN_DATABASE_FORMAT_ERROR)
        .Const(_SC("InvalidDataError"),                 MMDB_INVALID_DATA_ERROR)
        .Const(_SC("InvalidLookupPathError"),           MMDB_INVALID_LOOKUP_PATH_ERROR)
        .Const(_SC("LookupPathDoesNotMatchDataError"),  MMDB_LOOKUP_PATH_DOES_NOT_MATCH_DATA_ERROR)
        .Const(_SC("InvalidNodeNumberError"),           MMDB_INVALID_NODE_NUMBER_ERROR)
        .Const(_SC("Ipv6LookupInIpv4DatabaseError"),    MMDB_IPV6_LOOKUP_IN_IPV4_DATABASE_ERROR)
    );

    Sqrat::ConstTable(vm)
        .Const(_SC("MMDB_MODE_MMAP"),  MMDB_MODE_MMAP)
        .Const(_SC("MMDB_MODE_MASK"),  MMDB_MODE_MASK);

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
        OutputError("%s: Cannot obtain the Squirrel virtual machine without the module API", SQMMDB_NAME);
        // Unable to proceed!
        return false;
    }
    // Obtain the Squirrel virtual machine from the host plug-in
    DefaultVM::Set(SqMod_GetSquirrelVM());
    // Make sure that a valid virtual machine exists
    if (!DefaultVM::Get())
    {
        OutputError("%s: Squirrel virtual machine obtained from the host plug-in is invalid", SQMMDB_NAME);
        // Unable to proceed!
        return false;
    }
    // Prevent common null objects from using dead virtual machines
    NullObject().Release();
    NullTable().Release();
    NullArray().Release();
    NullLightObj().Release();
    NullFunction().ReleaseGently();
    // Register the module API
    if (RegisterAPI(DefaultVM::Get()))
    {
        OutputMessage("Registered: %s", SQMMDB_NAME);
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
    OutputMessage("Terminating: %s", SQMMDB_NAME);
    // Release null objects just in case
    NullObject().Release();
    NullTable().Release();
    NullArray().Release();
    NullFunction().ReleaseGently();
    // Release script resources...
}

/* ------------------------------------------------------------------------------------------------
 * The virtual machined is about to be closed. Last chance to release anything manually.
*/
static void OnSquirrelClosing()
{
	// Nothing to release manually...
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
            if (CheckModuleAPIVer(message, SQMMDB_NAME))
            {
                try
                {
                    ImportModuleAPI(_Func, SQMMDB_NAME);
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
        case SQMOD_CLOSING_CMD:
        {
            OnSquirrelClosing();
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
    OutputMessage("Plug-in: %s", SQMMDB_NAME);
    OutputMessage("Author: %s", SQMMDB_AUTHOR);
    OutputMessage("Legal: %s", SQMMDB_COPYRIGHT);
    OutputMessage("--------------------------------------------------------------------");
    puts("");
    // Make sure that the module was loaded after the host plug-in
    if (!CheckModuleOrder(functions, info->pluginId, SQMMDB_NAME))
    {
        return SQMOD_FAILURE;
    }
    // Store server proxies
    _Func = functions;
    _Clbk = callbacks;
    _Info = info;
    // Assign plug-in version
    _Info->pluginVersion = SQMMDB_VERSION;
    _Info->apiMajorVersion = PLUGIN_API_MAJOR;
    _Info->apiMinorVersion = PLUGIN_API_MINOR;
    // Assign the plug-in name
    std::snprintf(_Info->name, sizeof(_Info->name), "%s", SQMMDB_HOST_NAME);
    // Bind to the server callbacks
    _Clbk->OnServerInitialise       = OnServerInitialise;
    _Clbk->OnServerShutdown         = OnServerShutdown;
    _Clbk->OnPluginCommand          = OnPluginCommand;
    // Notify that the plug-in was successfully loaded
    OutputMessage("Successfully loaded %s", SQMMDB_NAME);
    // Dummy spacing
    puts("");
    // Done!
    return SQMOD_SUCCESS;
}
