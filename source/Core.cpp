#include "Core.hpp"
#include "Logger.hpp"
#include "Entity.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
#include "Misc/Automobile.hpp"
#include "Misc/Model.hpp"

// ------------------------------------------------------------------------------------------------
#include <SimpleIni.h>

// ------------------------------------------------------------------------------------------------
#include <sqstdio.h>
#include <sqstdblob.h>
#include <sqstdmath.h>
#include <sqstdsystem.h>
#include <sqstdstring.h>

// ------------------------------------------------------------------------------------------------
#include <cstdarg>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <exception>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const Core::Pointer _Core = Core::Inst();

// ------------------------------------------------------------------------------------------------
static std::shared_ptr<CSimpleIni> g_Config;

// ------------------------------------------------------------------------------------------------
Core::Core() noexcept
    : m_State(SQMOD_SUCCESS)
    , m_Options()
    , m_VM(nullptr)
    , m_RootTable(nullptr)
    , m_Scripts()
    , m_ErrorMsg()
    , m_PlayerTrack()
    , m_VehicleTrack()
{
    // Create a few shared buffers
    MakeBuffer(8);
}

// ------------------------------------------------------------------------------------------------
Core::~Core()
{
    // Tell the plugin to terminate
    this->Terminate();
}

// ------------------------------------------------------------------------------------------------
void Core::_Finalizer(Core * ptr) noexcept
{
    delete ptr; /* Assuming 'delete' checks for NULL */
}

// ------------------------------------------------------------------------------------------------
Core::Pointer Core::Inst() noexcept
{
    if (!_Core)
    {
        return Pointer(new Core(), &Core::_Finalizer);
    }

    return Pointer(nullptr, &Core::_Finalizer);
}

// ------------------------------------------------------------------------------------------------
bool Core::Init() noexcept
{
    LogMsg("%s", CenterStr("INITIALIZING", '*'));
    // Attempt to initialize the plugin resources
    if (!this->Configure() || !this->CreateVM() || !this->LoadScripts())
    {
        return false;
    }

    LogMsg("%s", CenterStr("SUCCESS", '*'));

    return true;
}

bool Core::Load() noexcept
{
    LogMsg("%s", CenterStr("LOADING", '*'));
    // Attempt to execute the loaded scripts
    if (!this->Execute())
    {
        return false;
    }

    LogMsg("%s", CenterStr("SUCCESS", '*'));

    return true;
}

// ------------------------------------------------------------------------------------------------
void Core::Deinit() noexcept
{
    // Release the VM created during the initialization process
    this->DestroyVM();
}

void Core::Unload() noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::Terminate() noexcept
{
    this->Deinit();
    this->Unload();
}

// ------------------------------------------------------------------------------------------------
void Core::SetState(SQInteger val) noexcept
{
    m_State = val;
}

SQInteger Core::GetState() const noexcept
{
    return m_State;
}

// ------------------------------------------------------------------------------------------------
string Core::GetOption(const String & name) const noexcept
{
    // Attempt to find the specified option
    OptionPool::const_iterator elem = m_Options.find(name);
    // Return the value associated with the found option
    return (elem == m_Options.cend()) ? String() : elem->second;
}

void Core::SetOption(const String & name, const String & value) noexcept
{
    m_Options[name] = value;
}

// ------------------------------------------------------------------------------------------------
Core::Buffer Core::PullBuffer(unsigned sz) noexcept
{
    // The container that will manage the buffer
    Buffer buf;
    // See if there's any buffers available in the pool
    if (m_BufferPool.empty())
    {
        //  Create a new buffer if one wasn't available
        buf.resize(sz);
    }
    // Just fetch one from the pool
    else
    {
        // Fetch the buffer
        buf = std::move(m_BufferPool.back());
        // Remove it from the pool
        m_BufferPool.pop();
    }
    // Give the obtained buffer
    return std::move(buf);
}

// ------------------------------------------------------------------------------------------------
void Core::PushBuffer(Buffer && buf) noexcept
{
    // Make sure we don't store empty buffers
    if (!buf.empty())
    {
        // Return the specified buffer back to the pool
        m_BufferPool.push(std::move(buf));
    }
}

// ------------------------------------------------------------------------------------------------
void Core::MakeBuffer(unsigned num, unsigned sz) noexcept
{
    // Create the specified number of buffers
    while (num--)
    {
        m_BufferPool.emplace(sz);
    }
}

// ------------------------------------------------------------------------------------------------
void Core::ConnectPlayer(SQInt32 id, SQInt32 header, SqObj & payload) noexcept
{
    // Attempt to activate the instance in the plugin at the received identifier
    if (EntMan< CPlayer >::Activate(id, false))
    {
        // Trigger the specific event
        OnPlayerCreated(id, header, payload);
    }
    else
    {
        LogErr("Unable to create a new <CPlayer> instance");
    }
}

void Core::DisconnectPlayer(SQInt32 id, SQInt32 header, SqObj & payload) noexcept
{
    // Check to be sure we have this player instance active
    if (Reference< CPlayer >::Verify(id))
    {
        // Trigger the specific event
        OnPlayerDestroyed(id, header, payload);
    }
}

// ------------------------------------------------------------------------------------------------
bool Core::Configure() noexcept
{
    LogDbg("Attempting to instantiate the configuration file");
    // See if the configurations instance was previously created
    if (g_Config)
    {
        // Release the loaded configurations
        g_Config->Reset();
    }
    // Create the configuration instance
    else
    {
        g_Config.reset(new CSimpleIniA(true, true, true));
    }
    // See if a configuration instance could be created
    if (!g_Config)
    {
        LogFtl("Unable to instantiate the configuration class");

        return false;
    }

    LogDbg("Attempting to load the configuration file.");
    // Attempt to load the configurations from disk
    SI_Error ini_ret = g_Config->LoadFile(_SC("./sqmod.ini"));
    // See if the configurations could be loaded
    if (ini_ret < 0)
    {
        switch (ini_ret)
        {
            case SI_FAIL:   LogErr("Failed to load the configuration file. Probably invalid"); break;
            case SI_NOMEM:  LogErr("Run out of memory while loading the configuration file"); break;
            case SI_FILE:   LogErr("Failed to load the configuration file. %s", strerror(errno)); break;
            default:        LogErr("Failed to load the configuration file for some unforeseen reason");
        }
        return false;
    }

    LogDbg("Applying the specified logging filters");
    // Apply the specified logging filters before anything else
    if (!SToB(g_Config->GetValue("ConsoleLog", "Debug", "true")))   _Log->DisableConsoleLevel(Logger::LEVEL_DBG);
    if (!SToB(g_Config->GetValue("ConsoleLog", "Message", "true"))) _Log->DisableConsoleLevel(Logger::LEVEL_MSG);
    if (!SToB(g_Config->GetValue("ConsoleLog", "Success", "true"))) _Log->DisableConsoleLevel(Logger::LEVEL_SCS);
    if (!SToB(g_Config->GetValue("ConsoleLog", "Info", "true")))    _Log->DisableConsoleLevel(Logger::LEVEL_INF);
    if (!SToB(g_Config->GetValue("ConsoleLog", "Warning", "true"))) _Log->DisableConsoleLevel(Logger::LEVEL_WRN);
    if (!SToB(g_Config->GetValue("ConsoleLog", "Error", "true")))   _Log->DisableConsoleLevel(Logger::LEVEL_ERR);
    if (!SToB(g_Config->GetValue("ConsoleLog", "Fatal", "true")))   _Log->DisableConsoleLevel(Logger::LEVEL_FTL);

    if (!SToB(g_Config->GetValue("FileLog", "Debug", "true")))      _Log->DisableFileLevel(Logger::LEVEL_DBG);
    if (!SToB(g_Config->GetValue("FileLog", "Message", "true")))    _Log->DisableFileLevel(Logger::LEVEL_MSG);
    if (!SToB(g_Config->GetValue("FileLog", "Success", "true")))    _Log->DisableFileLevel(Logger::LEVEL_SCS);
    if (!SToB(g_Config->GetValue("FileLog", "Info", "true")))       _Log->DisableFileLevel(Logger::LEVEL_INF);
    if (!SToB(g_Config->GetValue("FileLog", "Warning", "true")))    _Log->DisableFileLevel(Logger::LEVEL_WRN);
    if (!SToB(g_Config->GetValue("FileLog", "Error", "true")))      _Log->DisableFileLevel(Logger::LEVEL_ERR);
    if (!SToB(g_Config->GetValue("FileLog", "Fatal", "true")))      _Log->DisableFileLevel(Logger::LEVEL_FTL);

    LogDbg("Reading the options from the general section");

    CSimpleIniA::TNamesDepend general_options;

    if (g_Config->GetAllKeys("Options", general_options) || general_options.size() > 0)
    {
        for (const auto & cfg_option : general_options)
        {
            CSimpleIniA::TNamesDepend option_list;

            if (!g_Config->GetAllValues("Options", cfg_option.pItem, option_list))
            {
                continue;
            }

            option_list.sort(CSimpleIniA::Entry::LoadOrder());

            for (const auto & cfg_value : option_list)
            {
                m_Options[cfg_option.pItem] = cfg_value.pItem;
            }
        }
    }
    else
    {
        LogInf("No options specified in the configuration file");
    }

    return true;
}

