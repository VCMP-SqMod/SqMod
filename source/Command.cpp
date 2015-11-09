#include "Command.hpp"
#include "Register.hpp"
#include "Core.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdlib>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const CmdManager::Pointer _Cmd = CmdManager::Inst();

// ------------------------------------------------------------------------------------------------
void CmdManager::_Finalizer(CmdManager * ptr)
{
    delete ptr; /* Assuming 'delete' checks for NULL */
}

// ------------------------------------------------------------------------------------------------
CmdManager::Pointer CmdManager::Inst()
{
    if (!_Cmd)
    {
        return Pointer(new CmdManager(), &CmdManager::_Finalizer);
    }

    return Pointer(nullptr, &CmdManager::_Finalizer);
}

// ------------------------------------------------------------------------------------------------
CmdManager::CmdManager()
    : m_Commands(), m_Invoker(SQMOD_UNKNOWN), m_Argv()
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
CmdManager::~CmdManager()
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
void CmdManager::VMClose()
{
    // Release the reference to the specified callbacks
    m_OnError.Release2();
    // We must not keep arguments
    m_Argv.fill(CmdArgs::value_type(CMDARG_ANY, NullData()));
    // Reset the argument counter
    m_Argc = 0;
}

// ------------------------------------------------------------------------------------------------
void CmdManager::Bind(const String & name, CmdListener * cmd)
{
    // If it doesn't exist then it will be created
    m_Commands[name] = cmd;
}

// ------------------------------------------------------------------------------------------------
void CmdManager::Unbind(const String & name)
{
    m_Commands.erase(name);
}

// ------------------------------------------------------------------------------------------------
Function & CmdManager::GetOnError()
{
    return m_OnError;
}

// ------------------------------------------------------------------------------------------------
void CmdManager::SetOnError(Function & func)
{
    m_OnError = func;
}

// ------------------------------------------------------------------------------------------------
Function & CmdManager::GetOnAuth()
{
    return m_OnAuth;
}

// ------------------------------------------------------------------------------------------------
void CmdManager::SetOnAuth(Function & func)
{
    m_OnAuth = func;
}

// ------------------------------------------------------------------------------------------------
const CPlayer & CmdManager::GetInvoker() const
{
    return m_Invoker;
}

// ------------------------------------------------------------------------------------------------
SQInt32 CmdManager::GetInvokerID() const
{
    return m_Invoker;
}

// ------------------------------------------------------------------------------------------------
const SQChar * CmdManager::GetName()
{
    return m_Name.c_str();
}

// ------------------------------------------------------------------------------------------------
const SQChar * CmdManager::GetText()
{
    return m_Text.c_str();
}

// ------------------------------------------------------------------------------------------------
void CmdManager::Execute(SQInt32 invoker, const String & str)
{
    // Save the invoker identifier
    m_Invoker.SetID(invoker);
    // Find where the command name ends
    String::size_type split_pos = str.find_first_of(' ');
    // Attempt to separate the command name from arguments
    if (split_pos != String::npos)
    {
        m_Name = str.substr(0, split_pos);
        m_Text = str.substr(split_pos+1);
    }
    // No arguments specified
    else
    {
        m_Name = str;
        m_Text = _SC("");
    }
    // Attempt to find a command with this name
    CmdPool::iterator itr = m_Commands.find(m_Name);
    // See if a command with this name could be found
    if (itr != m_Commands.end())
    {
        // Place a lock on the command
        itr->second->m_Lock = true;
        // Attempt to execute the command
        Exec(*itr->second);
        // Take the lock from the command
        itr->second->m_Lock = false;
    }
    else
    {
        Error(CMDERR_UNKNOWN_COMMAND, _SC("No such command exists: %s"), m_Name.c_str());
    }
}

