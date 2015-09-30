#include "Core.hpp"
#include "Logger.hpp"
#include "Entity.hpp"
#include "Register.hpp"

// ------------------------------------------------------------------------------------------------
#include "Base/Color3.hpp"
#include "Base/Vector2i.hpp"
#include "Misc/Automobile.hpp"
#include "Misc/Model.hpp"

// ------------------------------------------------------------------------------------------------
#include <SimpleIni.h>
#include <format.h>

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
    this->Terminate();
}

// ------------------------------------------------------------------------------------------------
void Core::_Finalizer(Core * ptr) noexcept
{
    if (ptr) delete ptr;
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
    _Log->Msg("%s", CenterStr("INITIALIZING", '*'));

    if (!this->Configure() || !this->CreateVM() || !this->LoadScripts())
    {
        return false;
    }

    _Log->Msg("%s", CenterStr("SUCCESS", '*'));

    return true;
}

bool Core::Load() noexcept
{
    _Log->Msg("%s", CenterStr("LOADING", '*'));

    if (!this->Execute())
    {
        return false;
    }

    _Log->Msg("%s", CenterStr("SUCCESS", '*'));

    return true;
}

// ------------------------------------------------------------------------------------------------
void Core::Deinit() noexcept
{
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
string Core::GetOption(const string & name) const noexcept
{
    OptionPool::const_iterator elem = m_Options.find(name);
    return (elem == m_Options.cend()) ? string() : elem->second;
}

void Core::SetOption(const string & name, const string & value) noexcept
{
    m_Options[name] = value;
}

// ------------------------------------------------------------------------------------------------
Core::Buffer Core::PullBuffer(unsigned sz) noexcept
{
    Buffer buf;
    if (m_BufferPool.empty())
    {
        buf.resize(sz);
    }
    else
    {
        buf = std::move(m_BufferPool.back());
        m_BufferPool.pop();
    }
    return std::move(buf);
}

// ------------------------------------------------------------------------------------------------
void Core::PushBuffer(Buffer && buf) noexcept
{
    m_BufferPool.push(std::move(buf));
}

// ------------------------------------------------------------------------------------------------
void Core::MakeBuffer(unsigned num, unsigned sz) noexcept
{
    while (num--)
    {
        m_BufferPool.emplace(sz);
    }
}

// ------------------------------------------------------------------------------------------------
void Core::ConnectPlayer(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
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

void Core::DisconnectPlayer(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
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
    _Log->Dbg("Attempting to instantiate the configuration file");

    if (g_Config)
    {
        g_Config->Reset();
    }
    else
    {
        g_Config.reset(new CSimpleIniA(true, true, true));
    }

    if (!g_Config)
    {
        _Log->Ftl("Unable to instantiate the configuration class");

        return false;
    }

    _Log->Dbg("Attempting to load the configuration file.");

    SI_Error ini_ret = g_Config->LoadFile(_SC("./sqmod.ini"));

    if (ini_ret < 0)
    {
        switch (ini_ret)
        {
            case SI_FAIL:   _Log->Err("Failed to load the configuration file. Probably invalid"); break;
            case SI_NOMEM:  _Log->Err("Run out of memory while loading the configuration file"); break;
            case SI_FILE:   _Log->Err("Failed to load the configuration file. %s", strerror(errno)); break;
            default:        _Log->Err("Failed to load the configuration file for some unforeseen reason");
        }
        return false;
    }

    _Log->Dbg("Applying the specified logging filters");

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

    _Log->Dbg("Reading the options from the general section");

    CSimpleIniA::TNamesDepend general_options;

    if (g_Config->GetAllKeys("Options", general_options) || general_options.size() > 0)
    {
        for (auto const & cfg_option : general_options)
        {
            CSimpleIniA::TNamesDepend option_list;

            if (!g_Config->GetAllValues("Options", cfg_option.pItem, option_list))
            {
                continue;
            }

            option_list.sort(CSimpleIniA::Entry::LoadOrder());

            for (auto const & cfg_value : option_list)
            {
                m_Options[cfg_option.pItem] = cfg_value.pItem;
            }
        }
    }
    else
    {
        _Log->Inf("No options specified in the configuration file");
    }

    return true;
}

// ------------------------------------------------------------------------------------------------
bool Core::CreateVM() noexcept
{
    _Log->Dbg("Acquiring the virtual machine stack size");

    // Start with an unknown stack size
    SQInteger stack_size = SQMOD_UNKNOWN;

    // Attempt to get it from the configuration file
    try
    {
        stack_size = SToI<SQInteger>::Fn(GetOption(_SC("VMStackSize")), 0, 10);
    }
    catch (const std::invalid_argument & e)
    {
        _Log->Wrn("Unable to extract option value: %s", e.what());
    }

    // Validate the stack size
    if (stack_size <= 0)
    {
        _Log->Wrn("Invalid stack size. Reverting to default size");
        SetOption(_SC("VMStackSize"), std::to_string(SQMOD_STACK_SIZE));
        stack_size = SQMOD_STACK_SIZE;
    }

    _Log->Inf("Creating a virtual machine with a stack size of: %d", stack_size);

    m_VM = sq_open(stack_size);

    if (!m_VM)
    {
        _Log->Ftl("Unable to create the virtual machine");

        return false;
    }
    else
    {
        DefaultVM::Set(m_VM);
        ErrorHandling::Enable(true);
        m_RootTable.reset(new RootTable(m_VM));
        m_Scripts.clear();
    }

    _Log->Dbg("Registering the standard libraries");

    sq_pushroottable(m_VM);
    sqstd_register_iolib(m_VM);
    sqstd_register_bloblib(m_VM);
    sqstd_register_mathlib(m_VM);
    sqstd_register_systemlib(m_VM);
    sqstd_register_stringlib(m_VM);
    sq_pop(m_VM, 1);

    _Log->Dbg("Setting the base output function");

    sq_setprintfunc(m_VM, PrintFunc, ErrorFunc);

    _Log->Dbg("Setting the base error handlers");

    sq_setcompilererrorhandler(m_VM, CompilerErrorHandler);
    sq_newclosure(m_VM, RuntimeErrorHandler, 0);
    sq_seterrorhandler(m_VM);

    _Log->Dbg("Registering the plugin API");

    if (!RegisterAPI(m_VM))
    {
        _Log->Ftl("Unable to register the script API");
        return false;
    }

    return true;
}

void Core::DestroyVM() noexcept
{
    if (m_VM) sq_close(m_VM);
}

// ------------------------------------------------------------------------------------------------
bool Core::LoadScripts() noexcept
{
    _Log->Dbg("Attempting to compile the specified scripts");

    if (!g_Config)
    {
        _Log->Wrn("Cannot compile any scripts without the configurations");

        return false;
    }

    CSimpleIniA::TNamesDepend script_list;
    g_Config->GetAllValues("Scripts", "Source", script_list);

    if (script_list.size() <= 0)
    {
        _Log->Wrn("No scripts specified in the configuration file");

        return false;
    }

    script_list.sort(CSimpleIniA::Entry::LoadOrder());

    for (auto const & cfg_script : script_list)
    {
        string path(cfg_script.pItem);

        if (m_Scripts.find(path) != m_Scripts.cend())
        {
            _Log->Wrn("Script was already loaded: %s", path.c_str());

            continue;
        }
        else if (!Compile(path))
        {
            return false;
        }
        else
        {
            _Log->Scs("Successfully compiled script: %s", path.c_str());
        }
    }

    if (m_Scripts.empty())
    {
        _Log->Err("No scripts compiled. No reason to load the plugin");

        return false;
    }

    return true;
}

// ------------------------------------------------------------------------------------------------
bool Core::Compile(const string & name) noexcept
{
    if (name.empty())
    {
        _Log->Err("Cannot compile script without a valid name");

        return false;
    }

    std::pair< SqScriptPool::iterator, bool > res = m_Scripts.emplace(name, Script(m_VM));

    if (res.second)
    {
        res.first->second.CompileFile(name);

        if (Error::Occurred(m_VM))
        {
            _Log->Err("Unable to compile script: %s", name.c_str());
            _Log->Inf("=> %s", Error::Message(m_VM).c_str());
            m_Scripts.erase(res.first);

            return false;
        }
    }
    else
    {
        _Log->Err("Unable to queue script: %s", name.c_str());

        return false;
    }

    return true;
}

bool Core::Execute() noexcept
{
    _Log->Dbg("Attempting to execute the specified scripts");

    for (auto & elem : m_Scripts)
    {
        elem.second.Run();

        if (Error::Occurred(m_VM))
        {
            _Log->Err("Unable to execute script: %s", elem.first.c_str());
            _Log->Inf("=> %s", Error::Message(m_VM).c_str());

            return false;
        }
        else
        {
            _Log->Scs("Successfully executed script: %s", elem.first.c_str());
        }
    }

    return true;
}

// ------------------------------------------------------------------------------------------------
void Core::PrintCallstack() noexcept
{
    SQStackInfos si;


    _Log->Msg("%s", CenterStr("CALLSTACK", '*'));

    for (SQInteger level = 1; SQ_SUCCEEDED(sq_stackinfos(m_VM, level, &si)); ++level)
    {
        _Log->Inf("FUNCTION %s()", si.funcname ? si.funcname : _SC("unknown"));
        _Log->SInf("=> [%d] : {%s}", si.line, si.source ? si.source : _SC("unknown"));
    }

    const SQChar * s_ = 0, * name = 0;
    SQInteger i_, seq = 0;
    SQFloat f_;
    SQUserPointer p_;

    _Log->Msg("%s", CenterStr("LOCALS", '*'));

    for (SQInteger level = 0; level < 10; level++) {
        seq = 0;
        while((name = sq_getlocal(m_VM, level, seq))) {
            ++seq;
            switch(sq_gettype(m_VM, -1))
            {
                case OT_NULL:
                    _Log->Inf("NULL [%s] : ...", name);
                    break;
                case OT_INTEGER:
                    sq_getinteger(m_VM, -1, &i_);
                    _Log->Inf("INTEGER [%s] : {%d}", name, i_);
                    break;
                case OT_FLOAT:
                    sq_getfloat(m_VM, -1, &f_);
                    _Log->Inf("FLOAT [%s] : {%f}", name, f_);
                    break;
                case OT_USERPOINTER:
                    sq_getuserpointer(m_VM, -1, &p_);
                    _Log->Inf("USERPOINTER [%s] : {%p}\n", name, p_);
                    break;
                case OT_STRING:
                    sq_getstring(m_VM, -1, &s_);
                    _Log->Inf("STRING [%s] : {%s}", name, s_);
                    break;
                case OT_TABLE:
                    _Log->Inf("TABLE [%s] : ...", name);
                    break;
                case OT_ARRAY:
                    _Log->Inf("ARRAY [%s] : ...", name);
                    break;
                case OT_CLOSURE:
                    _Log->Inf("CLOSURE [%s] : ...", name);
                    break;
                case OT_NATIVECLOSURE:
                    _Log->Inf("NATIVECLOSURE [%s] : ...", name);
                    break;
                case OT_GENERATOR:
                    _Log->Inf("GENERATOR [%s] : ...", name);
                    break;
                case OT_USERDATA:
                    _Log->Inf("USERDATA [%s] : ...", name);
                    break;
                case OT_THREAD:
                    _Log->Inf("THREAD [%s] : ...", name);
                    break;
                case OT_CLASS:
                    _Log->Inf("CLASS [%s] : ...", name);
                    break;
                case OT_INSTANCE:
                    _Log->Inf("INSTANCE [%s] : ...", name);
                    break;
                case OT_WEAKREF:
                    _Log->Inf("WEAKREF [%s] : ...", name);
                    break;
                case OT_BOOL:
                    sq_getinteger(m_VM, -1, &i_);
                    _Log->Inf("BOOL [%s] : {%s}", name, i_ ? _SC("true") : _SC("false"));
                    break;
                default:
                    _Log->Err("UNKNOWN [%s] : ...", name);
                break;
            }
            sq_pop(m_VM, 1);
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Core::PrintFunc(HSQUIRRELVM vm, const SQChar * str, ...) noexcept
{
    va_list args;
    va_start(args, str);

    std::vector<char> buffer(256);
    SQInt32 fmt_ret = std::vsnprintf(&buffer[0], buffer.size(), str, args);

    if (fmt_ret < 0)
    {
        return;
    }
    else if (static_cast<std::size_t>(fmt_ret) > buffer.size())
    {
        buffer.resize(++fmt_ret);
        vsnprintf(&buffer[0], buffer.size(), str, args);
    }

    va_end(args);

    _Log->Msg("%s", buffer.data());
}

void Core::ErrorFunc(HSQUIRRELVM vm, const SQChar * str, ...) noexcept
{
    va_list args;
    va_start(args, str);

    std::vector<char> buffer(256);
    std::size_t buff_sz = static_cast<std::size_t>(vsnprintf(&buffer[0], buffer.size(), str, args));

    if (buff_sz > buffer.size())
    {
        buffer.resize(++buff_sz);
        vsnprintf(&buffer[0], buffer.size(), str, args);
    }

    va_end(args);

    _Log->Err("%s", buffer.data());
}

// ------------------------------------------------------------------------------------------------
SQInteger Core::RuntimeErrorHandler(HSQUIRRELVM vm) noexcept
{
    if (sq_gettop(vm) < 1)
    {
        return 0;
    }

    const SQChar * err_msg = NULL;

    if (SQ_SUCCEEDED(sq_getstring(vm, 2, &err_msg)))
    {
        _Core->m_ErrorMsg.assign(err_msg);
    }
    else
    {
        _Core->m_ErrorMsg.assign(_SC("An unknown runtime error has occurred"));
    }

    _Log->Msg("%s", CenterStr("ERROR", '*'));

    _Log->Inf("[MESSAGE] : %s", _Core->m_ErrorMsg.c_str());

    if (_Log->GetVerbosity() > 0)
    {
        _Core->PrintCallstack();
    }

    _Log->Msg("%s", CenterStr("CONCLUDED", '*'));

    return SQ_OK;
}

void Core::CompilerErrorHandler(HSQUIRRELVM vm, const SQChar * desc, const SQChar * src, SQInteger line, SQInteger column) noexcept
{
    try
    {
        _Core->m_ErrorMsg.assign(fmt::format("{0:s} : {1:d}:{2:d} : {3:s}", src, line, column, desc).c_str());
    }
    catch (const std::exception & e)
    {
        _Log->Err("Compiler error: %s", e.what());
        _Core->m_ErrorMsg.assign(_SC("An unknown compiler error has occurred"));
    }

    _Log->Err("%s", _Core->m_ErrorMsg.c_str());
}

// ------------------------------------------------------------------------------------------------
Reference< CBlip > Core::CreateBlip(SQInt32 index, SQInt32 world, const Vector3 & pos, SQInt32 scale, \
                        const Color4 & color, SQInt32 sprite, SQInt32 header, const SqObj & payload) noexcept
{
    // Attempt to create an instance on the server and obtain it's identifier
    SQInt32 id = _Func->CreateCoordBlip(index, world, pos.x, pos.y, pos.z, scale, color.GetRGBA(), sprite);
    // Attempt to activate the instance in the plugin at the received identifier
    if (EntMan< CBlip >::Activate(id, true, world, scale, sprite, pos, color))
    {
        // Trigger the specific event
        OnBlipCreated(id, header, payload);
    }
    else
    {
        LogErr("Unable to create a new <CBlip> instance");
    }
    // Return the obtained reference as is
    return Reference< CBlip >(id);
}

// ------------------------------------------------------------------------------------------------
Reference< CCheckpoint > Core::CreateCheckpoint(const Reference< CPlayer > & player, SQInt32 world, const Vector3 & pos, \
                        const Color4 & color, SQFloat radius, SQInt32 header, const SqObj & payload) noexcept
{
    // See if the specified player reference is valid
    if (!player)
    {
        LogWrn("Attempting to create a <Checkpoint> instance on an invalid player: %d", _SCI32(player));
    }
    // Attempt to create an instance on the server and obtain it's identifier
    SQInt32 id = _Func->CreateCheckpoint(_SCI32(player), world, pos.x, pos.y, pos.z, color.r, color.g, color.b, color.a, radius);
    // Attempt to activate the instance in the plugin at the received identifier
    if (EntMan< CCheckpoint >::Activate(id, true))
    {
        // Trigger the specific event
        OnCheckpointCreated(id, header, payload);
    }
    else
    {
        LogErr("Unable to create a new <CCheckpoint> instance");
    }
    // Return the obtained reference as is
    return Reference< CCheckpoint >(id);
}

// ------------------------------------------------------------------------------------------------
Reference< CKeybind > Core::CreateKeybind(SQInt32 slot, bool release, SQInt32 primary, SQInt32 secondary, \
                        SQInt32 alternative, SQInt32 header, const SqObj & payload) noexcept
{
    // Attempt to create an instance on the server and obtain it's identifier
    SQInt32 id = _Func->RegisterKeyBind(slot, release, primary, secondary, alternative);
    // Attempt to activate the instance in the plugin at the received identifier
    if (EntMan< CKeybind >::Activate(id, true, primary, secondary, alternative, release))
    {
        // Trigger the specific event
        OnKeybindCreated(id, header, payload);
    }
    else
    {
        LogErr("Unable to create a new <CKeybind> instance");
    }
    // Return the obtained reference as is
    return Reference< CKeybind >(id);
}

// ------------------------------------------------------------------------------------------------
Reference< CObject > Core::CreateObject(const CModel & model, SQInt32 world, const Vector3 & pos, SQInt32 alpha, \
                        SQInt32 header, const SqObj & payload) noexcept
{
    // See if the specified model is valid
    if (!model)
    {
        LogWrn("Attempting to create an <Object> instance with an invalid model: %d", _SCI32(model));
    }
    // Attempt to create an instance on the server and obtain it's identifier
    SQInt32 id = _Func->CreateObject(_SCI32(model), world, pos.x, pos.y, pos.z, alpha);
    // Attempt to activate the instance in the plugin at the received identifier
    if (EntMan< CObject >::Activate(id, true))
    {
        // Trigger the specific event
        OnObjectCreated(id, header, payload);
    }
    else
    {
        LogErr("Unable to create a new <CObject> instance");
    }
    // Return the obtained reference as is
    return Reference< CObject >(id);
}

// ------------------------------------------------------------------------------------------------
Reference< CPickup > Core::CreatePickup(const CModel & model, SQInt32 world, SQInt32 quantity, const Vector3 & pos, \
                        SQInt32 alpha, bool automatic, SQInt32 header, const SqObj & payload) noexcept
{
    // See if the specified model is valid
    if (!model)
    {
        LogWrn("Attempting to create a <Pickup> instance with an invalid model: %d", _SCI32(model));
    }
    // Attempt to create an instance on the server and obtain it's identifier
    SQInt32 id = _Func->CreatePickup(_SCI32(model), world, quantity, pos.x, pos.y, pos.z, alpha, automatic);
    // Attempt to activate the instance in the plugin at the received identifier
    if (EntMan< CPickup >::Activate(id, true))
    {
        // Trigger the specific event
        OnPickupCreated(id, header, payload);
    }
    else
    {
        LogErr("Unable to create a new <CPickup> instance");
    }
    // Return the obtained reference as is
    return Reference< CPickup >(id);
}

// ------------------------------------------------------------------------------------------------
Reference< CSphere > Core::CreateSphere(const Reference< CPlayer > & player, SQInt32 world, const Vector3 & pos, \
                        const Color3 & color, SQFloat radius, SQInt32 header, const SqObj & payload) noexcept
{
    // See if the specified player reference is valid
    if (!player)
    {
        LogWrn("Attempting to create a <Sphere> instance on an invalid player: %d", _SCI32(player));
    }
    // Attempt to create an instance on the server and obtain it's identifier
    SQInt32 id = _Func->CreateSphere(_SCI32(player), world, pos.x, pos.y, pos.z, color.r, color.g, color.b, radius);
    // Attempt to activate the instance in the plugin at the received identifier
    if (EntMan< CSphere >::Activate(id, true))
    {
        // Trigger the specific event
        OnSphereCreated(id, header, payload);
    }
    else
    {
        LogErr("Unable to create a new <CSphere> instance");
    }
    // Return the obtained reference as is
    return Reference< CSphere >(id);
}

// ------------------------------------------------------------------------------------------------
Reference< CSprite > Core::CreateSprite(SQInt32 index, const String & file, const Vector2i & pos, const Vector2i & rot, \
                        SQFloat angle, SQInt32 alpha, bool rel, SQInt32 header, const SqObj & payload) noexcept
{
    // See if the specified file path is valid
    if (file.empty())
    {
        LogWrn("Attempting to create a <Sprite> instance with an empty path: %d", file.size());
    }
    // Attempt to create an instance on the server and obtain it's identifier
    SQInt32 id = _Func->CreateSprite(index, file.c_str(), pos.x, pos.y, rot.x, rot.y, angle, alpha, rel);
    // Attempt to activate the instance in the plugin at the received identifier
    if (EntMan< CSprite >::Activate(id, true, file))
    {
        // Trigger the specific event
        OnSpriteCreated(id, header, payload);
    }
    else
    {
        LogErr("Unable to create a new <CSprite> instance");
    }
    // Return the obtained reference as is
    return Reference< CSprite >(id);
}

// ------------------------------------------------------------------------------------------------
Reference< CTextdraw > Core::CreateTextdraw(SQInt32 index, const String & text, const Vector2i & pos, \
                        const Color4 & color, bool rel, SQInt32 header, const SqObj & payload) noexcept
{
    // See if the specified text is valid
    if (text.empty())
    {
        LogWrn("Attempting to create a <Textdraw> instance with an empty text: %d", text.size());
    }
    // Attempt to create an instance on the server and obtain it's identifier
    SQInt32 id = _Func->CreateTextdraw(index, text.c_str(), pos.x, pos.y, color.GetRGBA(), rel);
    // Attempt to activate the instance in the plugin at the received identifier
    if (EntMan< CTextdraw >::Activate(id, true, text))
    {
        // Trigger the specific event
        OnTextdrawCreated(id, header, payload);
    }
    else
    {
        LogErr("Unable to create a new <CTextdraw> instance");
    }
    // Return the obtained reference as is
    return Reference< CTextdraw >(id);
}

// ------------------------------------------------------------------------------------------------
Reference< CVehicle > Core::CreateVehicle(const CAutomobile & model, SQInt32 world, const Vector3 & pos, \
                        SQFloat angle, SQInt32 primary, SQInt32 secondary, SQInt32 header, const SqObj & payload) noexcept
{
    // See if the specified model is valid
    if (!model)
    {
        LogWrn("Attempting to create an <Vehicle> instance with an invalid model: %d", _SCI32(model));
    }
    // Attempt to create an instance on the server and obtain it's identifier
    SQInt32 id = _Func->CreateVehicle(_SCI32(model), world, pos.x, pos.y, pos.z, angle, primary, secondary);
    // Attempt to activate the instance in the plugin at the received identifier
    if (EntMan< CVehicle >::Activate(id, true))
    {
        // Trigger the specific event
        OnVehicleCreated(id, header, payload);
    }
    else
    {
        LogErr("Unable to create a new <CVehicle> instance");
    }
    // Return the obtained reference as is
    return Reference< CVehicle >(id);
}

// ------------------------------------------------------------------------------------------------
bool Core::DestroyBlip(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

    return true;
}

// ------------------------------------------------------------------------------------------------
bool Core::DestroyCheckpoint(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Core::DestroyKeybind(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Core::DestroyObject(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Core::DestroyPickup(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Core::DestroyPlayer(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Core::DestroySphere(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Core::DestroySprite(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Core::DestroyTextdraw(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{
    return true;
}

// ------------------------------------------------------------------------------------------------
bool Core::DestroyVehicle(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{
    return true;
}

// ------------------------------------------------------------------------------------------------
void Core::OnBlipCreated(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

void Core::OnCheckpointCreated(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

void Core::OnKeybindCreated(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

void Core::OnObjectCreated(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

void Core::OnPickupCreated(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

void Core::OnPlayerCreated(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

void Core::OnSphereCreated(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

void Core::OnSpriteCreated(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

void Core::OnTextdrawCreated(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

void Core::OnVehicleCreated(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnBlipDestroyed(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

void Core::OnCheckpointDestroyed(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

void Core::OnKeybindDestroyed(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

void Core::OnObjectDestroyed(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

void Core::OnPickupDestroyed(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

void Core::OnPlayerDestroyed(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

void Core::OnSphereDestroyed(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

void Core::OnSpriteDestroyed(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

void Core::OnTextdrawDestroyed(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

void Core::OnVehicleDestroyed(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnBlipCustom(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

void Core::OnCheckpointCustom(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

void Core::OnKeybindCustom(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

void Core::OnObjectCustom(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

void Core::OnPickupCustom(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

void Core::OnPlayerCustom(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

void Core::OnSphereCustom(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

void Core::OnSpriteCustom(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

void Core::OnTextdrawCustom(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

void Core::OnVehicleCustom(SQInt32 id, SQInt32 header, const SqObj & payload) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnPlayerAway(SQInt32 player, bool status) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnPlayerGameKeys(SQInt32 player, SQInt32 previous, SQInt32 current) noexcept
{

}

void Core::OnPlayerName(SQInt32 player, const SQChar * previous, const SQChar * current) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnPlayerRequestClass(SQInt32 player, SQInt32 offset) noexcept
{

}

void Core::OnPlayerRequestSpawn(SQInt32 player) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnPlayerSpawn(SQInt32 player) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnPlayerStartTyping(SQInt32 player) noexcept
{

}

void Core::OnPlayerStopTyping(SQInt32 player) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnPlayerChat(SQInt32 player, const SQChar * message) noexcept
{

}

void Core::OnPlayerCommand(SQInt32 player, const SQChar * command) noexcept
{

}

void Core::OnPlayerMessage(SQInt32 player, SQInt32 receiver, const SQChar * message) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnPlayerHealth(SQInt32 player, SQFloat previous, SQFloat current) noexcept
{

}

void Core::OnPlayerArmour(SQInt32 player, SQFloat previous, SQFloat current) noexcept
{

}

void Core::OnPlayerWeapon(SQInt32 player, SQInt32 previous, SQInt32 current) noexcept
{

}

void Core::OnPlayerMove(SQInt32 player, const Vector3 & previous, const Vector3 & current) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnPlayerWasted(SQInt32 player, SQInt32 reason) noexcept
{

}

void Core::OnPlayerKilled(SQInt32 player, SQInt32 killer, SQInt32 reason, SQInt32 body_part) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnPlayerSpectate(SQInt32 player, SQInt32 target) noexcept
{

}

void Core::OnPlayerCrashreport(SQInt32 player, const SQChar * report) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnPlayerBurning(SQInt32 player, bool state) noexcept
{

}

void Core::OnPlayerCrouching(SQInt32 player, bool state) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnPlayerState(SQInt32 player, SQInt32 previous, SQInt32 current) noexcept
{

}

void Core::OnPlayerAction(SQInt32 player, SQInt32 previous, SQInt32 current) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnStateNone(SQInt32 player, SQInt32 previous) noexcept
{

}

void Core::OnStateNormal(SQInt32 player, SQInt32 previous) noexcept
{

}

void Core::OnStateShooting(SQInt32 player, SQInt32 previous) noexcept
{

}

void Core::OnStateDriver(SQInt32 player, SQInt32 previous) noexcept
{

}

void Core::OnStatePassenger(SQInt32 player, SQInt32 previous) noexcept
{

}

void Core::OnStateEnterDriver(SQInt32 player, SQInt32 previous) noexcept
{

}

void Core::OnStateEnterPassenger(SQInt32 player, SQInt32 previous) noexcept
{

}

void Core::OnStateExitVehicle(SQInt32 player, SQInt32 previous) noexcept
{

}

void Core::OnStateUnspawned(SQInt32 player, SQInt32 previous) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnActionNone(SQInt32 player, SQInt32 previous) noexcept
{

}

void Core::OnActionNormal(SQInt32 player, SQInt32 previous) noexcept
{

}

void Core::OnActionAiming(SQInt32 player, SQInt32 previous) noexcept
{

}

void Core::OnActionShooting(SQInt32 player, SQInt32 previous) noexcept
{

}

void Core::OnActionJumping(SQInt32 player, SQInt32 previous) noexcept
{

}

void Core::OnActionLieDown(SQInt32 player, SQInt32 previous) noexcept
{

}

void Core::OnActionGettingUp(SQInt32 player, SQInt32 previous) noexcept
{

}

void Core::OnActionJumpVehicle(SQInt32 player, SQInt32 previous) noexcept
{

}

void Core::OnActionDriving(SQInt32 player, SQInt32 previous) noexcept
{

}

void Core::OnActionDying(SQInt32 player, SQInt32 previous) noexcept
{

}

void Core::OnActionWasted(SQInt32 player, SQInt32 previous) noexcept
{

}

void Core::OnActionEmbarking(SQInt32 player, SQInt32 previous) noexcept
{

}

void Core::OnActionDisembarking(SQInt32 player, SQInt32 previous) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnVehicleRespawn(SQInt32 vehicle) noexcept
{

}

void Core::OnVehicleExplode(SQInt32 vehicle) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnVehicleHealth(SQInt32 vehicle, SQFloat previous, SQFloat current) noexcept
{

}

void Core::OnVehicleMove(SQInt32 vehicle, const Vector3 & previous, const Vector3 & current) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnPickupRespawn(SQInt32 pickup) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnPlayerKeyPress(SQInt32 player, SQInt32 keybind) noexcept
{

}

void Core::OnPlayerKeyRelease(SQInt32 player, SQInt32 keybind) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnPlayerEmbarking(SQInt32 player, SQInt32 vehicle, SQInt32 slot) noexcept
{

}

void Core::OnPlayerEmbarked(SQInt32 player, SQInt32 vehicle, SQInt32 slot) noexcept
{

}

void Core::OnPlayerDisembark(SQInt32 player, SQInt32 vehicle) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnPickupClaimed(SQInt32 pickup, SQInt32 player) noexcept
{

}

void Core::OnPickupCollected(SQInt32 pickup, SQInt32 player) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnObjectShot(SQInt32 object, SQInt32 player, SQInt32 weapon) noexcept
{

}

void Core::OnObjectBump(SQInt32 object, SQInt32 player) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnCheckpointEntered(SQInt32 checkpoint, SQInt32 player) noexcept
{

}

void Core::OnCheckpointExited(SQInt32 checkpoint, SQInt32 player) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnSphereEntered(SQInt32 sphere, SQInt32 player) noexcept
{

}

void Core::OnSphereExited(SQInt32 sphere, SQInt32 player) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnServerFrame(SQFloat delta) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnServerStartup() noexcept
{

}

void Core::OnServerShutdown() noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnInternalCommand(SQInt32 type, const SQChar * text) noexcept
{

}

void Core::OnLoginAttempt(const SQChar * name, const SQChar * passwd, const SQChar * ip) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnCustomEvent(SQInt32 group, SQInt32 header, const SqObj & payload) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnWorldOption(SQInt32 option, const SqObj & value) noexcept
{

}

void Core::OnWorldToggle(SQInt32 option, bool value) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnScriptReload(SQInt32 header, const SqObj & payload) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnLogMessage(SQInt32 type, const SQChar * message) noexcept
{

}

// ------------------------------------------------------------------------------------------------
void Core::OnPlayerUpdate(SQInt32 player, SQInt32 type) noexcept
{

}

void Core::OnVehicleUpdate(SQInt32 vehicle, SQInt32 type) noexcept
{

}

void Core::OnEntityPool(SQInt32 type, SQInt32 id, bool deleted) noexcept
{
    switch (type)
    {
        case SQMOD_ENTITY_POOL_VEHICLE:
            if (deleted)
            {
                DestroyVehicle(id, SQMOD_DESTROY_POOL, SqObj());
            }
            else if (EntMan< CVehicle >::Activate(id, false))
            {
                OnVehicleCreated(id, SQMOD_CREATE_POOL, SqObj());
            }
        break;
        case SQMOD_ENTITY_POOL_OBJECT:
            if (deleted)
            {
                DestroyObject(id, SQMOD_DESTROY_POOL, SqObj());
            }
            else if (EntMan< CObject >::Activate(id, false))
            {
                OnObjectCreated(id, SQMOD_CREATE_POOL, SqObj());
            }
        break;
        case SQMOD_ENTITY_POOL_PICKUP:
            if (deleted)
            {
                DestroyPickup(id, SQMOD_DESTROY_POOL, SqObj());
            }
            else if (EntMan< CPickup >::Activate(id, false))
            {
                OnPickupCreated(id, SQMOD_CREATE_POOL, SqObj());
            }
        break;
        case SQMOD_ENTITY_POOL_RADIO:
            // @TODO Implement...
        break;
        case SQMOD_ENTITY_POOL_SPRITE:
            if (deleted)
            {
                DestroySprite(id, SQMOD_DESTROY_POOL, SqObj());
            }
            else if (EntMan< CSprite >::Activate(id, false, ""))
            {
                OnSpriteCreated(id, SQMOD_CREATE_POOL, SqObj());
            }
        break;
        case SQMOD_ENTITY_POOL_TEXTDRAW:
            if (deleted)
            {
                DestroyTextdraw(id, SQMOD_DESTROY_POOL, SqObj());
            }
            else if (EntMan< CTextdraw >::Activate(id, false, ""))
            {
                OnTextdrawCreated(id, SQMOD_CREATE_POOL, SqObj());
            }
        break;
        case SQMOD_ENTITY_POOL_BLIP:
            if (deleted)
            {
                DestroyBlip(id, SQMOD_DESTROY_POOL, SqObj());
            }
            else
            {
                SQInt32 world, scale, sprite;
                SQUint32 pcolor;
                Vector3 pos;
                Color4 color;
                // Get the blip information from the server
                _Func->GetCoordBlipInfo(id, &world, &pos.x, &pos.y, &pos.z, &scale, &pcolor, &sprite);
                // Unpack the retrieved color
                color.SetRGBA(pcolor);

                if (EntMan< CBlip >::Activate(id, false, world, scale, sprite, pos, color))
                {
                    OnBlipCreated(id, SQMOD_CREATE_POOL, SqObj());
                }
            }
        break;
        default:
            LogErr("Unknown change in the entity pool of type: %d", type);
    }

}


} // Namespace:: SqMod