// ------------------------------------------------------------------------------------------------
bool Core::CreateVM() noexcept
{
    LogDbg("Acquiring the virtual machine stack size");

    // Start with an unknown stack size
    SQInteger stack_size = SQMOD_UNKNOWN;

    // Attempt to get it from the configuration file
    try
    {
        stack_size = SToI< SQInteger >::Fn(GetOption(_SC("VMStackSize")), 0, 10);
    }
    catch (const std::invalid_argument & e)
    {
        LogWrn("Unable to extract option value: %s", e.what());
    }

    // Validate the stack size
    if (stack_size <= 0)
    {
        LogWrn("Invalid stack size. Reverting to default size: %d", SQMOD_STACK_SIZE);
        SetOption(_SC("VMStackSize"), std::to_string(SQMOD_STACK_SIZE));
        stack_size = SQMOD_STACK_SIZE;
    }

    LogInf("Creating a virtual machine with a stack size of: %d", stack_size);

    m_VM = sq_open(stack_size);

    if (!m_VM)
    {
        LogFtl("Unable to open a virtual machine with a stack size: %d", stack_size);

        return false;
    }
    else
    {
        DefaultVM::Set(m_VM);
        ErrorHandling::Enable(true);
        m_RootTable.reset(new RootTable(m_VM));
        m_Scripts.clear();
    }

    LogDbg("Registering the standard libraries");
    // Register the standard libraries
    sq_pushroottable(m_VM);
    sqstd_register_iolib(m_VM);
    sqstd_register_bloblib(m_VM);
    sqstd_register_mathlib(m_VM);
    sqstd_register_systemlib(m_VM);
    sqstd_register_stringlib(m_VM);
    sq_pop(m_VM, 1);

    LogDbg("Setting the base output function");
    // Set the function that handles the text output
    sq_setprintfunc(m_VM, PrintFunc, ErrorFunc);

    LogDbg("Setting the base error handlers");
    // Se the error handlers
    sq_setcompilererrorhandler(m_VM, CompilerErrorHandler);
    sq_newclosure(m_VM, RuntimeErrorHandler, 0);
    sq_seterrorhandler(m_VM);

    LogDbg("Registering the plugin API");
    // Register the plugin API
    if (!RegisterAPI(m_VM))
    {
        LogFtl("Unable to register the plugin API");
        return false;
    }
    // At this point the VM is ready
    return true;
}

void Core::DestroyVM() noexcept
{
    // See if the Virtual Machine wasn't already destroyed
    if (m_VM != nullptr)
    {
        // Let instances know that they should release links to this VM
        VMClose.Emit();
        // Release the references to the script objects
        m_Scripts.clear();
        // Release the reference to the root table
        m_RootTable.reset();
        // Assertions during close may cause double delete
        HSQUIRRELVM sq_vm = m_VM;
        // Explicitly set the virtual machine to null
        m_VM = nullptr;
        // Close the Virtual Machine
        sq_close(sq_vm);
    }
}