// ------------------------------------------------------------------------------------------------
void CmdManager::Exec(CmdListener & cmd)
{
    // See if the invoker has enough authority to execute this command
    if (!cmd.AuthCheck(m_Invoker))
    {
        Error(CMDERR_INSUFFICIENT_AUTH, _SC("Insufficient authority to execute command"));
        // Command failed
        return;
    }
    // See if an executer was specified
    if (cmd.GetOnExec().IsNull())
    {
        Error(CMDERR_MISSING_EXECUTER, _SC("No executer was specified for this command"));
        // Command failed
        return;
    }
    // See if there are any arguments to parse
    if (!m_Text.empty() && !Parse(cmd.GetMaxArgC()))
    {
        // The error message was reported while parsing
        return;
    }
    // See if we have enough arguments specified
    if (cmd.GetMinArgC() > m_Argc)
    {
        Error(CMDERR_INCOMPLETE_ARGS, _SC("Incomplete command arguments: %u < %u"),
                                            m_Argc, cmd.GetMinArgC());
        // Command failed
        return;
    }
    // The check during the parsing may not count the last argument
    else if (cmd.GetMaxArgC() < m_Argc)
    {
        Error(CMDERR_EXTRANEOUS_ARGS, _SC("Extraneous command arguments: %u < %u"),
                                    cmd.GetMaxArgC(), m_Argc);
        // Command failed
        return;
    }
    // Check argument types against the command specifiers
    for (Uint32 arg = 0; arg < m_Argc; ++arg)
    {
        if (!cmd.ArgCheck(arg, m_Argv[arg].first))
        {
            Error(CMDERR_UNSUPPORTED_ARG, _SC("Unsupported command argument: %u -> %s"),
                                    arg, CmdArgSpecToStr(m_Argv[arg].first));
            // Command failed
            return;
        }
    }
    // Reserve an array for the extracted arguments
    Array args(DefaultVM::Get(), m_Argc);
    // Copy the arguments into the array
    for (SQUint32 arg = 0; arg < m_Argc; ++arg)
    {
        args.Bind(arg, m_Argv[arg].second);
    }
    // Attempt to execute the command with the specified arguments
    bool result = cmd.Execute(m_Invoker, args);
    // See if an error occurred
    if (Error::Occurred(DefaultVM::Get()))
    {
        Error(CMDERR_EXECUTION_FAILED, _SC("Command execution failed: %s"), Error::Message(DefaultVM::Get()).c_str());
    }
    // See if the command failed
    else if (!result)
    {
        Error(CMDERR_EXECUTION_FAILED, _SC("Command execution failed"));
    }
}

