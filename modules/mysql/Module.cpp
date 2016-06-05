// ------------------------------------------------------------------------------------------------
#include "Common.hpp"
#include "Account.hpp"
#include "Column.hpp"
#include "Connection.hpp"
#include "ResultSet.hpp"
#include "Savepoint.hpp"
#include "Statement.hpp"
#include "Transaction.hpp"

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
        OutputError("Failed to attach [%s] on host plug-in.", SQMYSQL_NAME);
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
    OutputMessage("Registered: %s", SQMYSQL_NAME);
}

/* ------------------------------------------------------------------------------------------------
 * The virtual machine is about to be terminated and script resources should be released.
*/
void OnSquirrelTerminate()
{
    OutputMessage("Terminating: %s", SQMYSQL_NAME);
    // Release null objects just in case
    NullObject().Release();
    NullTable().Release();
    NullArray().Release();
    NullFunction().ReleaseGently();
    // Release the current virtual machine, if any
    DefaultVM::Set(nullptr);
    // Release script resources...
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
    OutputError("API version mismatch on %s", SQMYSQL_NAME);
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
    Table sqlns(vm);

    sqlns.Bind(_SC("Account"), Class< Account >(vm, _SC("SqMySQLAccount"))
        // Constructors
        .Ctor< const Account & >()
        .Ctor< CSStr, CSStr >()
        .Ctor< CSStr, CSStr, CSStr >()
        .Ctor< CSStr, CSStr, CSStr, CSStr >()
        .Ctor< CSStr, CSStr, CSStr, CSStr, SQInteger >()
        .Ctor< CSStr, CSStr, CSStr, CSStr, SQInteger, CSStr >()
        // Core Meta-methods
        .Func(_SC("_cmp"), &Account::Cmp)
        .SquirrelFunc(_SC("_typename"), &Account::Typename)
        .Func(_SC("_tostring"), &Account::ToString)
        // Properties
        .Prop(_SC("Port"), &Account::GetPortNum, &Account::SetPortNum)
        .Prop(_SC("Host"), &Account::GetHost, &Account::SetHost)
        .Prop(_SC("User"), &Account::GetUser, &Account::SetUser)
        .Prop(_SC("Pass"), &Account::GetPass, &Account::SetPass)
        .Prop(_SC("Socket"), &Account::GetSocket, &Account::SetSocket)
        .Prop(_SC("Flags"), &Account::GetFlags, &Account::SetFlags)
        .Prop(_SC("SSL_Key"), &Account::GetSSL_Key, &Account::SetSSL_Key)
        .Prop(_SC("SSL_Cert"), &Account::GetSSL_Cert, &Account::SetSSL_Cert)
        .Prop(_SC("SSL_CA"), &Account::GetSSL_CA, &Account::SetSSL_CA)
        .Prop(_SC("SSL_CA_Path"), &Account::GetSSL_CA_Path, &Account::SetSSL_CA_Path)
        .Prop(_SC("SSL_Cipher"), &Account::GetSSL_Cipher, &Account::SetSSL_Cipher)
        .Prop(_SC("AutoCommit"), &Account::GetAutoCommit, &Account::SetAutoCommit)
        .Prop(_SC("Options"), &Account::GetOptionsTable)
        .Prop(_SC("OptionsCount"), &Account::OptionsCount)
        .Prop(_SC("OptionsEmpty"), &Account::OptionsEmpty)
        // Member Methods
        .Func(_SC("EnableFlags"), &Account::EnableFlags)
        .Func(_SC("DisableFlags"), &Account::DisableFlags)
        .Func(_SC("SetSSL"), &Account::SetSSL)
        .Func(_SC("GetOption"), &Account::GetOption)
        .Func(_SC("SetOption"), &Account::SetOption)
        .Func(_SC("RemoveOption"), &Account::RemoveOption)
        .Func(_SC("OptionsClear"), &Account::OptionsClear)
        .Func(_SC("Connect"), &Account::Connect)
    );

    sqlns.Bind(_SC("Connection"), Class< Connection >(vm, _SC("SqMySQLConnection"))
        // Constructors
        .Ctor()
        .Ctor< const Account & >()
        // Core Meta-methods
        .Func(_SC("_cmp"), &Connection::Cmp)
        .SquirrelFunc(_SC("_typename"), &Connection::Typename)
        .Func(_SC("_tostring"), &Connection::ToString)
        // Properties
        .Prop(_SC("IsValid"), &Connection::IsValid)
        .Prop(_SC("Connected"), &Connection::Connected)
        .Prop(_SC("ErrNo"), &Connection::GetErrNo)
        .Prop(_SC("ErrStr"), &Connection::GetErrStr)
        .Prop(_SC("LastErrNo"), &Connection::GetLastErrNo)
        .Prop(_SC("LastErrStr"), &Connection::GetLastErrStr)
        .Prop(_SC("Port"), &Connection::GetPortNum)
        .Prop(_SC("Host"), &Connection::GetHost)
        .Prop(_SC("User"), &Connection::GetUser)
        .Prop(_SC("Pass"), &Connection::GetPass)
        .Prop(_SC("Name"), &Connection::GetName, &Connection::SetName)
        .Prop(_SC("Socket"), &Connection::GetSocket)
        .Prop(_SC("Flags"), &Connection::GetFlags)
        .Prop(_SC("SSL_Key"), &Connection::GetSSL_Key)
        .Prop(_SC("SSL_Cert"), &Connection::GetSSL_Cert)
        .Prop(_SC("SSL_CA"), &Connection::GetSSL_CA)
        .Prop(_SC("SSL_CA_Path"), &Connection::GetSSL_CA_Path)
        .Prop(_SC("SSL_Cipher"), &Connection::GetSSL_Cipher)
        .Prop(_SC("Charset"), &Connection::GetCharset, &Connection::SetCharset)
        .Prop(_SC("AutoCommit"), &Connection::GetAutoCommit, &Connection::SetAutoCommit)
        .Prop(_SC("InTransaction"), &Connection::GetInTransaction)
        // Member Methods
        .Func(_SC("Disconnect"), &Connection::Disconnect)
        .Func(_SC("SelectDb"), &Connection::SetName)
        .Func(_SC("Execute"), &Connection::Execute)
        .Func(_SC("Insert"), &Connection::Insert)
        .Func(_SC("Query"), &Connection::Query)
        .Func(_SC("Statement"), &Connection::GetStatement)
        .Func(_SC("Transaction"), &Connection::GetTransaction)
    );

    sqlns.Bind(_SC("Statement"), Class< Statement >(vm, _SC("SqMySQLStatement"))
        // Constructors
        .Ctor()
        // Core Meta-methods
        .Func(_SC("_cmp"), &Statement::Cmp)
        .SquirrelFunc(_SC("_typename"), &Statement::Typename)
        .Func(_SC("_tostring"), &Statement::ToString)
        // Properties
        .Prop(_SC("IsValid"), &Statement::IsValid)
        .Prop(_SC("Connection"), &Statement::GetConnection, &Statement::SetConnection)
        // Member Methods
        .Func(_SC("Execute"), &Statement::Execute)
        .Func(_SC("Insert"), &Statement::Insert)
        .Func(_SC("Query"), &Statement::Query)
        .Func(_SC("SetInt8"), &Statement::SetInt8)
        .Func(_SC("SetUint8"), &Statement::SetUint8)
        .Func(_SC("SetInt16"), &Statement::SetInt16)
        .Func(_SC("SetUint16"), &Statement::SetUint16)
        .Func(_SC("SetInt32"), &Statement::SetInt32)
        .Func(_SC("SetUint32"), &Statement::SetUint32)
        .Func(_SC("SetInt64"), &Statement::SetInt64)
        .Func(_SC("SetUint64"), &Statement::SetUint64)
        .Func(_SC("SetSLongInt"), &Statement::SetSLongInt)
        .Func(_SC("SetULongInt"), &Statement::SetULongInt)
        .Func(_SC("SetInteger"), &Statement::SetInteger)
        .Func(_SC("SetFloat32"), &Statement::SetFloat32)
        .Func(_SC("SetFloat64"), &Statement::SetFloat64)
        .Func(_SC("SetFloat"), &Statement::SetFloat)
        .Func(_SC("SetBoolean"), &Statement::SetBoolean)
        .Func(_SC("SetDate"), &Statement::SetDate)
        .Func(_SC("SetTime"), &Statement::SetTime)
        .Func(_SC("SetDatetime"), &Statement::SetDatetime)
        .Func(_SC("SetString"), &Statement::SetString)
        .Func(_SC("SetEnum"), &Statement::SetEnum)
        .Func(_SC("SetSet"), &Statement::SetSet)
        .Func(_SC("SetBlob"), &Statement::SetBlob)
        .Func(_SC("SetData"), &Statement::SetData)
        .Func(_SC("SetBuffer"), &Statement::SetData)
        .Func(_SC("SetNull"), &Statement::SetNull)
    );

    sqlns.Bind(_SC("ResultSet"), Class< ResultSet >(vm, _SC("SqMySQLResultSet"))
        // Constructors
        .Ctor()
        // Core Meta-methods
        .Func(_SC("_cmp"), &ResultSet::Cmp)
        .SquirrelFunc(_SC("_typename"), &ResultSet::Typename)
        .Func(_SC("_tostring"), &ResultSet::ToString)
        // Properties
        .Prop(_SC("IsValid"), &ResultSet::IsValid)
        // Member Methods
        .Func(_SC("SetInt8"), &ResultSet::GetInt8)
        .Func(_SC("SetUint8"), &ResultSet::GetUint8)
        .Func(_SC("SetInt16"), &ResultSet::GetInt16)
        .Func(_SC("SetUint16"), &ResultSet::GetUint16)
        .Func(_SC("SetInt32"), &ResultSet::GetInt32)
        .Func(_SC("SetUint32"), &ResultSet::GetUint32)
        .Func(_SC("SetInt64"), &ResultSet::GetInt64)
        .Func(_SC("SetUint64"), &ResultSet::GetUint64)
        .Func(_SC("SetFloat32"), &ResultSet::GetFloat32)
        .Func(_SC("SetFloat64"), &ResultSet::GetFloat64)
        .Func(_SC("SetBool"), &ResultSet::GetBoolean)
        .Func(_SC("SetBoolean"), &ResultSet::GetBoolean)
    );

    RootTable(vm).Bind(_SC("SqMySQL"), sqlns);
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
    // Attempt to find the host plug-in ID
    const int host_plugin_id = functions->FindPlugin(SQMOD_HOST_NAME);
    // See if our plug-in was loaded after the host plug-in
    if (host_plugin_id < 0)
    {
        OutputError("%s could find the host plug-in", SQMYSQL_NAME);
        // Don't load!
        return SQMOD_FAILURE;
    }
    // Should never reach this point but just in case
    else if (static_cast< Uint32 >(host_plugin_id) > info->pluginId)
    {
        OutputError("%s loaded after the host plug-in", SQMYSQL_NAME);
        // Don't load!
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
    // Bind callbacks
    BindCallbacks();
    // Notify that the plug-in was successfully loaded
    OutputMessage("Successfully loaded %s", SQMYSQL_NAME);
    // Dummy spacing
    std::puts("");
    // Done!
    return SQMOD_SUCCESS;
}