// ------------------------------------------------------------------------------------------------
bool Core::LoadScripts() noexcept
{
    LogDbg("Attempting to compile the specified scripts");
    // See if the config file was loaded
    if (!g_Config)
    {
        LogWrn("Cannot compile any scripts without the configurations");
        // No point in loading the plugin
        return false;
    }
    // Attempt to retrieve the list of strings specified in the config
    CSimpleIniA::TNamesDepend script_list;
    g_Config->GetAllValues("Scripts", "Source", script_list);
    // See if any script was specified
    if (script_list.size() <= 0)
    {
        LogWrn("No scripts specified in the configuration file");
        // No point in loading the plugin
        return false;
    }
    // Sort the list in it's original order
    script_list.sort(CSimpleIniA::Entry::LoadOrder());
    // Process each specified script path
    for (auto const & cfg_script : script_list)
    {
        // Get the file path as a string
        string path(cfg_script.pItem);
        // See if it wasn't already loaded
        if (m_Scripts.find(path) != m_Scripts.cend())
        {
            LogWrn("Script was already loaded: %s", path.c_str());
            // No point in loading it again
            continue;
        }
        // Attempt to compile it
        else if (!Compile(path))
        {
            // Plugin shouldn't load
            return false;
        }
        else
        {
            LogScs("Successfully compiled script: %s", path.c_str());
        }
    }
    // See if any script could be compiled
    if (m_Scripts.empty())
    {
        LogErr("No scripts compiled. No reason to load the plugin");
        // No point in loading the plugin
        return false;
    }
    // At this point everything went as expected
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Core::Compile(const string & name) noexcept
{
    // See if the specified script path is valid
    if (name.empty())
    {
        LogErr("Cannot compile script without a valid name");
        // Failed to compile the specified script
        return false;
    }
    // Create a new script container and insert it into the script pool
    std::pair< SqScriptPool::iterator, bool > res = m_Scripts.emplace(name, Script(m_VM));
    // See if the script container could be created and inserted
    if (res.second)
    {
        // Attempt to load and compile the specified script file
        res.first->second.CompileFile(name);
        // See if any compile time error occurred in the compiled script
        if (Error::Occurred(m_VM))
        {
            // Output debugging information
            LogErr("Unable to compile script: %s", name.c_str());
            LogInf("=> %s", Error::Message(m_VM).c_str());
            // Release the script container
            m_Scripts.erase(res.first);
            // Failed to compile the specified script
            return false;
        }
    }
    // Failed to create the script container
    else
    {
        LogErr("Unable to queue script: %s", name.c_str());
            // Failed to compile the specified script
        return false;
    }
    // At this point everything went as it should
    return true;
}

bool Core::Execute() noexcept
{
    LogDbg("Attempting to execute the specified scripts");
    // Go through each loaded script
    for (auto & elem : m_Scripts)
    {
        // Attempt to execute the script
        elem.second.Run();
        // See if the executed script had any errors
        if (Error::Occurred(m_VM))
        {
            // Output the error information
            LogErr("Unable to execute script: %s", elem.first.c_str());
            LogInf("=> %s", Error::Message(m_VM).c_str());
            // Failed to execute scripts
            return false;
        }
        else
        {
            LogScs("Successfully executed script: %s", elem.first.c_str());
        }
    }
    // At this point everything succeeded
    return true;
}

// ------------------------------------------------------------------------------------------------
void Core::PrintCallstack() noexcept
{
    SQStackInfos si;
    // Begin a new section in the console
    LogMsg("%s", CenterStr("CALLSTACK", '*'));
    // Trace back the function call
    for (SQInteger level = 1; SQ_SUCCEEDED(sq_stackinfos(m_VM, level, &si)); ++level)
    {
        // Function name
        LogInf("FUNCTION %s()", si.funcname ? si.funcname : _SC("unknown"));
        // Function location
        LogInf("=> [%d] : {%s}", si.line, si.source ? si.source : _SC("unknown"));
    }
    // Dummy variables used to retrieve values from the Squirrel VM
    const SQChar * s_ = 0, * name = 0;
    SQInteger i_, seq = 0;
    SQFloat f_;
    SQUserPointer p_;
    // Begin a new section in the console
    LogMsg("%s", CenterStr("LOCALS", '*'));
    // Process each local variable
    for (SQInteger level = 0; level < 10; level++) {
        seq = 0;
        while((name = sq_getlocal(m_VM, level, seq))) {
            ++seq;
            switch(sq_gettype(m_VM, -1))
            {
                case OT_NULL:
                    LogInf("NULL [%s] : ...", name);
                    break;
                case OT_INTEGER:
                    sq_getinteger(m_VM, -1, &i_);
                    LogInf("INTEGER [%s] : {%d}", name, i_);
                    break;
                case OT_FLOAT:
                    sq_getfloat(m_VM, -1, &f_);
                    LogInf("FLOAT [%s] : {%f}", name, f_);
                    break;
                case OT_USERPOINTER:
                    sq_getuserpointer(m_VM, -1, &p_);
                    LogInf("USERPOINTER [%s] : {%p}\n", name, p_);
                    break;
                case OT_STRING:
                    sq_getstring(m_VM, -1, &s_);
                    LogInf("STRING [%s] : {%s}", name, s_);
                    break;
                case OT_TABLE:
                    LogInf("TABLE [%s] : ...", name);
                    break;
                case OT_ARRAY:
                    LogInf("ARRAY [%s] : ...", name);
                    break;
                case OT_CLOSURE:
                    LogInf("CLOSURE [%s] : ...", name);
                    break;
                case OT_NATIVECLOSURE:
                    LogInf("NATIVECLOSURE [%s] : ...", name);
                    break;
                case OT_GENERATOR:
                    LogInf("GENERATOR [%s] : ...", name);
                    break;
                case OT_USERDATA:
                    LogInf("USERDATA [%s] : ...", name);
                    break;
                case OT_THREAD:
                    LogInf("THREAD [%s] : ...", name);
                    break;
                case OT_CLASS:
                    LogInf("CLASS [%s] : ...", name);
                    break;
                case OT_INSTANCE:
                    LogInf("INSTANCE [%s] : ...", name);
                    break;
                case OT_WEAKREF:
                    LogInf("WEAKREF [%s] : ...", name);
                    break;
                case OT_BOOL:
                    sq_getinteger(m_VM, -1, &i_);
                    LogInf("BOOL [%s] : {%s}", name, i_ ? _SC("true") : _SC("false"));
                    break;
                default:
                    LogErr("UNKNOWN [%s] : ...", name);
                break;
            }
            sq_pop(m_VM, 1);
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Core::PrintFunc(HSQUIRRELVM vm, const SQChar * str, ...) noexcept
{
    SQMOD_UNUSED_VAR(vm);
    // Prepare the arguments list
    va_list args;
    va_start(args, str);
    // Acquire a buffer from the buffer pool
    Core::Buffer vbuf = _Core->PullBuffer();
    // Attempt to process the specified format string
    SQInt32 fmt_ret = std::vsnprintf(vbuf.data(), vbuf.size(), str, args);
    // See if the formatting was successful
    if (fmt_ret < 0)
    {
        // Return the buffer back to the buffer pool
        _Core->PushBuffer(std::move(vbuf));
        LogErr("Format error");
        return;
    }
    // See if the buffer was big enough
    else if (_SCSZT(fmt_ret) > vbuf.size())
    {
        // Resize the buffer to accommodate the required size
        vbuf.resize(++fmt_ret);
        // Attempt to process the specified format string again
        fmt_ret = std::vsnprintf(vbuf.data(), vbuf.size(), str, args);
        // See if the formatting was successful
        if (fmt_ret < 0)
        {
            // Return the buffer back to the buffer pool
            _Core->PushBuffer(std::move(vbuf));
            LogErr("Format error");
            return;
        }
    }
    // Release the arguments list
    va_end(args);
    // Output the buffer content
    LogMsg("%s", vbuf.data());
    // Return the buffer back to the buffer pool
    _Core->PushBuffer(std::move(vbuf));
}

void Core::ErrorFunc(HSQUIRRELVM vm, const SQChar * str, ...) noexcept
{
    SQMOD_UNUSED_VAR(vm);
    // Prepare the arguments list
    va_list args;
    va_start(args, str);
    // Acquire a buffer from the buffer pool
    Core::Buffer vbuf = _Core->PullBuffer();
    // Attempt to process the specified format string
    SQInt32 fmt_ret = std::vsnprintf(vbuf.data(), vbuf.size(), str, args);
    // See if the formatting was successful
    if (fmt_ret < 0)
    {
        // Return the buffer back to the buffer pool
        _Core->PushBuffer(std::move(vbuf));
        LogErr("Format error");
        return;
    }
    // See if the buffer was big enough
    else if (_SCSZT(fmt_ret) > vbuf.size())
    {
        // Resize the buffer to accommodate the required size
        vbuf.resize(++fmt_ret);
        // Attempt to process the specified format string again
        fmt_ret = std::vsnprintf(vbuf.data(), vbuf.size(), str, args);
        // See if the formatting was successful
        if (fmt_ret < 0)
        {
            // Return the buffer back to the buffer pool
            _Core->PushBuffer(std::move(vbuf));
            LogErr("Format error");
            return;
        }
    }
    // Release the arguments list
    va_end(args);
    // Output the buffer content
    LogErr("%s", vbuf.data());
    // Return the buffer back to the buffer pool
    _Core->PushBuffer(std::move(vbuf));
}

// ------------------------------------------------------------------------------------------------
SQInteger Core::RuntimeErrorHandler(HSQUIRRELVM vm) noexcept
{
    // Verify the top of the stack and whether there's any information to process
    if (sq_gettop(vm) < 1)
    {
        return 0;
    }

    const SQChar * err_msg = NULL;
    // Attempt to retrieve the error message
    if (SQ_SUCCEEDED(sq_getstring(vm, 2, &err_msg)))
    {
        _Core->m_ErrorMsg.assign(err_msg);
    }
    else
    {
        _Core->m_ErrorMsg.assign(_SC("An unknown runtime error has occurred"));
    }
    // Start a new section in the console
    LogMsg("%s", CenterStr("ERROR", '*'));
    // Output the retrieved error message
    LogInf("[MESSAGE] : %s", _Core->m_ErrorMsg.c_str());
    // See if the specified verbosity level allows a print of the call-stack
    if (_Log->GetVerbosity() > 0)
    {
        _Core->PrintCallstack();
    }
    // Close the console section
    LogMsg("%s", CenterStr("CONCLUDED", '*'));
    // The error was handled
    return SQ_OK;
}

void Core::CompilerErrorHandler(HSQUIRRELVM vm, const SQChar * desc, const SQChar * src, SQInteger line, SQInteger column) noexcept
{
    SQMOD_UNUSED_VAR(vm);
    try
    {
        _Core->m_ErrorMsg.assign(ToStringF("%s : %s:%d : %s", src, line, column, desc));
    }
    catch (const std::exception & e)
    {
        LogErr("Compiler error: %s", e.what());
        _Core->m_ErrorMsg.assign(_SC("An unknown compiler error has occurred"));
    }
    // Output the obtained error message
    LogErr("%s", _Core->m_ErrorMsg.c_str());
}

// ------------------------------------------------------------------------------------------------
Reference< CBlip > Core::NewBlip(SQInt32 index, SQInt32 world, SQFloat x, SQFloat y, SQFloat z,
                                    SQInt32 scale, SQUint32 color, SQInt32 sprid,
                                    SQInt32 header, SqObj & payload) noexcept
{
    // Attempt to create the entity and return a reference to it
    return Reference< CBlip >(EntMan< CBlip >::Create(header, payload, true,
                                    index, world, x, y, z, scale, color, sprid));
}

// ------------------------------------------------------------------------------------------------
Reference< CCheckpoint > Core::NewCheckpoint(SQInt32 player, SQInt32 world, SQFloat x, SQFloat y, SQFloat z,
                                                Uint8 r, Uint8 g, Uint8 b, Uint8 a, SQFloat radius,
                                                SQInt32 header, SqObj & payload) noexcept
{
    // See if the specified player reference is valid
    if (!Reference< CPlayer >::Verify(player))
    {
        LogWrn("Attempting to <create a checkpoint instance> on an invalid player: %d", player);
    }
    // Attempt to create the entity and return a reference to it
    else
    {
        return Reference< CCheckpoint >(EntMan< CCheckpoint >::Create(header, payload, true,
                                        player, world, x, y, z, r, g, b, a, radius));
    }
    // Return an invalid reference
    return Reference< CCheckpoint >();
}

// ------------------------------------------------------------------------------------------------
Reference< CKeybind > Core::NewKeybind(SQInt32 slot, bool release,
                                        SQInt32 primary, SQInt32 secondary, SQInt32 alternative,
                                        SQInt32 header, SqObj & payload) noexcept
{
    // Attempt to create the entity and return a reference to it
    return Reference< CKeybind >(EntMan< CKeybind >::Create(header, payload, true,
                                    slot, release, primary, secondary, alternative));
}

// ------------------------------------------------------------------------------------------------
Reference< CObject > Core::NewObject(SQInt32 model, SQInt32 world, SQFloat x, SQFloat y, SQFloat z,
                                        SQInt32 alpha,
                                        SQInt32 header, SqObj & payload) noexcept
{
    // See if the specified model is valid
    if (!CModel::Valid(model))
    {
        LogWrn("Attempting to <create an object instance> using an invalid model: %d", model);
    }
    // Attempt to create the entity and return a reference to it
    else
    {
        return Reference< CObject >(EntMan< CObject >::Create(header, payload, true,
                                    model, world, x, y, z, alpha));
    }
    // Return an invalid reference
    return Reference< CObject >();
}

// ------------------------------------------------------------------------------------------------
Reference< CPickup > Core::NewPickup(SQInt32 model, SQInt32 world, SQInt32 quantity,
                                        SQFloat x, SQFloat y, SQFloat z, SQInt32 alpha, bool automatic,
                                        SQInt32 header, SqObj & payload) noexcept
{
    // See if the specified model is valid
    if (!CModel::Valid(model))
    {
        LogWrn("Attempting to <create a pickup instance> using an invalid model: %d", model);
    }
    // Attempt to create the entity and return a reference to it
    else
    {
        return Reference< CPickup >(EntMan< CPickup >::Create(header, payload, true,
                                    model, world, quantity, x, y, z, alpha, automatic));
    }
    // Return an invalid reference
    return Reference< CPickup >();
}

// ------------------------------------------------------------------------------------------------
Reference< CSphere > Core::NewSphere(SQInt32 player, SQInt32 world, SQFloat x, SQFloat y, SQFloat z,
                                        Uint8 r, Uint8 g, Uint8 b, SQFloat radius,
                                        SQInt32 header, SqObj & payload) noexcept
{
    // See if the specified player reference is valid
    if (!Reference< CPlayer >::Verify(player))
    {
        LogWrn("Attempting to <create a Sphere instance> on an invalid player: %d", player);
    }
    // Attempt to create the entity and return a reference to it
    else
    {
        return Reference< CSphere >(EntMan< CSphere >::Create(header, payload, true,
                                    player, world, x, y, z, r, g, b, radius));
    }
    // Return an invalid reference
    return Reference< CSphere >();
}

// ------------------------------------------------------------------------------------------------
Reference< CSprite > Core::NewSprite(SQInt32 index, const SQChar * file, SQInt32 xp, SQInt32 yp,
                                        SQInt32 xr, SQInt32 yr, SQFloat angle, SQInt32 alpha, bool rel,
                                        SQInt32 header, SqObj & payload) noexcept
{
    // See if the specified file path is valid
    if (!file)
    {
        LogWrn("Attempting to <create a sprite instance> with an empty path: null");
    }
    // Attempt to create the entity and return a reference to it
    else
    {
        return Reference< CSprite >(EntMan< CSprite >::Create(header, payload, true,
                                        index, file, xp, yp, xr, yr, angle, alpha, rel));
    }
    // Return an invalid reference
    return Reference< CSprite >();
}

// ------------------------------------------------------------------------------------------------
Reference< CTextdraw > Core::NewTextdraw(SQInt32 index, const SQChar * text, SQInt32 xp, SQInt32 yp,
                                            SQUint32 color, bool rel,
                                            SQInt32 header, SqObj & payload) noexcept
{
    // See if the specified text is valid
    if (!text)
    {
        LogWrn("Attempting to <create a textdraw instance> using an empty text: null");
    }
    // Attempt to create the entity and return a reference to it
    else
    {
        return Reference< CTextdraw >(EntMan< CTextdraw >::Create(header, payload, true,
                                        index, text, xp, yp, color, rel));
    }
    // Return an invalid reference
    return Reference< CTextdraw >();
}

// ------------------------------------------------------------------------------------------------
Reference< CVehicle > Core::NewVehicle(SQInt32 model, SQInt32 world, SQFloat x, SQFloat y, SQFloat z,
                                        SQFloat angle, SQInt32 primary, SQInt32 secondary,
                                        SQInt32 header, SqObj & payload) noexcept
{
    // See if the specified model is valid
    if (!CAutomobile::Valid(model))
    {
        LogWrn("Attempting to <create a vehicle instance> using an invalid model: %d", model);
    }
    // Attempt to create the entity and return a reference to it
    else
    {
        return Reference< CVehicle >(EntMan< CVehicle >::Create(header, payload, true,
                                        model, world, x, y, z, angle, primary, secondary));
    }
    // Return an invalid reference
    return Reference< CVehicle >();
}

// ------------------------------------------------------------------------------------------------
void Core::OnBlipCreated(SQInt32 blip, SQInt32 header, SqObj & payload) noexcept
{
    BlipCreated.Emit(blip, header, payload);
    Reference< CBlip >::Get(blip).BlipCreated.Emit(blip, header, payload);
}

void Core::OnCheckpointCreated(SQInt32 checkpoint, SQInt32 header, SqObj & payload) noexcept
{
    CheckpointCreated.Emit(checkpoint, header, payload);
    Reference< CCheckpoint >::Get(checkpoint).CheckpointCreated.Emit(checkpoint, header, payload);
}

void Core::OnKeybindCreated(SQInt32 keybind, SQInt32 header, SqObj & payload) noexcept
{
    KeybindCreated.Emit(keybind, header, payload);
    Reference< CKeybind >::Get(keybind).KeybindCreated.Emit(keybind, header, payload);
}

void Core::OnObjectCreated(SQInt32 object, SQInt32 header, SqObj & payload) noexcept
{
    ObjectCreated.Emit(object, header, payload);
    Reference< CObject >::Get(object).ObjectCreated.Emit(object, header, payload);
}

void Core::OnPickupCreated(SQInt32 pickup, SQInt32 header, SqObj & payload) noexcept
{
    PickupCreated.Emit(pickup, header, payload);
    Reference< CPickup >::Get(pickup).PickupCreated.Emit(pickup, header, payload);
}

void Core::OnPlayerCreated(SQInt32 player, SQInt32 header, SqObj & payload) noexcept
{
    PlayerCreated.Emit(player, header, payload);
    Reference< CPlayer >::Get(player).PlayerCreated.Emit(player, header, payload);
}

void Core::OnSphereCreated(SQInt32 sphere, SQInt32 header, SqObj & payload) noexcept
{
    SphereCreated.Emit(sphere, header, payload);
    Reference< CSphere >::Get(sphere).SphereCreated.Emit(sphere, header, payload);
}

void Core::OnSpriteCreated(SQInt32 sprite, SQInt32 header, SqObj & payload) noexcept
{
    SpriteCreated.Emit(sprite, header, payload);
    Reference< CSprite >::Get(sprite).SpriteCreated.Emit(sprite, header, payload);
}

void Core::OnTextdrawCreated(SQInt32 textdraw, SQInt32 header, SqObj & payload) noexcept
{
    TextdrawCreated.Emit(textdraw, header, payload);
    Reference< CTextdraw >::Get(textdraw).TextdrawCreated.Emit(textdraw, header, payload);
}

void Core::OnVehicleCreated(SQInt32 vehicle, SQInt32 header, SqObj & payload) noexcept
{
    VehicleCreated.Emit(vehicle, header, payload);
    Reference< CVehicle >::Get(vehicle).VehicleCreated.Emit(vehicle, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::OnBlipDestroyed(SQInt32 blip, SQInt32 header, SqObj & payload) noexcept
{
    BlipDestroyed.Emit(blip, header, payload);
    Reference< CBlip >::Get(blip).BlipDestroyed.Emit(blip, header, payload);
}

void Core::OnCheckpointDestroyed(SQInt32 checkpoint, SQInt32 header, SqObj & payload) noexcept
{
    CheckpointDestroyed.Emit(checkpoint, header, payload);
    Reference< CCheckpoint >::Get(checkpoint).CheckpointDestroyed.Emit(checkpoint, header, payload);
}

void Core::OnKeybindDestroyed(SQInt32 keybind, SQInt32 header, SqObj & payload) noexcept
{
    KeybindDestroyed.Emit(keybind, header, payload);
    Reference< CKeybind >::Get(keybind).KeybindDestroyed.Emit(keybind, header, payload);
}

void Core::OnObjectDestroyed(SQInt32 object, SQInt32 header, SqObj & payload) noexcept
{
    ObjectDestroyed.Emit(object, header, payload);
    Reference< CObject >::Get(object).ObjectDestroyed.Emit(object, header, payload);
}

void Core::OnPickupDestroyed(SQInt32 pickup, SQInt32 header, SqObj & payload) noexcept
{
    PickupDestroyed.Emit(pickup, header, payload);
    Reference< CPickup >::Get(pickup).PickupDestroyed.Emit(pickup, header, payload);
}

void Core::OnPlayerDestroyed(SQInt32 player, SQInt32 header, SqObj & payload) noexcept
{
    PlayerDestroyed.Emit(player, header, payload);
    Reference< CPlayer >::Get(player).PlayerDestroyed.Emit(player, header, payload);
}

void Core::OnSphereDestroyed(SQInt32 sphere, SQInt32 header, SqObj & payload) noexcept
{
    SphereDestroyed.Emit(sphere, header, payload);
    Reference< CSphere >::Get(sphere).SphereDestroyed.Emit(sphere, header, payload);
}

void Core::OnSpriteDestroyed(SQInt32 sprite, SQInt32 header, SqObj & payload) noexcept
{
    SpriteDestroyed.Emit(sprite, header, payload);
    Reference< CSprite >::Get(sprite).SpriteDestroyed.Emit(sprite, header, payload);
}

void Core::OnTextdrawDestroyed(SQInt32 textdraw, SQInt32 header, SqObj & payload) noexcept
{
    TextdrawDestroyed.Emit(textdraw, header, payload);
    Reference< CTextdraw >::Get(textdraw).TextdrawDestroyed.Emit(textdraw, header, payload);
}

void Core::OnVehicleDestroyed(SQInt32 vehicle, SQInt32 header, SqObj & payload) noexcept
{
    VehicleDestroyed.Emit(vehicle, header, payload);
    Reference< CVehicle >::Get(vehicle).VehicleDestroyed.Emit(vehicle, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::OnBlipCustom(SQInt32 blip, SQInt32 header, SqObj & payload) noexcept
{
    BlipCustom.Emit(blip, header, payload);
    Reference< CBlip >::Get(blip).BlipCustom.Emit(blip, header, payload);
}

void Core::OnCheckpointCustom(SQInt32 checkpoint, SQInt32 header, SqObj & payload) noexcept
{
    CheckpointCustom.Emit(checkpoint, header, payload);
    Reference< CCheckpoint >::Get(checkpoint).CheckpointCustom.Emit(checkpoint, header, payload);
}

void Core::OnKeybindCustom(SQInt32 keybind, SQInt32 header, SqObj & payload) noexcept
{
    KeybindCustom.Emit(keybind, header, payload);
    Reference< CKeybind >::Get(keybind).KeybindCustom.Emit(keybind, header, payload);
}

void Core::OnObjectCustom(SQInt32 object, SQInt32 header, SqObj & payload) noexcept
{
    ObjectCustom.Emit(object, header, payload);
    Reference< CObject >::Get(object).ObjectCustom.Emit(object, header, payload);
}

void Core::OnPickupCustom(SQInt32 pickup, SQInt32 header, SqObj & payload) noexcept
{
    PickupCustom.Emit(pickup, header, payload);
    Reference< CPickup >::Get(pickup).PickupCustom.Emit(pickup, header, payload);
}

void Core::OnPlayerCustom(SQInt32 player, SQInt32 header, SqObj & payload) noexcept
{
    PlayerCustom.Emit(player, header, payload);
    Reference< CPlayer >::Get(player).PlayerCustom.Emit(player, header, payload);
}

void Core::OnSphereCustom(SQInt32 sphere, SQInt32 header, SqObj & payload) noexcept
{
    SphereCustom.Emit(sphere, header, payload);
    Reference< CSphere >::Get(sphere).SphereCustom.Emit(sphere, header, payload);
}

void Core::OnSpriteCustom(SQInt32 sprite, SQInt32 header, SqObj & payload) noexcept
{
    SpriteCustom.Emit(sprite, header, payload);
    Reference< CSprite >::Get(sprite).SpriteCustom.Emit(sprite, header, payload);
}

void Core::OnTextdrawCustom(SQInt32 textdraw, SQInt32 header, SqObj & payload) noexcept
{
    TextdrawCustom.Emit(textdraw, header, payload);
    Reference< CTextdraw >::Get(textdraw).TextdrawCustom.Emit(textdraw, header, payload);
}

void Core::OnVehicleCustom(SQInt32 vehicle, SQInt32 header, SqObj & payload) noexcept
{
    VehicleCustom.Emit(vehicle, header, payload);
    Reference< CVehicle >::Get(vehicle).VehicleCustom.Emit(vehicle, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::OnPlayerAway(SQInt32 player, bool status) noexcept
{
    PlayerAway.Emit(player, status);
    Reference< CPlayer >::Get(player).PlayerAway.Emit(player, status);
}

// ------------------------------------------------------------------------------------------------
void Core::OnPlayerGameKeys(SQInt32 player, SQInt32 previous, SQInt32 current) noexcept
{
    PlayerGameKeys.Emit(player, previous, current);
    Reference< CPlayer >::Get(player).PlayerGameKeys.Emit(player, previous, current);
}

void Core::OnPlayerName(SQInt32 player, const SQChar * previous, const SQChar * current) noexcept
{
    PlayerRename.Emit(player, previous, current);
    Reference< CPlayer >::Get(player).PlayerRename.Emit(player, previous, current);
}

// ------------------------------------------------------------------------------------------------
void Core::OnPlayerRequestClass(SQInt32 player, SQInt32 offset) noexcept
{
    PlayerRequestClass.Emit(player, offset);
    Reference< CPlayer >::Get(player).PlayerRequestClass.Emit(player, offset);
}

void Core::OnPlayerRequestSpawn(SQInt32 player) noexcept
{
    PlayerRequestSpawn.Emit(player);
    Reference< CPlayer >::Get(player).PlayerRequestSpawn.Emit(player);
}

// ------------------------------------------------------------------------------------------------
void Core::OnPlayerSpawn(SQInt32 player) noexcept
{
    PlayerSpawn.Emit(player);
    Reference< CPlayer >::Get(player).PlayerSpawn.Emit(player);
}

// ------------------------------------------------------------------------------------------------
void Core::OnPlayerStartTyping(SQInt32 player) noexcept
{
    PlayerStartTyping.Emit(player);
    Reference< CPlayer >::Get(player).PlayerStartTyping.Emit(player);
}

void Core::OnPlayerStopTyping(SQInt32 player) noexcept
{
    PlayerStopTyping.Emit(player);
    Reference< CPlayer >::Get(player).PlayerStopTyping.Emit(player);
}

// ------------------------------------------------------------------------------------------------
void Core::OnPlayerChat(SQInt32 player, const SQChar * message) noexcept
{
    PlayerChat.Emit(player, message);
    Reference< CPlayer >::Get(player).PlayerChat.Emit(player, message);
}

void Core::OnPlayerCommand(SQInt32 player, const SQChar * command) noexcept
{
    PlayerCommand.Emit(player, command);
    Reference< CPlayer >::Get(player).PlayerCommand.Emit(player, command);
}

void Core::OnPlayerMessage(SQInt32 player, SQInt32 receiver, const SQChar * message) noexcept
{
    PlayerMessage.Emit(player, receiver, message);
    Reference< CPlayer >::Get(player).PlayerMessage.Emit(player, receiver, message);
}

// ------------------------------------------------------------------------------------------------
void Core::OnPlayerHealth(SQInt32 player, SQFloat previous, SQFloat current) noexcept
{
    PlayerHealth.Emit(player, previous, current);
    Reference< CPlayer >::Get(player).PlayerHealth.Emit(player, previous, current);
}

void Core::OnPlayerArmour(SQInt32 player, SQFloat previous, SQFloat current) noexcept
{
    PlayerArmour.Emit(player, previous, current);
    Reference< CPlayer >::Get(player).PlayerArmour.Emit(player, previous, current);
}

void Core::OnPlayerWeapon(SQInt32 player, SQInt32 previous, SQInt32 current) noexcept
{
    PlayerWeapon.Emit(player, previous, current);
    Reference< CPlayer >::Get(player).PlayerWeapon.Emit(player, previous, current);
}

void Core::OnPlayerMove(SQInt32 player, const Vector3 & previous, const Vector3 & current) noexcept
{
    PlayerMove.Emit(player, previous, current);
    Reference< CPlayer >::Get(player).PlayerMove.Emit(player, previous, current);
}

// ------------------------------------------------------------------------------------------------
void Core::OnPlayerWasted(SQInt32 player, SQInt32 reason) noexcept
{
    PlayerWasted.Emit(player, reason);
    Reference< CPlayer >::Get(player).PlayerWasted.Emit(player, reason);
}

void Core::OnPlayerKilled(SQInt32 player, SQInt32 killer, SQInt32 reason, SQInt32 body_part) noexcept
{
    PlayerKilled.Emit(player, killer, reason, body_part);
    Reference< CPlayer >::Get(player).PlayerKilled.Emit(player, killer, reason, body_part);
}

// ------------------------------------------------------------------------------------------------
void Core::OnPlayerSpectate(SQInt32 player, SQInt32 target) noexcept
{
    PlayerSpectate.Emit(player, target);
    Reference< CPlayer >::Get(player).PlayerSpectate.Emit(player, target);
}

void Core::OnPlayerCrashreport(SQInt32 player, const SQChar * report) noexcept
{
    PlayerCrashreport.Emit(player, report);
    Reference< CPlayer >::Get(player).PlayerCrashreport.Emit(player, report);
}

// ------------------------------------------------------------------------------------------------
void Core::OnPlayerBurning(SQInt32 player, bool state) noexcept
{
    PlayerBurning.Emit(player, state);
    Reference< CPlayer >::Get(player).PlayerBurning.Emit(player, state);
}

void Core::OnPlayerCrouching(SQInt32 player, bool state) noexcept
{
    PlayerCrouching.Emit(player, state);
    Reference< CPlayer >::Get(player).PlayerCrouching.Emit(player, state);
}

// ------------------------------------------------------------------------------------------------
void Core::OnPlayerState(SQInt32 player, SQInt32 previous, SQInt32 current) noexcept
{
    PlayerState.Emit(player, previous, current);
    Reference< CPlayer >::Get(player).PlayerState.Emit(player, previous, current);
}

void Core::OnPlayerAction(SQInt32 player, SQInt32 previous, SQInt32 current) noexcept
{
    PlayerAction.Emit(player, previous, current);
    Reference< CPlayer >::Get(player).PlayerAction.Emit(player, previous, current);
}

// ------------------------------------------------------------------------------------------------
void Core::OnStateNone(SQInt32 player, SQInt32 previous) noexcept
{
    StateNone.Emit(player, previous);
    Reference< CPlayer >::Get(player).StateNone.Emit(player, previous);
}

void Core::OnStateNormal(SQInt32 player, SQInt32 previous) noexcept
{
    StateNormal.Emit(player, previous);
    Reference< CPlayer >::Get(player).StateNormal.Emit(player, previous);
}

void Core::OnStateShooting(SQInt32 player, SQInt32 previous) noexcept
{
    StateShooting.Emit(player, previous);
    Reference< CPlayer >::Get(player).StateShooting.Emit(player, previous);
}

void Core::OnStateDriver(SQInt32 player, SQInt32 previous) noexcept
{
    StateDriver.Emit(player, previous);
    Reference< CPlayer >::Get(player).StateDriver.Emit(player, previous);
}

void Core::OnStatePassenger(SQInt32 player, SQInt32 previous) noexcept
{
    StatePassenger.Emit(player, previous);
    Reference< CPlayer >::Get(player).StatePassenger.Emit(player, previous);
}

void Core::OnStateEnterDriver(SQInt32 player, SQInt32 previous) noexcept
{
    StateEnterDriver.Emit(player, previous);
    Reference< CPlayer >::Get(player).StateEnterDriver.Emit(player, previous);
}

void Core::OnStateEnterPassenger(SQInt32 player, SQInt32 previous) noexcept
{
    StateEnterPassenger.Emit(player, previous);
    Reference< CPlayer >::Get(player).StateEnterPassenger.Emit(player, previous);
}

void Core::OnStateExitVehicle(SQInt32 player, SQInt32 previous) noexcept
{
    StateExitVehicle.Emit(player, previous);
    Reference< CPlayer >::Get(player).StateExitVehicle.Emit(player, previous);
}

void Core::OnStateUnspawned(SQInt32 player, SQInt32 previous) noexcept
{
    StateUnspawned.Emit(player, previous);
    Reference< CPlayer >::Get(player).StateUnspawned.Emit(player, previous);
}

// ------------------------------------------------------------------------------------------------
void Core::OnActionNone(SQInt32 player, SQInt32 previous) noexcept
{
    ActionNone.Emit(player, previous);
    Reference< CPlayer >::Get(player).ActionNone.Emit(player, previous);
}

void Core::OnActionNormal(SQInt32 player, SQInt32 previous) noexcept
{
    ActionNormal.Emit(player, previous);
    Reference< CPlayer >::Get(player).ActionNormal.Emit(player, previous);
}

void Core::OnActionAiming(SQInt32 player, SQInt32 previous) noexcept
{
    ActionAiming.Emit(player, previous);
    Reference< CPlayer >::Get(player).ActionAiming.Emit(player, previous);
}

void Core::OnActionShooting(SQInt32 player, SQInt32 previous) noexcept
{
    ActionShooting.Emit(player, previous);
    Reference< CPlayer >::Get(player).ActionShooting.Emit(player, previous);
}

void Core::OnActionJumping(SQInt32 player, SQInt32 previous) noexcept
{
    ActionJumping.Emit(player, previous);
    Reference< CPlayer >::Get(player).ActionJumping.Emit(player, previous);
}

void Core::OnActionLieDown(SQInt32 player, SQInt32 previous) noexcept
{
    ActionLieDown.Emit(player, previous);
    Reference< CPlayer >::Get(player).ActionLieDown.Emit(player, previous);
}

void Core::OnActionGettingUp(SQInt32 player, SQInt32 previous) noexcept
{
    ActionGettingUp.Emit(player, previous);
    Reference< CPlayer >::Get(player).ActionGettingUp.Emit(player, previous);
}

void Core::OnActionJumpVehicle(SQInt32 player, SQInt32 previous) noexcept
{
    ActionJumpVehicle.Emit(player, previous);
    Reference< CPlayer >::Get(player).ActionJumpVehicle.Emit(player, previous);
}

void Core::OnActionDriving(SQInt32 player, SQInt32 previous) noexcept
{
    ActionDriving.Emit(player, previous);
    Reference< CPlayer >::Get(player).ActionDriving.Emit(player, previous);
}

void Core::OnActionDying(SQInt32 player, SQInt32 previous) noexcept
{
    ActionDying.Emit(player, previous);
    Reference< CPlayer >::Get(player).ActionDying.Emit(player, previous);
}

void Core::OnActionWasted(SQInt32 player, SQInt32 previous) noexcept
{
    ActionWasted.Emit(player, previous);
    Reference< CPlayer >::Get(player).ActionWasted.Emit(player, previous);
}

void Core::OnActionEmbarking(SQInt32 player, SQInt32 previous) noexcept
{
    ActionEmbarking.Emit(player, previous);
    Reference< CPlayer >::Get(player).ActionEmbarking.Emit(player, previous);
}

void Core::OnActionDisembarking(SQInt32 player, SQInt32 previous) noexcept
{
    ActionDisembarking.Emit(player, previous);
    Reference< CPlayer >::Get(player).ActionDisembarking.Emit(player, previous);
}

// ------------------------------------------------------------------------------------------------
void Core::OnVehicleRespawn(SQInt32 vehicle) noexcept
{
    VehicleRespawn.Emit(vehicle);
    Reference< CVehicle >::Get(vehicle).VehicleRespawn.Emit(vehicle);
}

void Core::OnVehicleExplode(SQInt32 vehicle) noexcept
{
    VehicleExplode.Emit(vehicle);
    Reference< CVehicle >::Get(vehicle).VehicleExplode.Emit(vehicle);
}

// ------------------------------------------------------------------------------------------------
void Core::OnVehicleHealth(SQInt32 vehicle, SQFloat previous, SQFloat current) noexcept
{
    VehicleHealth.Emit(vehicle, previous, current);
    Reference< CVehicle >::Get(vehicle).VehicleHealth.Emit(vehicle, previous, current);
}

void Core::OnVehicleMove(SQInt32 vehicle, const Vector3 & previous, const Vector3 & current) noexcept
{
    VehicleMove.Emit(vehicle, previous, current);
    Reference< CVehicle >::Get(vehicle).VehicleMove.Emit(vehicle, previous, current);
}

// ------------------------------------------------------------------------------------------------
void Core::OnPickupRespawn(SQInt32 pickup) noexcept
{
    PickupRespawn.Emit(pickup);
    Reference< CPickup >::Get(pickup).PickupRespawn.Emit(pickup);
}

// ------------------------------------------------------------------------------------------------
void Core::OnPlayerKeyPress(SQInt32 player, SQInt32 keybind) noexcept
{
    KeybindKeyPress.Emit(player, keybind);
    Reference< CKeybind >::Get(keybind).KeybindKeyPress.Emit(player, keybind);
    Reference< CPlayer >::Get(player).KeybindKeyPress.Emit(player, keybind);
}

void Core::OnPlayerKeyRelease(SQInt32 player, SQInt32 keybind) noexcept
{
    KeybindKeyRelease.Emit(player, keybind);
    Reference< CKeybind >::Get(keybind).KeybindKeyRelease.Emit(player, keybind);
    Reference< CPlayer >::Get(player).KeybindKeyRelease.Emit(player, keybind);
}

// ------------------------------------------------------------------------------------------------
void Core::OnPlayerEmbarking(SQInt32 player, SQInt32 vehicle, SQInt32 slot) noexcept
{
    VehicleEmbarking.Emit(player, vehicle, slot);
    Reference< CVehicle >::Get(vehicle).VehicleEmbarking.Emit(player, vehicle, slot);
    Reference< CPlayer >::Get(player).VehicleEmbarking.Emit(player, vehicle, slot);
}

void Core::OnPlayerEmbarked(SQInt32 player, SQInt32 vehicle, SQInt32 slot) noexcept
{
    VehicleEmbarked.Emit(player, vehicle, slot);
    Reference< CVehicle >::Get(vehicle).VehicleEmbarked.Emit(player, vehicle, slot);
    Reference< CPlayer >::Get(player).VehicleEmbarked.Emit(player, vehicle, slot);
}

void Core::OnPlayerDisembark(SQInt32 player, SQInt32 vehicle) noexcept
{
    VehicleDisembark.Emit(player, vehicle);
    Reference< CVehicle >::Get(vehicle).VehicleDisembark.Emit(player, vehicle);
    Reference< CPlayer >::Get(player).VehicleDisembark.Emit(player, vehicle);
}

// ------------------------------------------------------------------------------------------------
void Core::OnPickupClaimed(SQInt32 player, SQInt32 pickup) noexcept
{
    PickupClaimed.Emit(player, pickup);
    Reference< CPickup >::Get(pickup).PickupClaimed.Emit(player, pickup);
    Reference< CPlayer >::Get(player).PickupClaimed.Emit(player, pickup);
}

void Core::OnPickupCollected(SQInt32 player, SQInt32 pickup) noexcept
{
    PickupClaimed.Emit(player, pickup);
    Reference< CPickup >::Get(pickup).PickupClaimed.Emit(player, pickup);
    Reference< CPlayer >::Get(player).PickupClaimed.Emit(player, pickup);
}

// ------------------------------------------------------------------------------------------------
void Core::OnObjectShot(SQInt32 player, SQInt32 object, SQInt32 weapon) noexcept
{
    ObjectShot.Emit(player, object, weapon);
    Reference< CObject >::Get(object).ObjectShot.Emit(player, object, weapon);
    Reference< CPlayer >::Get(player).ObjectShot.Emit(player, object, weapon);
}

void Core::OnObjectBump(SQInt32 player, SQInt32 object) noexcept
{
    ObjectBump.Emit(player, object);
    Reference< CObject >::Get(object).ObjectBump.Emit(player, object);
    Reference< CPlayer >::Get(player).ObjectBump.Emit(player, object);
}

// ------------------------------------------------------------------------------------------------
void Core::OnCheckpointEntered(SQInt32 player, SQInt32 checkpoint) noexcept
{
    CheckpointEntered.Emit(player, checkpoint);
    Reference< CCheckpoint >::Get(checkpoint).CheckpointEntered.Emit(player, checkpoint);
    Reference< CPlayer >::Get(player).CheckpointEntered.Emit(player, checkpoint);
}

void Core::OnCheckpointExited(SQInt32 player, SQInt32 checkpoint) noexcept
{
    CheckpointExited.Emit(player, checkpoint);
    Reference< CCheckpoint >::Get(checkpoint).CheckpointExited.Emit(player, checkpoint);
    Reference< CPlayer >::Get(player).CheckpointExited.Emit(player, checkpoint);
}

// ------------------------------------------------------------------------------------------------
void Core::OnSphereEntered(SQInt32 player, SQInt32 sphere) noexcept
{
    SphereEntered.Emit(player, sphere);
    Reference< CSphere >::Get(sphere).SphereEntered.Emit(player, sphere);
    Reference< CPlayer >::Get(player).SphereEntered.Emit(player, sphere);
}

void Core::OnSphereExited(SQInt32 player, SQInt32 sphere) noexcept
{
    SphereExited.Emit(player, sphere);
    Reference< CSphere >::Get(sphere).SphereExited.Emit(player, sphere);
    Reference< CPlayer >::Get(player).SphereExited.Emit(player, sphere);
}

// ------------------------------------------------------------------------------------------------
void Core::OnServerFrame(SQFloat delta) noexcept
{
    ServerFrame.Emit(delta);
}

// ------------------------------------------------------------------------------------------------
void Core::OnServerStartup() noexcept
{
    ServerStartup.Emit();
}

void Core::OnServerShutdown() noexcept
{
    ServerShutdown.Emit();
}

// ------------------------------------------------------------------------------------------------
void Core::OnInternalCommand(SQInt32 type, const SQChar * text) noexcept
{
    InternalCommand.Emit(type, text);
}

void Core::OnLoginAttempt(const SQChar * name, const SQChar * passwd, const SQChar * ip) noexcept
{
    LoginAttempt.Emit(name, passwd, ip);
}

// ------------------------------------------------------------------------------------------------
void Core::OnCustomEvent(SQInt32 group, SQInt32 header, SqObj & payload) noexcept
{
    CustomEvent.Emit(group, header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::OnWorldOption(SQInt32 option, SqObj & value) noexcept
{
    WorldOption.Emit(option, value);
}

void Core::OnWorldToggle(SQInt32 option, bool value) noexcept
{
    WorldToggle.Emit(option, value);
}

// ------------------------------------------------------------------------------------------------
void Core::OnScriptReload(SQInt32 header, SqObj & payload) noexcept
{
    ScriptReload.Emit(header, payload);
}

// ------------------------------------------------------------------------------------------------
void Core::OnLogMessage(SQInt32 type, const SQChar * message) noexcept
{
    LogMessage.Emit(type, message);
}

// ------------------------------------------------------------------------------------------------
void Core::OnPlayerUpdate(SQInt32 player, SQInt32 type) noexcept
{
    SQMOD_UNUSED_VAR(type);
    Vector3 pos;
    // Is this player instance tracked for the first time
    if (m_PlayerTrack[player].Fresh)
    {
        // Obtain the current position of this instance
        _Func->GetPlayerPos(player, &pos.x, &pos.y, &pos.z);
        // Initialize the tracked values for the first time
        m_PlayerTrack[player].Position =  pos;
        m_PlayerTrack[player].Health = _Func->GetPlayerHealth(player);
        m_PlayerTrack[player].Armour = _Func->GetPlayerArmour(player);
        m_PlayerTrack[player].Weapon = _Func->GetPlayerWeapon(player);
        m_PlayerTrack[player].Fresh = false;
        // No need to check a freshly tracked instance
        return;
    }
    // Obtain the current position of this instance
    _Func->GetPlayerPos(player, &pos.x, &pos.y, &pos.z);
    // Did the position change since the last tracked value?
    if (pos != m_PlayerTrack[player].Position)
    {
        // Trigger the specific event
        PlayerMove.Emit(player, m_PlayerTrack[player].Position, pos);
        // Update the tracked value
        m_PlayerTrack[player].Position = pos;
    }
    // Obtain the current health of this instance
    SQFloat health = _Func->GetPlayerHealth(player);
    // Did the health change since the last tracked value?
    if (!EpsEq(health, m_PlayerTrack[player].Health))
    {
        // Trigger the specific event
        PlayerHealth.Emit(player, m_PlayerTrack[player].Health, health);
        // Update the tracked value
        m_PlayerTrack[player].Health = health;
    }
    // Obtain the current armor of this instance
    SQFloat armour = _Func->GetPlayerArmour(player);
    // Did the armor change since the last tracked value?
    if (!EpsEq(armour, m_PlayerTrack[player].Armour))
    {
        // Trigger the specific event
        PlayerArmour.Emit(player, m_PlayerTrack[player].Armour, armour);
        // Update the tracked value
        m_PlayerTrack[player].Armour = armour;
    }
    // Obtain the current weapon of this instance
    SQInteger wep = _Func->GetPlayerWeapon(player);
    // Did the weapon change since the last tracked value?
    if (wep != m_PlayerTrack[player].Weapon)
    {
        // Trigger the specific event
        PlayerWeapon.Emit(player, m_PlayerTrack[player].Weapon, wep);
        // Update the tracked value
        m_PlayerTrack[player].Weapon = wep;
    }
}

void Core::OnVehicleUpdate(SQInt32 vehicle, SQInt32 type) noexcept
{
    SQMOD_UNUSED_VAR(type);
    Vector3 pos;
    // Is this vehicle instance tracked for the first time
    if (m_VehicleTrack[vehicle].Fresh)
    {
        // Obtain the current position of this instance
        _Func->GetVehiclePos(vehicle, &pos.x, &pos.y, &pos.z);
        // Initialize the tracked values for the first time
        m_VehicleTrack[vehicle].Position =  pos;
        m_VehicleTrack[vehicle].Health = _Func->GetVehicleHealth(vehicle);
        m_VehicleTrack[vehicle].Fresh = false;
        // No need to check a freshly tracked instance
        return;
    }
    // Obtain the current position of this instance
    _Func->GetVehiclePos(vehicle, &pos.x, &pos.y, &pos.z);
    // Did the position change since the last tracked value?
    if (pos != m_VehicleTrack[vehicle].Position)
    {
        // Trigger the specific event
        VehicleMove.Emit(vehicle, m_VehicleTrack[vehicle].Position, pos);
        // Update the tracked value
        m_VehicleTrack[vehicle].Position = pos;
    }
    // Obtain the current health of this instance
    SQFloat health = _Func->GetVehicleHealth(vehicle);
    // Did the health change since the last tracked value?
    if (!EpsEq(health, m_VehicleTrack[vehicle].Health))
    {
        // Trigger the specific event
        VehicleHealth.Emit(vehicle, m_VehicleTrack[vehicle].Health, health);
        // Update the tracked value
        m_VehicleTrack[vehicle].Health = health;
    }
}

void Core::OnEntityPool(SQInt32 type, SQInt32 id, bool deleted) noexcept
{
    // Script object to play the role of a dummy payload
    static SqObj payload;
    // Make sure that the payload is null
    payload.Release();
    // See what type of change happened in the entity pool
    switch (type)
    {
        case SQMOD_ENTITY_POOL_VEHICLE:
            if (deleted)
            {
                EntMan< CVehicle >::Deactivate(id, SQMOD_DESTROY_POOL, payload, true);
            }
            else if (EntMan< CVehicle >::Activate(id, false))
            {
                OnVehicleCreated(id, SQMOD_CREATE_POOL, payload);
            }
        break;
        case SQMOD_ENTITY_POOL_OBJECT:
            if (deleted)
            {
                EntMan< CObject >::Deactivate(id, SQMOD_DESTROY_POOL, payload, true);
            }
            else if (EntMan< CObject >::Activate(id, false))
            {
                OnObjectCreated(id, SQMOD_CREATE_POOL, payload);
            }
        break;
        case SQMOD_ENTITY_POOL_PICKUP:
            if (deleted)
            {
                EntMan< CPickup >::Deactivate(id, SQMOD_DESTROY_POOL, payload, true);
            }
            else if (EntMan< CPickup >::Activate(id, false))
            {
                OnPickupCreated(id, SQMOD_CREATE_POOL, payload);
            }
        break;
        case SQMOD_ENTITY_POOL_RADIO:
            // @TODO Implement...
        break;
        case SQMOD_ENTITY_POOL_SPRITE:
            if (deleted)
            {
                EntMan< CSprite >::Deactivate(id, SQMOD_DESTROY_POOL, payload, true);
            }
            else if (EntMan< CSprite >::Activate(id, false))
            {
                OnSpriteCreated(id, SQMOD_CREATE_POOL, payload);
            }
        break;
        case SQMOD_ENTITY_POOL_TEXTDRAW:
            if (deleted)
            {
                EntMan< CTextdraw >::Deactivate(id, SQMOD_DESTROY_POOL, payload, true);
            }
            else if (EntMan< CTextdraw >::Activate(id, false))
            {
                OnTextdrawCreated(id, SQMOD_CREATE_POOL, payload);
            }
        break;
        case SQMOD_ENTITY_POOL_BLIP:
            if (deleted)
            {
                EntMan< CBlip >::Deactivate(id, SQMOD_DESTROY_POOL, payload, true);
            }
            else
            {
                SQInt32 world, scale, sprid;
                SQUint32 color;
                SQFloat x, y, z;
                // Get the blip information from the server
                _Func->GetCoordBlipInfo(id, &world, &x, &y, &z, &scale, &color, &sprid);
                // Attempt to activate this instance
                if (EntMan< CBlip >::Activate(id, false, SQMOD_UNKNOWN, world, x, y, z, scale, color, sprid))
                {
                    OnBlipCreated(id, SQMOD_CREATE_POOL, payload);
                }
            }
        break;
        default:
            LogErr("Unknown change in the entity pool of type: %d", type);
    }

}


} // Namespace:: SqMod