// ------------------------------------------------------------------------------------------------
bool CmdManager::Parse(SQUint32 max)
{
    // Clear previous arguments
    m_Argv.fill(CmdArgs::value_type(CMDARG_ANY, NullData()));
    // Reset the argument counter
    m_Argc = 0;
    // Request a temporary buffer
    Buffer buffer = _Core->PullBuffer(128);
    // Internal start and end of the temporary buffer
    SQChar * cur = nullptr, * end = nullptr;
    // The pointer to the currently processed character
    const SQChar * str = m_Text.c_str();
    // Currently and previously processed character
    SQChar ch = 0, pr = 0;
    // When parsing may continue
    bool good = true;
    // Process the specified command text
    while (good)
    {
        // Extract the current character before advancing
        ch = *(str++);
        // See if there's anything left to parse
        if (ch == 0)
        {
            // Finished parsing
            break;
        }
        // Early check to prevent parsing unneeded arguments
        else if (max < m_Argc)
        {
            Error(CMDERR_EXTRANEOUS_ARGS, _SC("Extraneous command arguments: %u < %u"),
                                                max, m_Argc);
            // Parsing failed
            good = false;
            // Stop parsing
            break;
        }
        // See if we have to extract a string argument
        else if ((ch == '\'' || ch == '"') && pr != '\\')
        {
            // Obtain the internal beginning and ending of the temporary buffer
            cur = buffer.Begin(), end = (buffer.End()-1); /* + null */
            // Attempt to consume the string argument
            while (good)
            {
                // Extract the current character before advancing
                ch = *(str++);
                // See if there's anything left to parse
                if (ch == 0)
                {
                    Error(CMDERR_SYNTAX_ERROR, _SC("String not closed properly near command argument: %u"), m_Argc);
                    // Parsing failed
                    good = false;
                    // Stop parsing
                    break;
                }
                // First un-escaped single or double quote character ends argument
                if (ch == '\'' || ch == '"')
                {
                    // Was this not escaped?
                    if (pr != '\\')
                    {
                        // Terminate the string value in the temporary buffer
                        *cur = 0;
                        // Stop parsing
                        break;
                    }
                    else
                    {
                        // Overwrite last character when replicating
                        --cur;
                    }
                }
                // See if the temporary buffer needs to scale
                else if  (cur == end)
                {
                    // Attempt to increase the size of the temporary buffer
                    buffer.Increase(512);
                    // Obtain the internal beginning and ending of the temporary buffer again
                    cur = (buffer.Begin() + (buffer.Size()-512)), end = (buffer.End()-1);  /* + null */
                }
                // Simply replicate the character to the temporary buffer
                *(cur++) = ch;
                // Save current argument as the previous one
                pr = ch;
            }
            // See if the argument was valid
            if (!good)
            {
                // Propagate failure
                break;
            }
            // Transform it into a squirrel object
            sq_pushstring(DefaultVM::Get(), buffer.Data(), cur - buffer.Begin());
            // Get the object from the squirrel VM
            Var< SqObj > var(DefaultVM::Get(), -1);
            // Pop the created object from the stack
            if (!var.value.IsNull())
            {
                sq_pop(DefaultVM::Get(), 1);
            }
            // Add it to the argument list along with it's type
            m_Argv[m_Argc].first = CMDARG_STRING;
            m_Argv[m_Argc].second = var.value;
            // Move to the next argument
            ++m_Argc;
        }
        // Ignore space until another valid argument is found
        else if (ch != ' ' && (pr == ' ' || pr == 0))
        {
            // Obtain the internal beginning and ending of the temporary buffer
            cur = buffer.Begin(), end = (buffer.End()-1); /* + null */
            // Move back to the previous character before extracting
            --str;
            // Attempt to consume the argument value
            while (good)
            {
                // Extract the current character before advancing
                ch = *(str++);
                // See if there's anything left to parse
                if (ch == 0)
                {
                    // Move to the previous character so the main loop can stop
                    --str;
                    // Argument ended
                    break;
                }
                else if (ch == ' ')
                {
                    // Argument ended
                    break;
                }
                // See if the buffer needs to scale
                else if  (cur == end)
                {
                    // Attempt to increase the size of the temporary buffer
                    buffer.Increase(512);
                    // Obtain the internal beginning and ending of the temporary buffer again
                    cur = buffer.Begin() + (buffer.Size()-512), end = (buffer.End()-1); /* + null */
                }
                // Simply replicate the character to the temporary buffer
                *(cur++) = ch;
                // Save current argument as the previous one
                pr = ch;
            }
            // Terminate the string value in the temporary buffer
            *cur = 0;
            // Used to exclude all other checks when a valid type was found
            bool found = false;
            // Attempt to treat the value as an integer number
            if (!found)
            {
                SQChar * next = NULL;
                // Attempt to extract the integer value
                Int64 value = strtol(buffer.Data(), &next, 10);
                // See if this whole string was an integer
                if (next == cur)
                {
                    // Transform it into a squirrel object
                    Var< SQInteger >::push(DefaultVM::Get(), _SCSQI(value));
                    // Get the object from the squirrel VM
                    Var< SqObj > var(DefaultVM::Get(), -1);
                    // Pop the created object from the stack
                    if (!var.value.IsNull())
                    {
                        sq_pop(DefaultVM::Get(), 1);
                    }
                    // Add it to the argument list along with it's type
                    m_Argv[m_Argc].first = CMDARG_INTEGER;
                    m_Argv[m_Argc].second = var.value;
                    // We've found the correct value
                    found = true;
                }
            }
            // Attempt to treat the value as a float number
            if (!found)
            {
                SQChar * next = 0;
                // Attempt to extract the floating point value
                Float64 value = strtod(buffer.Data(), &next);
                // See if this whole string was a float value
                if (next == cur)
                {
                    // Transform it into a squirrel object
                    Var< SQFloat >::push(DefaultVM::Get(), _SCSQF(value));
                    // Get the object from the squirrel VM
                    Var< SqObj > var(DefaultVM::Get(), -1);
                    // Pop the created object from the stack
                    if (!var.value.IsNull())
                    {
                        sq_pop(DefaultVM::Get(), 1);
                    }
                    // Add it to the argument list along with it's type
                    m_Argv[m_Argc].first = CMDARG_FLOAT;
                    m_Argv[m_Argc].second = var.value;
                    // We've found the correct value
                    found = true;
                }
            }
            // Attempt to treat the value as a boolean
            if (!found)
            {
                if (strcmp(buffer.Data(), "true") == 0 || strcmp(buffer.Data(), "on") == 0)
                {
                    // Transform it into a squirrel object
                    Var< bool >::push(DefaultVM::Get(), true);
                    // Get the object from the squirrel VM
                    Var< SqObj > var(DefaultVM::Get(), -1);
                    // Pop the created object from the stack
                    if (!var.value.IsNull())
                    {
                        sq_pop(DefaultVM::Get(), 1);
                    }
                    // Add it to the argument list along with it's type
                    m_Argv[m_Argc].first = CMDARG_BOOLEAN;
                    m_Argv[m_Argc].second = var.value;
                    // We've found the correct value
                    found = true;
                }
                else if (strcmp(buffer.Data(), "false") == 0 || strcmp(buffer.Data(), "off") == 0)
                {
                    // Transform it into a squirrel object
                    Var< bool >::push(DefaultVM::Get(), false);
                    // Get the object from the squirrel VM
                    Var< SqObj > var(DefaultVM::Get(), -1);
                    // Pop the created object from the stack
                    if (!var.value.IsNull())
                    {
                        sq_pop(DefaultVM::Get(), 1);
                    }
                    // Add it to the argument list along with it's type
                    m_Argv[m_Argc].first = CMDARG_BOOLEAN;
                    m_Argv[m_Argc].second = var.value;
                    // We've found the correct value
                    found = true;
                }
            }
            // If everything else failed then simply treat the value as a string
            if (!found)
            {
                // Transform it into a squirrel object
                Var< const SQChar * >::push(DefaultVM::Get(), buffer.Data(), (cur - buffer.Begin()));
                // Get the object from the squirrel VM
                Var< SqObj > var(DefaultVM::Get(), -1);
                // Pop the created object from the stack
                if (!var.value.IsNull())
                {
                    sq_pop(DefaultVM::Get(), 1);
                }
                // Add it to the argument list along with it's type
                m_Argv[m_Argc].first = CMDARG_STRING;
                m_Argv[m_Argc].second = var.value;
            }
            // Move to the next argument
            ++m_Argc;
        }
        // Save current argument as the previous one
        pr = ch;
    }
    // Return the borrowed buffer
    _Core->PushBuffer(std::move(buffer));
    // Return whether the parsing was successful
    return good;
}

// ------------------------------------------------------------------------------------------------
CmdListener::CmdListener()
    : CmdListener(_SC(""), _SC(""))
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
CmdListener::CmdListener(const SQChar * name)
    : CmdListener(name, _SC(""), NullArray(), 0, MAX_CMD_ARGS)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
CmdListener::CmdListener(const SQChar * name, const SQChar * spec)
    : CmdListener(name, spec, NullArray(), 0, MAX_CMD_ARGS)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
CmdListener::CmdListener(const SQChar * name, const SQChar * spec, Array & tags)
    : CmdListener(name, spec, tags, 0, MAX_CMD_ARGS)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
CmdListener::CmdListener(const SQChar * name, const SQChar * spec, Uint8 min, Uint8 max)
    : CmdListener(name, spec, NullArray(), min, max)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
CmdListener::CmdListener(const SQChar * name, const SQChar * spec, Array & tags, Uint8 min, Uint8 max)
    : m_Args({{CMDARG_ANY}})
    , m_Argt({{_SC("")}})
    , m_MinArgc(0)
    , m_MaxArgc(MAX_CMD_ARGS)
    , m_Name()
    , m_Spec()
    , m_Help()
    , m_Info()
    , m_OnExec()
    , m_OnAuth(_Cmd->GetOnAuth())
    , m_Tag()
    , m_Data()
    , m_Level(SQMOD_UNKNOWN)
    , m_Authority(false)
    , m_Suspended(false)
    , m_Lock(false)
{
    // Set the minimum and maximum allowed arguments
    SetMinArgC(min);
    SetMaxArgC(max);
    // Extract the specified argument tags
    SetArgTags(tags);
    // Bind to the specified command name
    SetName(name);
    // Apply the specified argument rules
    SetSpec(spec);
    // Receive notification when the VM is about to be closed to release object references
    _Core->VMClose.Connect< CmdListener, &CmdListener::VMClose >(this);
}

// ------------------------------------------------------------------------------------------------
CmdListener::~CmdListener()
{
    // Stop receiving notification when the VM is about to be closed
    _Core->VMClose.Disconnect< CmdListener, &CmdListener::VMClose >(this);
}

// ------------------------------------------------------------------------------------------------
void CmdListener::VMClose()
{
    // Release the reference to the specified callbacks
    m_OnAuth.Release2();
    m_OnExec.Release2();
    // Release the reference to the specified user data
    m_Data.Release();
}

// ------------------------------------------------------------------------------------------------
SQInt32 CmdListener::Cmp(const CmdListener & o) const
{
    if (m_Name == o.m_Name)
    {
        return 0;
    }
    else if (m_Name.size() > o.m_Name.size())
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

// ------------------------------------------------------------------------------------------------
const SQChar * CmdListener::ToString() const
{
    return m_Name.c_str();
}

// ------------------------------------------------------------------------------------------------
const SQChar * CmdListener::GetTag() const
{
    return m_Tag.c_str();
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetTag(const SQChar * tag)
{
    m_Tag.assign(tag);
}

// ------------------------------------------------------------------------------------------------
SqObj & CmdListener::GetData()
{
    return m_Data;
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetData(SqObj & data)
{
    m_Data = data;
}

// ------------------------------------------------------------------------------------------------
const SQChar * CmdListener::GetName() const
{
    return m_Name.c_str();
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetName(const SQChar * name)
{
    if (!m_Lock)
    {
        // If there's a name then we're already binded
        if (!m_Name.empty())
        {
            _Cmd->Unbind(name);
        }
        // Assign the new name
        m_Name.assign(name);
        // If the new name is valid then bind to it
        if (!m_Name.empty())
        {
            _Cmd->Bind(name, this);
        }
    }
    else
    {
        LogWrn("Attempting to <change command name> while command is under active lock: %s", m_Name.c_str());
    }
}

// ------------------------------------------------------------------------------------------------
const SQChar * CmdListener::GetSpec() const
{
    return m_Spec.c_str();
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetSpec(const SQChar * spec)
{
    if (ProcSpec(spec))
    {
        m_Spec.assign(spec);
    }
}

// ------------------------------------------------------------------------------------------------
const SQChar * CmdListener::GetArgTag(SQUint32 arg) const
{
    if (arg < MAX_CMD_ARGS)
    {
        return m_Argt[arg].c_str();
    }
    else
    {
        LogErr("Unable to <get command argument tag/name> because : argument is out of bounds %u > %u",
                arg, MAX_CMD_ARGS);
    }
    // Argument failed inspection
    return _SC("");
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetArgTag(SQUint32 arg, const SQChar * name)
{
    if (arg < MAX_CMD_ARGS)
    {
        m_Argt[arg].assign(name);
    }
    else
    {
        LogErr("Unable to <set command argument tag/name> because : argument is out of bounds %u > %u",
                arg, MAX_CMD_ARGS);
    }
}

// ------------------------------------------------------------------------------------------------
Array CmdListener::GetArgTags() const
{
    // Allocate an array to encapsulate all tags
    Array arr(DefaultVM::Get(), MAX_CMD_ARGS);
    // Put the tags to the allocated array
    for (SQUint32 arg = 0; arg < MAX_CMD_ARGS; ++arg)
    {
        arr.SetValue(arg, m_Argt[arg]);
    }
    // Return the array with the tags
    return arr;
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetArgTags(Array & tags)
{
    // Attempt to retrieve the number of specified tags
    const SQUint32 max = _SCU32(tags.Length());
    // If no tags were specified then clear current tags
    if (max == 0)
    {
        m_Argt.fill(Argt::value_type());
    }
    // See if we're in range
    else if (max < MAX_CMD_ARGS)
    {
        // Attempt to get all arguments in one go
        tags.GetArray(m_Argt.data(), max);
    }
    else
    {
        LogErr("Unable to <set command argument tag/name> because : argument is out of bounds %u > %u",
                max, MAX_CMD_ARGS);
    }
}

// ------------------------------------------------------------------------------------------------
const SQChar * CmdListener::GetHelp() const
{
    return m_Help.c_str();
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetHelp(const SQChar * help)
{
    m_Help.assign(help);
}

// ------------------------------------------------------------------------------------------------
const SQChar * CmdListener::GetInfo() const
{
    return m_Info.c_str();
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetInfo(const SQChar * info)
{
    m_Info.assign(info);
}

// ------------------------------------------------------------------------------------------------
Function & CmdListener::GetOnExec()
{
    return m_OnExec;
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetOnExec(Function & func)
{
    m_OnExec = func;
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetOnExec_Env(SqObj & env, Function & func)
{
    m_OnExec = Function(env.GetVM(), env, func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
Function & CmdListener::GetOnAuth()
{
    return m_OnAuth;
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetOnAuth(Function & func)
{
    m_OnAuth = func;
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetOnAuth_Env(SqObj & env, Function & func)
{
    m_OnAuth = Function(env.GetVM(), env, func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
SQInt32 CmdListener::GetLevel() const
{
    return m_Level;
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetLevel(SQInt32 level)
{
    m_Level = level;
}

// ------------------------------------------------------------------------------------------------
bool CmdListener::GetAuthority() const
{
    return m_Authority;
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetAuthority(bool toggle)
{
    m_Authority = toggle;
}

// ------------------------------------------------------------------------------------------------
bool CmdListener::GetSuspended() const
{
    return m_Suspended;
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetSuspended(bool toggle)
{
    m_Suspended = toggle;
}

// ------------------------------------------------------------------------------------------------
Uint8 CmdListener::GetMinArgC() const
{
    return m_MinArgc;
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetMinArgC(Uint8 val)
{
    if (val < MAX_CMD_ARGS && val <= m_MaxArgc)
    {
        m_MinArgc = val;
    }
    else if (val > m_MaxArgc)
    {
        LogWrn("Attempting to <set maximum command arguments> using a value bigger then maximum: %u > %u",
                val, m_MaxArgc);
    }
    else
    {
        LogWrn("Attempting to <set minimum command arguments> using an out of bounds value %u > %u",
                val, MAX_CMD_ARGS);
    }
}

// ------------------------------------------------------------------------------------------------
Uint8 CmdListener::GetMaxArgC() const
{
    return m_MaxArgc;
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetMaxArgC(Uint8 val)
{
    if (val < MAX_CMD_ARGS && val >= m_MinArgc)
    {
        m_MaxArgc = val;
    }
    else if (val < m_MinArgc)
    {
        LogWrn("Attempting to <set maximum command arguments> using a value smaller then minimum: %u < %u",
                val, m_MinArgc);
    }
    else
    {
        LogWrn("Attempting to <set maximum command arguments> using an out of bounds value: %u > %d",
                val, MAX_CMD_ARGS);
    }
}

// ------------------------------------------------------------------------------------------------
void CmdListener::GenerateInfo(bool full)
{
    // Clear any previously generated informational message
    m_Info.clear();
    // Process each supported command argument
    for (SQUint32 arg = 0; arg < m_MaxArgc; ++arg)
    {
        // If this is not a full command request then see if we must stop
        if (!full)
        {
            // Default to stop if criteria are not meet
            bool stop = true;
            // Check all arguments after this and see if there's any left
            for (Uint32 idx = arg; idx < m_MaxArgc; ++idx)
            {
                // If the argument has a name or a type specifier then it's valid
                if (!m_Argt[idx].empty() || m_Args[idx] != CMDARG_ANY)
                {
                    // We have more arguments that need to be parsed
                    stop = false;
                    // Go back to the main loop
                    break;
                }
            }
            // Is there any argument left?
            if (stop)
            {
                // Stop the main loop as well
                break;
            }
        }
        // Begin the argument block
        m_Info += '<';
        // If the current argument is beyond minimum then mark it as optional
        if (arg >= m_MinArgc)
        {
            m_Info += '*';
        }
        // If the argument has a tag/name associated then add it as well
        if (!m_Argt[arg].empty())
        {
            // Add the name first
            m_Info += m_Argt[arg];
            // Separate the name from the specifiers
            m_Info += ':';
        }
        // If the argument has any explicit types specified
        if (m_Args[arg] != CMDARG_ANY)
        {
            // Does it support integers?
            if (m_Args[arg] & CMDARG_INTEGER)
            {
                m_Info += _SC("integer");
            }
            // Does it support floats?
            if (m_Args[arg] & CMDARG_FLOAT)
            {
                // Add a separator if this is not the first enabled type?
                if (m_Info.back() != ':' && m_Info.back() != '<')
                {
                    m_Info += ',';
                }
                // Now add the type name
                m_Info += _SC("float");
            }
            // Does it support booleans?
            if (m_Args[arg] & CMDARG_BOOLEAN)
            {
                // Add a separator if this is not the first enabled type?
                if (m_Info.back() != ':' && m_Info.back() != '<')
                {
                    m_Info += ',';
                }
                // Now add the type name
                m_Info += _SC("boolean");
            }
            // Does it support strings?
            if (m_Args[arg] & CMDARG_STRING)
            {
                // Add a separator if this is not the first enabled type?
                if (m_Info.back() != ':' && m_Info.back() != '<')
                {
                    m_Info += ',';
                }
                // Now add the type name
                m_Info += _SC("string");
            }
        }
        // Any kind of value is supported by this argument
        else
        {
            m_Info += _SC("any");
        }
        // Stop the argument block
        m_Info += '>';
        // If this is not the last argument then add a spacer
        if (arg+1 != m_MaxArgc)
        {
            m_Info += ' ';
        }
    }
}

// ------------------------------------------------------------------------------------------------
bool CmdListener::ArgCheck(SQUint32 arg, Uint8 mask) const
{
    if (arg < MAX_CMD_ARGS)
    {
        return (m_Args[arg] & mask) || m_Args[arg] == CMDARG_ANY;
    }
    else
    {
        LogErr("Unable to <check command argument specifier> because : argument is out of bounds %u > %u",
                arg, MAX_CMD_ARGS);
    }
    // Argument failed inspection
    return false;
}

// ------------------------------------------------------------------------------------------------
bool CmdListener::AuthCheck(Reference< CPlayer > & player)
{
    return AuthCheckID(player);
}

// ------------------------------------------------------------------------------------------------
bool CmdListener::AuthCheckID(SQInt32 id)
{
    // Allow execution by default
    bool allow = true;
    // Do we need explicit authority verification?
    if (m_Authority)
    {
        // Was there a custom authority inspector specified?
        if (!m_OnAuth.IsNull())
        {
            // Ask the specified authority inspector if this execution should be allowed
            SharedPtr< bool > ret = m_OnAuth.Evaluate< bool, SQInt32 >(id);
            // See what the custom authority inspector said or default to disallow
            allow = (!ret ? false : *ret);
        }
        // Can we use the default authority system?
        else if (m_Level >= 0)
        {
            allow = (Reference< CPlayer >::Get(id).Level >= m_Level);
        }
    }
    // Return result
    return allow;
}

// ------------------------------------------------------------------------------------------------
bool CmdListener::Execute(CPlayer & invoker, Array & args)
{
    bool result = false;
    // Was there an executer specified?
    if (!m_OnExec.IsNull())
    {
        // Attempt to evaluate the specified executer
        SharedPtr< bool > ret = m_OnExec.Evaluate< bool, CPlayer &, Array & >(invoker, args);
        // See if the executer succeeded or default to failed
        result = !ret ? false : *ret;
    }
    // Return result
    return result;
}

// ------------------------------------------------------------------------------------------------
bool CmdListener::ProcSpec(const SQChar * str)
{
    // Currently processed character
    SQChar ch = 0;
    // When parsing may continue
    bool good = true;
    // Currently processed argument
    SQUint32 idx = 0;
    // Process each character in the specified command
    while (good)
    {
        // Extract the current character before advancing
        ch = *(str++);
        // See if there are still things left to parse
        if (ch == 0)
        {
            // Finished parsing successfully
            break;
        }
        // See if we need to move to the next argument
        else if (ch == '|')
        {
            if (idx > MAX_CMD_ARGS)
            {
                LogErr("Unable to <parse command specifier> because : argument is out of bounds %u > %u",
                        idx, MAX_CMD_ARGS);
                // Parsing failed
                good = false;
                // Stop parsing
                break;
            }
            // Move to the next argument
            ++idx;
        }
        // Simply ignore a type specifier delimiter
        else if (ch != ',')
        {
            // Consume space when found
            if (ch == ' ')
            {
                while (good)
                {
                    ch = *(str++);
                    // Stop when the text ends or on the first non-space character
                    if (ch == 0 || ch != ' ')
                    {
                        break;
                    }
                }
            }
            // See if there is a specifier left
            if (!good)
            {
                // Propagate stop
                break;
            }
            // Apply the type specifier
            switch(ch)
            {
                // Enable the integer type
                case 'i': m_Args[idx] |= CMDARG_INTEGER; break;
                // Enable the float type
                case 'f': m_Args[idx] |= CMDARG_FLOAT; break;
                // Enable the boolean type
                case 'b': m_Args[idx] |= CMDARG_BOOLEAN; break;
                // Enable the string type
                case 's': m_Args[idx] |= CMDARG_STRING; break;
                // Unknown type!
                default:
                    {
                        LogErr("Unable to <parse command specifier> because : unknown type specifier %u -> %c",
                                idx, ch);
                        // Parsing failed
                        good = false;
                    }
                break;
            }
        }
    }
    // Reset all argument specifiers if failed
    if (!good)
    {
        m_Args.fill(CMDARG_ANY);
    }
    // Attempt to generate an informational message
    GenerateInfo(false);
    // Return whether the parsing was successful
    return good;
}

// ------------------------------------------------------------------------------------------------
const SQChar * CmdArgSpecToStr(Uint8 spec)
{
    switch (spec)
    {
        case CMDARG_ANY:       return _SC("any");
        case CMDARG_INTEGER:   return _SC("integer");
        case CMDARG_FLOAT:     return _SC("float");
        case CMDARG_BOOLEAN:   return _SC("boolean");
        case CMDARG_STRING:    return _SC("string");
        default:            return _SC("unknown");
    }
}

// ------------------------------------------------------------------------------------------------
static Function & Cmd_GetOnError()
{
    return _Cmd->GetOnError();
}

// ------------------------------------------------------------------------------------------------
static void Cmd_SetOnError(Function & func)
{
    _Cmd->SetOnError(func);
}

// ------------------------------------------------------------------------------------------------
static Function & Cmd_GetOnAuth()
{
    return _Cmd->GetOnAuth();
}

// ------------------------------------------------------------------------------------------------
static void Cmd_SetOnAuth(Function & func)
{
    _Cmd->SetOnAuth(func);
}

// ------------------------------------------------------------------------------------------------
static const CPlayer & Cmd_GetInvoker()
{
    return _Cmd->GetInvoker();
}

// ------------------------------------------------------------------------------------------------
static SQInt32 Cmd_GetInvokerID()
{
    return _Cmd->GetInvokerID();
}

// ------------------------------------------------------------------------------------------------
static const SQChar * Cmd_GetName()
{
    return _Cmd->GetName();
}

// ------------------------------------------------------------------------------------------------
static const SQChar * Cmd_GetText()
{
    return _Cmd->GetText();
}

// ================================================================================================
bool Register_Cmd(HSQUIRRELVM vm)
{
    // // Attempt to create the Cmd namespace
    Sqrat::Table cmdns(vm);

    // Output debugging information
    LogDbg("Beginning registration of <Cmd Listener> type");
    // Attempt to register the specified type
    cmdns.Bind(_SC("Listener"), Sqrat::Class< CmdListener, NoCopy< CmdListener > >(vm, _SC("Listener"))
        /* Constructors */
        .Ctor()
        .Ctor< const SQChar * >()
        .Ctor< const SQChar *, const SQChar * >()
        .Ctor< const SQChar *, const SQChar *, Array & >()
        .Ctor< const SQChar *, const SQChar *, Uint8, Uint8 >()
        .Ctor< const SQChar *, const SQChar *, Array &, Uint8, Uint8 >()
        /* Metamethods */
        .Func(_SC("_cmp"), &CmdListener::Cmp)
        .Func(_SC("_tostring"), &CmdListener::ToString)
        /* Properties */
        .Prop(_SC("ltag"), &CmdListener::GetTag, &CmdListener::SetTag)
        .Prop(_SC("ldata"), &CmdListener::GetData, &CmdListener::SetData)
        .Prop(_SC("name"), &CmdListener::GetName, &CmdListener::SetName)
        .Prop(_SC("spec"), &CmdListener::GetSpec, &CmdListener::SetSpec)
        .Prop(_SC("tags"), &CmdListener::GetArgTags, &CmdListener::SetArgTags)
        .Prop(_SC("help"), &CmdListener::GetHelp, &CmdListener::SetHelp)
        .Prop(_SC("info"), &CmdListener::GetInfo, &CmdListener::SetInfo)
        .Prop(_SC("on_exec"), &CmdListener::GetOnExec, &CmdListener::SetOnExec)
        .Prop(_SC("on_auth"), &CmdListener::GetOnAuth, &CmdListener::SetOnAuth)
        .Prop(_SC("level"), &CmdListener::GetLevel, &CmdListener::SetLevel)
        .Prop(_SC("auth"), &CmdListener::GetAuthority, &CmdListener::SetAuthority)
        .Prop(_SC("authority"), &CmdListener::GetAuthority, &CmdListener::SetAuthority)
        .Prop(_SC("suspended"), &CmdListener::GetSuspended, &CmdListener::SetSuspended)
        .Prop(_SC("min_args"), &CmdListener::GetMinArgC, &CmdListener::SetMinArgC)
        .Prop(_SC("max_args"), &CmdListener::GetMaxArgC, &CmdListener::SetMaxArgC)
        /* Functions */
        .Func(_SC("get_arg_tag"), &CmdListener::GetArgTag)
        .Func(_SC("set_arg_tag"), &CmdListener::SetArgTag)
        .Func(_SC("set_on_exec"), &CmdListener::SetOnExec_Env)
        .Func(_SC("set_on_auth"), &CmdListener::SetOnAuth_Env)
        .Func(_SC("generate_info"), &CmdListener::GenerateInfo)
        .Func(_SC("arg_check"), &CmdListener::ArgCheck)
        .Func(_SC("auth_check"), &CmdListener::AuthCheck)
        .Func(_SC("auth_check_id"), &CmdListener::AuthCheckID)
    );

    // Output debugging information
    LogDbg("Registration of <Cmd Listener> type was successful");

    // Output debugging information
    LogDbg("Beginning registration of <Cmd functions> type");
    // Attempt to register the free functions
    cmdns.Func(_SC("GetOnError"), &Cmd_GetOnError);
    cmdns.Func(_SC("SetOnError"), &Cmd_SetOnError);
    cmdns.Func(_SC("GetOnAuth"), &Cmd_GetOnAuth);
    cmdns.Func(_SC("SetOnAuth"), &Cmd_SetOnAuth);
    cmdns.Func(_SC("GetInvoker"), &Cmd_GetInvoker);
    cmdns.Func(_SC("GetInvokerID"), &Cmd_GetInvokerID);
    cmdns.Func(_SC("GetName"), &Cmd_GetName);
    cmdns.Func(_SC("GetText"), &Cmd_GetText);
    // Output debugging information
    LogDbg("Registration of <Cmd functions> type was successful");

    // Attempt to bind the namespace to the root table
    Sqrat::RootTable(vm).Bind(_SC("Cmd"), cmdns);

    // Output debugging information
    LogDbg("Beginning registration of <Cmd Constants> type");
    // Attempt to register the argument types enumeration
    Sqrat::ConstTable(vm).Enum(_SC("ECMDARG"), Sqrat::Enumeration(vm)
        .Const(_SC("ANY"),                  CMDARG_ANY)
        .Const(_SC("INTEGER"),              CMDARG_INTEGER)
        .Const(_SC("FLOAT"),                CMDARG_FLOAT)
        .Const(_SC("BOOLEAN"),              CMDARG_BOOLEAN)
        .Const(_SC("STRING"),               CMDARG_STRING)
    );
    // Attempt to register the error codes enumeration
    Sqrat::ConstTable(vm).Enum(_SC("ECMDERR"), Sqrat::Enumeration(vm)
        .Const(_SC("UNKNOWN"),              CMDERR_UNKNOWN)
        .Const(_SC("SYNTAX_ERROR"),         CMDERR_SYNTAX_ERROR)
        .Const(_SC("UNKNOWN_COMMAND"),      CMDERR_UNKNOWN_COMMAND)
        .Const(_SC("MISSING_EXECUTER"),     CMDERR_MISSING_EXECUTER)
        .Const(_SC("INSUFFICIENT_AUTH"),    CMDERR_INSUFFICIENT_AUTH)
        .Const(_SC("INCOMPLETE_ARGS"),      CMDERR_INCOMPLETE_ARGS)
        .Const(_SC("EXTRANEOUS_ARGS"),      CMDERR_EXTRANEOUS_ARGS)
        .Const(_SC("UNSUPPORTED_ARG"),      CMDERR_UNSUPPORTED_ARG)
        .Const(_SC("EXECUTION_FAILED"),     CMDERR_EXECUTION_FAILED)
    );
    // Output debugging information
    LogDbg("Registration of <IRC Constants> type was successful");
    // Registration succeeded
    return true;
}


} // Namespace:: SqMod
