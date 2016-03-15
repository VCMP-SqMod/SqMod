// ------------------------------------------------------------------------------------------------
#include "Command.hpp"
#include "Core.hpp"
#include "Entity/Player.hpp"

// ------------------------------------------------------------------------------------------------
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <iterator>
#include <algorithm>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
CmdManager * _Cmd = nullptr;

// ------------------------------------------------------------------------------------------------
SQInteger CmdListener::Typename(HSQUIRRELVM vm)
{
    static SQChar name[] = _SC("SqCmdListener");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
static void ValidateName(CSStr name)
{
    // Is the name empty?
    if (!name || *name == '\0')
    {
        SqThrowF("Invalid or empty command name");
    }
    // Inspect name characters
    while (*name != '\0')
    {
        // Does it contain spaces?
        if (isspace(*name) != 0)
        {
            SqThrowF("Command names cannot contain spaces");
        }
        // Move to the next character
        ++name;
    }
}

// ------------------------------------------------------------------------------------------------
CSStr CmdArgSpecToStr(Uint8 spec)
{
    switch (spec)
    {
        case CMDARG_ANY:        return _SC("any");
        case CMDARG_INTEGER:    return _SC("integer");
        case CMDARG_FLOAT:      return _SC("float");
        case CMDARG_BOOLEAN:    return _SC("boolean");
        case CMDARG_STRING:
        case CMDARG_LOWER:
        case CMDARG_UPPER:
        case CMDARG_GREEDY:     return _SC("string");
        default:                return _SC("unknown");
    }
}

// ------------------------------------------------------------------------------------------------
CmdManager::CmdManager()
    : m_Buffer(512)
    , m_Commands()
    , m_Invoker(SQMOD_UNKNOWN)
    , m_Command(64, '\0')
    , m_Argument(512, '\0')
    , m_Argv()
    , m_Argc(0)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
CmdManager::~CmdManager()
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Object & CmdManager::Attach(const String & name, CmdListener * ptr, bool autorel)
{
    // Obtain the unique identifier of the specified name
    const std::size_t hash = std::hash< String >()(name);
    // Make sure the command doesn't already exist
    for (const auto & cmd : m_Commands)
    {
        // Are the hashes identical?
        if (cmd.mHash == hash)
        {
            // Do we have to release this listener instance our self?
            if (autorel)
            {
                delete ptr; // Let's avoid memory leaks!
            }
            // Now it's safe to throw the exception
            // (include information necessary to help identify hash collisions!)
            SqThrowF("Command (%s:%zu) already exists as (%s:%zu)",
                        name.c_str(), hash, cmd.mName.c_str(), cmd.mHash);
        }
    }
    // Obtain the initial stack size
    const StackGuard sg;
    // Push this instance on the stack
    ClassType< CmdListener >::PushInstance(DefaultVM::Get(), ptr);
    // Attempt to insert the command
    m_Commands.emplace_back(hash, name, Var< Object >(DefaultVM::Get(), -1).value);
    // Return the script object of the listener
    return m_Commands.back().mObj;
}

// ------------------------------------------------------------------------------------------------
void CmdManager::Detach(const String & name)
{
    // Obtain the unique identifier of the specified name
    const std::size_t hash = std::hash< String >()(name);
    // Iterator to the found command, if any
    CmdList::const_iterator itr = m_Commands.cbegin();
    // Attempt to find the specified command
    for (; itr != m_Commands.cend(); ++itr)
    {
        // Are the hashes identical?
        if (itr->mHash == hash)
        {
            break; // We found our command!
        }
    }
    // Make sure the command exist before attempting to remove it
    if (itr != m_Commands.end())
    {
        m_Commands.erase(itr);
    }
}

// ------------------------------------------------------------------------------------------------
void CmdManager::Detach(CmdListener * ptr)
{
    // Iterator to the found command, if any
    CmdList::const_iterator itr = m_Commands.cbegin();
    // Attempt to find the specified command
    for (; itr != m_Commands.cend(); ++itr)
    {
        // Are the instances identical?
        if (itr->mPtr == ptr)
        {
            break; // We found our command!
        }
    }
    // Make sure the command exists before attempting to remove it
    if (itr != m_Commands.end())
    {
        m_Commands.erase(itr);
    }
}

// ------------------------------------------------------------------------------------------------
bool CmdManager::Attached(const String & name) const
{
    // Obtain the unique identifier of the specified name
    const std::size_t hash = std::hash< String >()(name);
    // Attempt to find the specified command
    for (const auto & cmd : m_Commands)
    {
        // Are the hashes identical?
        if (cmd.mHash == hash)
        {
            return true; // We found our command!
        }
    }
    // No such command exists
    return false;
}

// ------------------------------------------------------------------------------------------------
bool CmdManager::Attached(const CmdListener * ptr) const
{
    // Attempt to find the specified command
    for (const auto & cmd : m_Commands)
    {
        // Are the instances identical?
        if (cmd.mPtr == ptr)
        {
            return true; // We found our command!
        }
    }
    // No such command exists
    return false;
}

// ------------------------------------------------------------------------------------------------
void CmdManager::Sort()
{
    std::sort(m_Commands.begin(), m_Commands.end(),
        [](CmdList::const_reference a, CmdList::const_reference b) -> bool {
            return (b.mName < a.mName);
        });
}

// ------------------------------------------------------------------------------------------------
const Object & CmdManager::FindByName(const String & name)
{
    // Obtain the unique identifier of the specified name
    const std::size_t hash = std::hash< String >()(name);
    // Attempt to find the specified command
    for (const auto & cmd : m_Commands)
    {
        // Are the hashes identical?
        if (cmd.mHash == hash)
        {
            return cmd.mObj; // We found our command!
        }
    }
    // No such command exist
    return NullObject();
}

// ------------------------------------------------------------------------------------------------
void CmdManager::Terminate()
{
    // Release the script resources from command instances
    for (const auto & cmd : m_Commands)
    {
        if (cmd.mPtr)
        {
            // Release the local command callbacks
            cmd.mPtr->m_OnExec.ReleaseGently();
            cmd.mPtr->m_OnAuth.ReleaseGently();
            cmd.mPtr->m_OnPost.ReleaseGently();
            cmd.mPtr->m_OnFail.ReleaseGently();
        }
    }
    // Clear the command list and release all references
    m_Commands.clear();
    // Release the script resources from this class
    m_Argv.clear();
    // Release the global callbacks
    m_OnError.ReleaseGently();
    m_OnAuth.ReleaseGently();
}

// ------------------------------------------------------------------------------------------------
Int32 CmdManager::Run(Int32 invoker, CCStr command)
{
    // Validate the string command
    if (!command || *command == '\0')
    {
        // Tell the script callback to deal with the error
        SqError(CMDERR_EMPTY_COMMAND, _SC("Invalid or empty command name"), invoker);
        // Execution failed!
        return -1;
    }
    // Save the invoker identifier
    m_Invoker = invoker;
    // Skip white-space until the command name
    while (isspace(*command)) ++command;
    // Anything left to process?
    if (*command == '\0')
    {
        // Tell the script callback to deal with the error
        SqError(CMDERR_EMPTY_COMMAND, _SC("Invalid or empty command name"), invoker);
        // Execution failed!
        return -1;
    }
    // Where the name ends and argument begins
    CCStr split = command;
    // Find where the command name ends
    while (!isspace(*split)) ++split;
    // Are there any arguments specified?
    if (split != nullptr)
    {
        // Save the command name
        m_Command.assign(command, (split - command));
        // Skip white space after command name
        while (isspace(*split)) ++split;
        // Save the command argument
        m_Argument.assign(split);
    }
    // No arguments specified
    else
    {
        // Save the command name
        m_Command.assign(command);
        // Leave argument empty
        m_Argument.assign("");
    }
    // Do we have a valid command name?
    try
    {
        ValidateName(m_Command.c_str());
    }
    catch (...)
    {
        // Tell the script callback to deal with the error
        SqError(CMDERR_INVALID_COMMAND, _SC("Cannot execute invalid command name"), invoker);
        // Execution failed!
        return -1;
    }
    // Attempt to find the specified command
    Object obj = FindByName(m_Command);
    // Have we found anything?
    if (obj.IsNull())
    {
        // Tell the script callback to deal with the error
        SqError(CMDERR_UNKNOWN_COMMAND, _SC("Unable to find the specified command"), m_Command);
        // Execution failed!
        return -1;
    }
    // Save the command instance
    m_Instance = obj.Cast< CmdListener * >();
    // Is the command instance valid? (just in case)
    if (!m_Instance)
    {
        // Tell the script callback to deal with the error
        SqError(CMDERR_UNKNOWN_COMMAND, _SC("Unable to find the specified command"), m_Command.c_str());
        // Execution failed!
        return -1;
    }
    // Value returned by the command
    Int32 ret = -1;
    // Attempt to execute the command
    try
    {
        ret = Exec();
    }
    catch (...)
    {
        // Tell the script callback to deal with the error
        SqError(CMDERR_EXECUTION_FAILED, _SC("Exceptions occurred during execution"), m_Invoker);
    }
    // Release the command instance
    m_Instance = nullptr;
    // Return the result
    return ret;
}

// ------------------------------------------------------------------------------------------------
Int32 CmdManager::Exec()
{
    // Clear previous arguments
    m_Argv.clear();
    // Reset the argument counter
    m_Argc = 0;
    // Make sure the invoker has enough authority to execute this command
    if (!m_Instance->AuthCheckID(m_Invoker))
    {
        // Tell the script callback to deal with the error
        SqError(CMDERR_INSUFFICIENT_AUTH, _SC("Insufficient authority to execute command"), m_Invoker);
        // Execution failed!
        return -1;
    }
    // Make sure an executer was specified
    else if (m_Instance->GetOnExec().IsNull())
    {
        // Tell the script callback to deal with the error
        SqError(CMDERR_MISSING_EXECUTER, _SC("No executer was specified for this command"), m_Invoker);
        // Execution failed!
        return -1;
    }
    // See if there are any arguments to parse
    else if (!m_Argument.empty() && !Parse())
    {
        // The error message was reported while parsing
        return -1;
    }
    // Make sure we have enough arguments specified
    else if (m_Instance->GetMinArgC() > m_Argc)
    {
        // Tell the script callback to deal with the error
        SqError(CMDERR_INCOMPLETE_ARGS, _SC("Incomplete command arguments"), m_Instance->GetMinArgC());
        // Execution failed!
        return -1;
    }
    // The check during the parsing may omit the last argument
    else if (m_Instance->GetMaxArgC() < m_Argc)
    {
        // Tell the script callback to deal with the error
        SqError(CMDERR_EXTRANEOUS_ARGS, _SC("Extraneous command arguments"), m_Instance->GetMaxArgC());
        // Execution failed!
        return -1;
    }
    // Check argument types against the command specifiers
    for (Uint32 arg = 0; arg < m_Argc; ++arg)
    {
        if (!m_Instance->ArgCheck(arg, m_Argv[arg].first))
        {
            // Tell the script callback to deal with the error
            SqError(CMDERR_UNSUPPORTED_ARG, _SC("Unsupported command argument"), arg);
            // Execution failed!
            return -1;
        }
    }
    // Result of the command execution
    SQInteger result = -1;
    // Clear any data from the buffer to make room for the error message
    m_Buffer.At(0) = 0;
    // Whether the command execution failed
    bool failed = false;
    // Do we have to call the command with an associative container?
    if (m_Instance->m_Associate)
    {
        // Create the associative container
        Table args(DefaultVM::Get());
        // Copy the arguments into the table
        for (Uint32 arg = 0; arg < m_Argc; ++arg)
        {
            // Do we have use the argument index as the key?
            if (m_Instance->m_ArgTags[arg].empty())
                args.SetValue(SQInteger(arg), m_Argv[arg].second);
            // Nope, we have a name for this argument!
            else
                args.SetValue(m_Instance->m_ArgTags[arg].c_str(), m_Argv[arg].second);
        }
        // Attempt to execute the command with the specified arguments
        try
        {
            result = m_Instance->Execute(_Core->GetPlayer(m_Invoker).mObj, args);
        }
        catch (const Sqrat::Exception & e)
        {
            // Let's store the exception message
            m_Buffer.Write(0, e.Message().c_str(), e.Message().size());
            // Specify that the command execution failed
            failed = true;
        }
    }
    else
    {
        // Reserve an array for the extracted arguments
        Array args(DefaultVM::Get(), m_Argc);
        // Copy the arguments into the array
        for (Uint32 arg = 0; arg < m_Argc; ++arg)
            args.Bind(SQInteger(arg), m_Argv[arg].second);
        // Attempt to execute the command with the specified arguments
        try
        {
            result = m_Instance->Execute(_Core->GetPlayer(m_Invoker).mObj, args);
        }
        catch (const Sqrat::Exception & e)
        {
            // Let's store the exception message
            m_Buffer.Write(0, e.Message().c_str(), e.Message().size());
            // Specify that the command execution failed
            failed = true;
        }
    }
    // Was there a runtime exception during the execution?
    if (failed)
    {
        // Tell the script callback to deal with the error
        SqError(CMDERR_EXECUTION_FAILED, _SC("Command execution failed"), m_Buffer.Data());
        // Is there a script callback that handles failures?
        if (!m_Instance->m_OnFail.IsNull())
        {
            // Then attempt to relay the result to that function
            try
            {
                m_Instance->m_OnFail.Execute(_Core->GetPlayer(m_Invoker).mObj, result);
            }
            catch (const Sqrat::Exception & e)
            {
                // Tell the script callback to deal with the error
                SqError(CMDERR_UNRESOLVED_FAILURE, _SC("Unable to resolve command failure"), e.Message());
            }
        }
        // Result is invalid at this point
        result = -1;
    }
    // Was the command aborted explicitly?
    else if (!result)
    {
        // Tell the script callback to deal with the error
        SqError(CMDERR_EXECUTION_ABORTED, _SC("Command execution aborted"), result);
        // Is there a script callback that handles failures?
        if (!m_Instance->m_OnFail.IsNull())
        {
            // Then attempt to relay the result to that function
            try
            {
                m_Instance->m_OnFail.Execute(_Core->GetPlayer(m_Invoker).mObj, result);
            }
            catch (const Sqrat::Exception & e)
            {
                // Tell the script callback to deal with the error
                SqError(CMDERR_UNRESOLVED_FAILURE, _SC("Unable to resolve command failure"), e.Message());
            }
        }
    }
    // Is there a callback that must be executed after a successful execution?
    else if (!m_Instance->m_OnPost.IsNull())
    {
            // Then attempt to relay the result to that function
            try
            {
                m_Instance->m_OnPost.Execute(_Core->GetPlayer(m_Invoker).mObj, result);
            }
            catch (const Sqrat::Exception & e)
            {
                // Tell the script callback to deal with the error
                SqError(CMDERR_POST_PROCESSING_FAILED, _SC("Unable to complete command post processing"), e.Message());
            }
    }
    // Return the result
    return static_cast< Int32 >(result);
}

// ------------------------------------------------------------------------------------------------
bool CmdManager::Parse()
{
    // Is there anything to parse?
    if (m_Argument.empty())
    {
        return true; // Done parsing!
    }
    // Obtain the flags of the currently processed argument
    Uint8 arg_flags = m_Instance->m_ArgSpec[m_Argc];
    // Adjust the internal buffer if necessary (mostly never)
    m_Buffer.Adjust< SQChar >(m_Argument.size());
    // The iterator to the currently processed character
    String::iterator itr = m_Argument.begin();
    // Previous and currently processed character
    SQChar prev = 0, elem = 0;
    // Maximum arguments allowed to be processed
    const Uint8 max_arg = m_Instance->m_MaxArgc;
    // Process loop result
    bool good = true;
    // Process the specified command text
    while (good)
    {
        // Extract the current characters before advancing
        prev = elem, elem = *itr;
        // See if there's anything left to parse
        if (elem == 0)
            break;
        // Early check to prevent parsing extraneous arguments
        else if (m_Argc >= max_arg)
        {
            // Tell the script callback to deal with the error
            SqError(CMDERR_EXTRANEOUS_ARGS, _SC("Extraneous command arguments"), max_arg);
            // Parsing aborted
            good = false;
            // Stop parsing
            break;
        }
        // Is this a greedy argument?
        else if (arg_flags & CMDARG_GREEDY)
        {
            // Skip white-space characters
            while (itr != m_Argument.end() && isspace(*itr)) ++itr;
            // Anything left to copy to the argument?
            if (itr != m_Argument.end())
                // Transform it into a script object
                sq_pushstring(DefaultVM::Get(), &(*itr), std::distance(itr, m_Argument.end()));
            // Just push an empty string
            else
                sq_pushstring(DefaultVM::Get(), _SC(""), 0);
            // Get the object from the stack and add it to the argument list along with it's type
            m_Argv.emplace_back(CMDARG_STRING, Var< Object >(DefaultVM::Get(), -1).value);
            // Pop the created object from the stack
            sq_pop(DefaultVM::Get(), 1);
            // Include this argument into the count
            ++m_Argc;
            // Nothing left to parse
            break;
        }
        // Do we have to extract a string argument?
        else if ((elem == '\'' || elem == '"') && prev != '\\')
        {
            // Obtain the beginning and ending of the internal buffer
            SStr str = m_Buffer.Begin< SQChar >();
            CSStr end = (m_Buffer.End< SQChar >()-1); // + null terminator
            // Save the closing quote type
            SQChar close = elem;
            // Skip the opening quote
            ++itr;
            // Attempt to consume the string argument
            while (good)
            {
                // Extract the current characters before advancing
                prev = elem, elem = *itr;
                // See if there's anything left to parse
                if (elem == 0)
                {
                    // Tell the script callback to deal with the error
                    SqError(CMDERR_SYNTAX_ERROR, _SC("String argument not closed properly"), m_Argc);
                    // Parsing aborted
                    good = false;
                    // Stop parsing
                    break;
                }
                // First un-escaped matching quote character ends the argument
                else if (elem == close)
                {
                    // Was this not escaped?
                    if (prev != '\\')
                    {
                        // Terminate the string value in the internal buffer
                        *str = 0;
                        // Stop parsing
                        break;
                    }
                    // Overwrite last character when replicating
                    else
                        --str;
                }
                // See if the internal buffer needs to scale
                else if  (str >= end)
                {
                    // We should already have a buffer as big as the entire command!
                    SqError(CMDERR_BUFFER_OVERFLOW, _SC("Command buffer was exceeded unexpectedly"), m_Invoker);
                    // Parsing aborted
                    good = false;
                    // Stop parsing
                    break;
                }
                // Simply replicate the character to the internal buffer
                *(str++) = elem;
                // Advance to the next character
                ++itr;
            }
            // See if the argument was valid
            if (!good)
                // Propagate failure
                break;
            // Do we have to make the string lowercase?
            else if (arg_flags & CMDARG_LOWER)
            {
                for (SStr chr = m_Buffer.Begin< SQChar >(); chr <= str; ++chr)
                    *chr = static_cast< SQChar >(tolower(*chr));
            }
            // Do we have to make the string uppercase?
            else if (arg_flags & CMDARG_UPPER)
            {
                for (SStr chr = m_Buffer.Begin< SQChar >(); chr <= str; ++chr)
                    *chr = static_cast< SQChar >(toupper(*chr));
            }
            // Transform it into a script object
            sq_pushstring(DefaultVM::Get(), m_Buffer.Get< SQChar >(), str - m_Buffer.Begin< SQChar >());
            // Get the object from the stack and add it to the argument list along with it's type
            m_Argv.emplace_back(CMDARG_STRING, Var< Object >(DefaultVM::Get(), -1).value);
            // Pop the created object from the stack
            sq_pop(DefaultVM::Get(), 1);
            // Advance to the next argument and obtain its flags
            arg_flags = m_Instance->m_ArgSpec[++m_Argc];
        }
        // Ignore white-space characters until another valid character is found
        else if (!isspace(elem) && (isspace(prev) || prev == 0))
        {
            // Find the first space character that marks the end of the argument
            String::iterator pos = std::find(String::iterator(itr), m_Argument.end(), ' ');
            // Copy all characters within range into the internal buffer
            const Uint32 sz = m_Buffer.Write(0, &(*itr), std::distance(itr, pos));
            // Update the main iterator position
            itr = pos;
            // Update the current character
            elem = *itr;
            // Make sure the argument string is null terminated
            m_Buffer.At< SQChar >(sz) = 0;
            // Used to exclude all other checks when a valid type was identified
            bool identified = false;
            // Attempt to treat the value as an integer number
            if (!identified)
            {
                // Let's us know if the whole argument was part of the resulted value
                CStr next = nullptr;
                // Attempt to extract the integer value from the string
                LongI value = strtol(m_Buffer.Data(), &next, 10);
                // See if this whole string was indeed an integer
                if (next == &m_Buffer.At< SQChar >(sz))
                {
                    // Transform it into a script object
                    sq_pushinteger(DefaultVM::Get(), static_cast< SQInteger >(value));
                    // Get the object from the stack and add it to the argument list along with it's type
                    m_Argv.emplace_back(CMDARG_INTEGER, Var< Object >(DefaultVM::Get(), -1).value);
                    // Pop the created object from the stack
                    sq_pop(DefaultVM::Get(), 1);
                    // We identified the correct value
                    identified = true;
                }
            }
            // Attempt to treat the value as an floating point number
            if (!identified)
            {
                // Let's us know if the whole argument was part of the resulted value
                CStr next = nullptr;
                // Attempt to extract the floating point value from the string
#ifdef SQUSEDOUBLE
                Float64 value = strtod(m_Buffer.Data(), &next);
#else
                Float32 value = strtof(m_Buffer.Data(), &next);
#endif // SQUSEDOUBLE
                // See if this whole string was indeed an floating point
                if (next == &m_Buffer.At< SQChar >(sz))
                {
                    // Transform it into a script object
                    sq_pushfloat(DefaultVM::Get(), static_cast< SQFloat >(value));
                    // Get the object from the stack and add it to the argument list along with it's type
                    m_Argv.emplace_back(CMDARG_FLOAT, Var< Object >(DefaultVM::Get(), -1).value);
                    // Pop the created object from the stack
                    sq_pop(DefaultVM::Get(), 1);
                    // We identified the correct value
                    identified = true;
                }
            }
            // Attempt to treat the value as a boolean if possible
            if (!identified && sz <= 6)
            {
                // Allocate memory for enough data to form a boolean value
                SQChar lc[6];
                // Fill the temporary buffer with data from the internal buffer
                snprintf (lc, 6, "%.5s", m_Buffer.Data());
                // Convert all characters to lowercase
                for (Uint32 i = 0; i < 5; ++i)
                    lc[i] = tolower(lc[i]);
                // Is this a boolean true value?
                if (strcmp(m_Buffer.Data(), "true") == 0 || strcmp(m_Buffer.Data(), "on") == 0)
                {
                    // Transform it into a script object
                    sq_pushbool(DefaultVM::Get(), true);
                    // Get the object from the stack and add it to the argument list along with it's type
                    m_Argv.emplace_back(CMDARG_BOOLEAN, Var< Object >(DefaultVM::Get(), -1).value);
                    // Pop the created object from the stack
                    sq_pop(DefaultVM::Get(), 1);
                    // We identified the correct value
                    identified = true;
                }
                // Is this a boolean false value?
                else if (strcmp(m_Buffer.Data(), "false") == 0 || strcmp(m_Buffer.Data(), "off") == 0)
                {
                    // Transform it into a script object
                    sq_pushbool(DefaultVM::Get(), false);
                    // Get the object from the stack and add it to the argument list along with it's type
                    m_Argv.emplace_back(CMDARG_BOOLEAN, Var< Object >(DefaultVM::Get(), -1).value);
                    // Pop the created object from the stack
                    sq_pop(DefaultVM::Get(), 1);
                    // We identified the correct value
                    identified = true;
                }
            }
            // If everything else failed then simply treat the value as a string
            if (!identified)
            {
                // Do we have to make the string lowercase?
                if (arg_flags & CMDARG_LOWER)
                {
                    for (Uint32 n = 0; n < sz; ++n)
                        m_Buffer.At< SQChar >(n) = static_cast< SQChar >(tolower(m_Buffer.At< SQChar >(n)));
                }
                // Do we have to make the string uppercase?
                else if (arg_flags & CMDARG_UPPER)
                {
                    for (Uint32 n = 0; n < sz; ++n)
                        m_Buffer.At< SQChar >(n) = static_cast< SQChar >(toupper(m_Buffer.At< SQChar >(n)));
                }
                // Transform it into a script object
                sq_pushstring(DefaultVM::Get(), m_Buffer.Get< SQChar >(), sz);
                // Get the object from the stack and add it to the argument list along with it's type
                m_Argv.emplace_back(CMDARG_STRING, Var< Object >(DefaultVM::Get(), -1).value);
                // Pop the created object from the stack
                sq_pop(DefaultVM::Get(), 1);
            }
            // Advance to the next argument and obtain its flags
            arg_flags = m_Instance->m_ArgSpec[++m_Argc];
        }
        // Is there anything left to parse?
        if (itr >= m_Argument.end())
            break;
        // Advance to the next character
        ++itr;
    }
    // Return whether the parsing was successful
    return good;
}

// ------------------------------------------------------------------------------------------------
Object & CmdManager::Create(CSStr name)
{
    return Attach(name, new CmdListener(name), true);
}

Object & CmdManager::Create(CSStr name, CSStr spec)
{
    return Attach(name, new CmdListener(name, spec), true);
}

Object & CmdManager::Create(CSStr name, CSStr spec, Array & tags)
{
    return Attach(name, new CmdListener(name, spec, tags), true);
}

Object & CmdManager::Create(CSStr name, CSStr spec, Uint8 min, Uint8 max)
{
    return Attach(name, new CmdListener(name, spec, min, max), true);
}

Object & CmdManager::Create(CSStr name, CSStr spec, Array & tags, Uint8 min, Uint8 max)
{
    return Attach(name, new CmdListener(name, spec, tags, min, max), true);
}

// ------------------------------------------------------------------------------------------------
void CmdListener::Init(CSStr name, CSStr spec, Array & tags, Uint8 min, Uint8 max)
{
    m_Name.assign("");
    // Initialize the specifiers and tags to default values
    for (Uint8 n = 0; n < SQMOD_MAX_CMD_ARGS; ++n)
    {
        m_ArgSpec[n] = CMDARG_ANY;
        m_ArgTags[n].assign("");
    }
    // Default to minimum and maximum arguments
    m_MinArgc = 0;
    m_MaxArgc = (SQMOD_MAX_CMD_ARGS - 1);
    // Default to no specifiers, help or informational message
    m_Spec.assign("");
    m_Help.assign("");
    m_Info.assign("");
    // Default to no authority check
    m_Authority = -1;
    // Default to unprotected command
    m_Protected = false;
    // Default to unsuspended command
    m_Suspended = false;
    // Default to non-associative arguments
    m_Associate = false;
    // Set the specified minimum and maximum allowed arguments
    SetMinArgC(min);
    SetMaxArgC(max);
    // Extract the specified argument tags
    SetArgTags(tags);
    // Bind to the specified command name
    SetName(name);
    // Apply the specified argument rules
    SetSpec(spec);
}

// ------------------------------------------------------------------------------------------------
CmdListener::CmdListener(CSStr name)
{
    Init(name, _SC(""), NullArray(), 0, SQMOD_MAX_CMD_ARGS-1);
}

CmdListener::CmdListener(CSStr name, CSStr spec)
{
    Init(name, spec, NullArray(), 0, SQMOD_MAX_CMD_ARGS-1);
}

CmdListener::CmdListener(CSStr name, CSStr spec, Array & tags)
{
    Init(name, spec, tags, 0, SQMOD_MAX_CMD_ARGS-1);
}

CmdListener::CmdListener(CSStr name, CSStr spec, Uint8 min, Uint8 max)
{
    Init(name, spec, NullArray(), min, max);
}

CmdListener::CmdListener(CSStr name, CSStr spec, Array & tags, Uint8 min, Uint8 max)
{
    Init(name, spec, tags, min, max);
}

// ------------------------------------------------------------------------------------------------
CmdListener::~CmdListener()
{
    // Detach this command (shouldn't be necessary!)
    _Cmd->Detach(this);
    // Release callbacks
    m_OnExec.ReleaseGently();
    m_OnAuth.ReleaseGently();
    m_OnPost.ReleaseGently();
    m_OnFail.ReleaseGently();
}

// ------------------------------------------------------------------------------------------------
Int32 CmdListener::Cmp(const CmdListener & o) const
{
    if (m_Name == o.m_Name)
        return 0;
    else if (m_Name.size() > o.m_Name.size())
        return 1;
    else
        return -1;
}

// ------------------------------------------------------------------------------------------------
CSStr CmdListener::ToString() const
{
    return m_Name.c_str();
}

// ------------------------------------------------------------------------------------------------
void CmdListener::Attach()
{
    // Is the associated name even valid?
    if (m_Name.empty())
    {
        SqThrowF("Invalid or empty command name");
    }
    // Are we already attached?
    else if (_Cmd->Attached(this))
    {
        SqThrowF("Command is already attached");
    }
    // Attempt to attach this command
    _Cmd->Attach(m_Name, this, false);
}

// ------------------------------------------------------------------------------------------------
void CmdListener::Detach()
{
    // Detach this command
    _Cmd->Detach(this);
}

// ------------------------------------------------------------------------------------------------
Uint8 CmdListener::GetArgFlags(Uint32 idx) const
{
    if (idx < SQMOD_MAX_CMD_ARGS)
        return m_ArgSpec[idx];
    return CMDARG_ANY;
}

// ------------------------------------------------------------------------------------------------
CSStr CmdListener::GetName() const
{
    return m_Name.c_str();
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetName(CSStr name)
{
    // Validate the specified name
    ValidateName(name);
    // Is this command already attached to a name?
    if (_Cmd->Attached(this))
    {
        // Detach from the current name if necessary
        _Cmd->Detach(this);
        // Now it's safe to assign the new name
        m_Name.assign(name);
        // We know the new name is valid
        _Cmd->Attach(m_Name, this, false);
    }
    else
    {
        // Just assign the name
        m_Name.assign(name);
    }
}

// ------------------------------------------------------------------------------------------------
CSStr CmdListener::GetSpec() const
{
    return m_Spec.c_str();
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetSpec(CSStr spec)
{
    // Attempt to process the specified string
    ProcSpec(spec);
    // At this point there were no errors
    m_Spec.assign(spec);
}

// ------------------------------------------------------------------------------------------------
Array CmdListener::GetArgTags() const
{
    // Allocate an array to encapsulate all tags
    Array arr(DefaultVM::Get(), SQMOD_MAX_CMD_ARGS);
    // Put the tags to the allocated array
    for (Uint32 arg = 0; arg < SQMOD_MAX_CMD_ARGS; ++arg)
    {
        arr.SetValue(arg, m_ArgTags[arg]);
    }
    // Return the array with the tags
    return arr;
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetArgTags(Array & tags)
{
    // Attempt to retrieve the number of specified tags
    const Uint32 max = static_cast< Uint32 >(tags.Length());
    // If no tags were specified then clear current tags
    if (tags.IsNull() || max == 0)
    {
        for (Uint8 n = 0; n < SQMOD_MAX_CMD_ARGS; ++n)
        {
            m_ArgTags[n].assign("");
        }
    }
    // See if we're in range
    else if (max < SQMOD_MAX_CMD_ARGS)
    {
        // Attempt to get all arguments in one go
        tags.GetArray(m_ArgTags, max);
    }
    else
    {
        SqThrowF("Argument tag (%u) is out of range (%u)", max, SQMOD_MAX_CMD_ARGS);
    }
}

// ------------------------------------------------------------------------------------------------
bool CmdListener::Attached() const
{
    return _Cmd->Attached(this);
}

// ------------------------------------------------------------------------------------------------
CSStr CmdListener::GetHelp() const
{
    return m_Help.c_str();
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetHelp(CSStr help)
{
    m_Help.assign(help);
}

// ------------------------------------------------------------------------------------------------
CSStr CmdListener::GetInfo() const
{
    return m_Info.c_str();
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetInfo(CSStr info)
{
    m_Info.assign(info);
}

// ------------------------------------------------------------------------------------------------
Int32 CmdListener::GetAuthority() const
{
    return m_Authority;
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetAuthority(Int32 level)
{
    m_Authority = level;
}

// ------------------------------------------------------------------------------------------------
bool CmdListener::GetProtected() const
{
    return m_Protected;
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetProtected(bool toggle)
{
    m_Protected = toggle;
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
bool CmdListener::GetAssociate() const
{
    return m_Associate;
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetAssociate(bool toggle)
{
    m_Associate = toggle;
}

// ------------------------------------------------------------------------------------------------
Uint8 CmdListener::GetMinArgC() const
{
    return m_MinArgc;
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetMinArgC(Uint8 val)
{
    // Perform a range check on the specified argument index
    if (val >= SQMOD_MAX_CMD_ARGS)
    {
        SqThrowF("Argument (%u) is out of total range (%u)", val, SQMOD_MAX_CMD_ARGS);
    }
    else if (val > m_MaxArgc)
    {
        SqThrowF("Minimum argument (%u) exceeds maximum (%u)", val, m_MaxArgc);
    }
    // Apply the specified value
    m_MinArgc = val;
}

// ------------------------------------------------------------------------------------------------
Uint8 CmdListener::GetMaxArgC() const
{
    return m_MaxArgc;
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetMaxArgC(Uint8 val)
{
    // Perform a range check on the specified argument index
    if (val >= SQMOD_MAX_CMD_ARGS)
    {
        SqThrowF("Argument (%u) is out of total range (%u)", val, SQMOD_MAX_CMD_ARGS);
    }
    else if (val < m_MinArgc)
    {
        SqThrowF("Minimum argument (%u) exceeds maximum (%u)", m_MinArgc, val);
    }
    // Apply the specified value
    m_MaxArgc = val;
}

// ------------------------------------------------------------------------------------------------
Function & CmdListener::GetOnExec()
{
    return m_OnExec;
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetOnExec(Object & env, Function & func)
{
    // Make sure that we are allowed to store script resources
    if (m_Name.empty())
    {
        SqThrowF("Invalid commands cannot store script resources");
    }
    // Apply the specified information
    m_OnExec = Function(env.GetVM(), env.GetObject(), func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
Function & CmdListener::GetOnAuth()
{
    return m_OnAuth;
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetOnAuth(Object & env, Function & func)
{
    // Make sure that we are allowed to store script resources
    if (m_Name.empty())
    {
        SqThrowF("Invalid commands cannot store script resources");
    }
    // Apply the specified information
    m_OnAuth = Function(env.GetVM(), env.GetObject(), func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
Function & CmdListener::GetOnPost()
{
    return m_OnPost;
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetOnPost(Object & env, Function & func)
{
    // Make sure that we are allowed to store script resources
    if (m_Name.empty())
    {
        SqThrowF("Invalid commands cannot store script resources");
    }
    // Apply the specified information
    m_OnPost = Function(env.GetVM(), env.GetObject(), func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
Function & CmdListener::GetOnFail()
{
    return m_OnFail;
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetOnFail(Object & env, Function & func)
{
    // Make sure that we are allowed to store script resources
    if (m_Name.empty())
    {
        SqThrowF("Invalid commands cannot store script resources");
    }
    // Apply the specified information
    m_OnFail = Function(env.GetVM(), env.GetObject(), func.GetFunc());
}

// ------------------------------------------------------------------------------------------------
CSStr CmdListener::GetArgTag(Uint32 arg) const
{
    // Perform a range check on the specified argument index
    if (arg >= SQMOD_MAX_CMD_ARGS)
    {
        SqThrowF("Argument (%u) is out of total range (%u)", arg, SQMOD_MAX_CMD_ARGS);
    }
    // Return the requested information
    return m_ArgTags[arg].c_str();
}

// ------------------------------------------------------------------------------------------------
void CmdListener::SetArgTag(Uint32 arg, CSStr name)
{
    // Perform a range check on the specified argument index
    if (arg >= SQMOD_MAX_CMD_ARGS)
    {
        SqThrowF("Argument (%u) is out of total range (%u)", arg, SQMOD_MAX_CMD_ARGS);
    }
    // The string type doesn't appreciate null values
    else if (name)
    {
        m_ArgTags[arg].assign(name);
    }
    // Clear previous name in this case
    else
        m_ArgTags[arg].clear();
}

// ------------------------------------------------------------------------------------------------
void CmdListener::GenerateInfo(bool full)
{
    // Clear any previously generated informational message
    m_Info.clear();
    // Process each supported command argument
    for (Uint32 arg = 0; arg < m_MaxArgc; ++arg)
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
                if (!m_ArgTags[idx].empty() || m_ArgSpec[idx] != CMDARG_ANY)
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
                break; // Stop the main loop as well
            }
        }
        // Begin the argument block
        m_Info.push_back('<');
        // If the current argument is beyond minimum then mark it as optional
        if (arg >= m_MinArgc)
        {
            m_Info.push_back('*');
        }
        // If the argument has a tag/name associated then add it as well
        if (!m_ArgTags[arg].empty())
        {
            // Add the name first
            m_Info.append(m_ArgTags[arg]);
            // Separate the name from the specifiers
            m_Info.push_back(':');
        }
        // Obtain the argument specifier
        const Uint8 spec = m_ArgSpec[arg];
        // Is this a greedy argument?
        if (spec & CMDARG_GREEDY)
        {
            m_Info.append("...");
        }
        // If the argument has any explicit types specified
        else if (spec != CMDARG_ANY)
        {
            // Does it support integers?
            if (spec & CMDARG_INTEGER)
            {
                m_Info.append("integer");
            }
            // Does it support floats?
            if (spec & CMDARG_FLOAT)
            {
                // Add a separator if this is not the first enabled type!
                if (m_Info.back() != ':' && m_Info.back() != '<')
                {
                    m_Info.push_back(',');
                }
                // Now add the type name
                m_Info.append("float");
            }
            // Does it support booleans?
            if (spec & CMDARG_BOOLEAN)
            {
                // Add a separator if this is not the first enabled type!
                if (m_Info.back() != ':' && m_Info.back() != '<')
                {
                    m_Info.push_back(',');
                }
                // Now add the type name
                m_Info.append("boolean");
            }
            // Does it support strings?
            if (spec & CMDARG_STRING)
            {
                // Add a separator if this is not the first enabled type?
                if (m_Info.back() != ':' && m_Info.back() != '<')
                {
                    m_Info.push_back(',');
                }
                // Now add the type name
                m_Info.append("string");
            }
        }
        // Any kind of value is supported by this argument
        else
        {
            m_Info.append("any");
        }
        // Terminate the argument block
        m_Info.push_back('>');
        // Don't process anything after greedy arguments
        if (spec & CMDARG_GREEDY)
        {
            break;
        }
        // If this is not the last argument then add a separator
        else if (arg+1 != m_MaxArgc)
        {
            m_Info.push_back(' ');
        }
    }
}

// ------------------------------------------------------------------------------------------------
bool CmdListener::ArgCheck(Uint32 arg, Uint8 flag) const
{
    // Perform a range check on the specified argument index
    if (arg >= SQMOD_MAX_CMD_ARGS)
    {
        SqThrowF("Argument (%u) is out of total range (%u)", arg, SQMOD_MAX_CMD_ARGS);
    }
    // Retrieve the argument flags
    const Uint8 f = m_ArgSpec[arg];
    // Perform the requested check
    return  (f == CMDARG_ANY) || /* Requires check? */
            (f & flag) || /* Exact match? */
            (f & CMDARG_GREEDY && flag & CMDARG_STRING);
}

// ------------------------------------------------------------------------------------------------
bool CmdListener::AuthCheck(CPlayer & player)
{
    return AuthCheckID(player.GetID());
}

// ------------------------------------------------------------------------------------------------
bool CmdListener::AuthCheckID(Int32 id)
{
    // Do we need explicit authority verification?
    if (!m_Protected)
    {
        return true;
    }
    // Allow execution by default
    bool allow = true;
    // Was there a custom authority inspector specified?
    if (!m_OnAuth.IsNull())
    {
        // Ask the specified authority inspector if this execution should be allowed
        SharedPtr< bool > ret = m_OnAuth.Evaluate< bool, Object & >(_Core->GetPlayer(id).mObj);
        // See what the custom authority inspector said or default to disallow
        allow = (!ret ? false : *ret);
    }
    // Was there a global authority inspector specified?
    else if (!_Cmd->GetOnAuth().IsNull())
    {
        // Ask the specified authority inspector if this execution should be allowed
        SharedPtr< bool > ret = _Cmd->GetOnAuth().Evaluate< bool, Object & >(_Core->GetPlayer(id).mObj);
        // See what the custom authority inspector said or default to disallow
        allow = (!ret ? false : *ret);
    }
    // Can we use the default authority system?
    else if (m_Authority >= 0)
    {
        allow = (_Core->GetPlayer(id).mAuthority >= m_Authority);
    }
    // Return result
    return allow;
}

// ------------------------------------------------------------------------------------------------
SQInteger CmdListener::Execute(Object & invoker, Array & args)
{
    // Attempt to evaluate the specified executer knowing the manager did the validations
    SharedPtr< SQInteger > ret = m_OnExec.Evaluate< SQInteger, Object &, Array & >(invoker, args);
    // See if the executer succeeded and return the result or default to failed
    return (!ret ? 0 : *ret);
}

// ------------------------------------------------------------------------------------------------
SQInteger CmdListener::Execute(Object & invoker, Table & args)
{
    // Attempt to evaluate the specified executer knowing the manager did the validations
    SharedPtr< SQInteger > ret = m_OnExec.Evaluate< SQInteger, Object &, Table & >(invoker, args);
    // See if the executer succeeded and return the result or default to failed
    return (!ret ? 0 : *ret);
}

// ------------------------------------------------------------------------------------------------
void CmdListener::ProcSpec(CSStr str)
{
    // Reset current argument specifiers
    memset(m_ArgSpec, CMDARG_ANY, sizeof(m_ArgSpec));
    // Make sure we have anything to parse
    if (!str || *str == '\0')
    {
        return;
    }
    // Currently processed argument
    Uint32 idx = 0;
    // Try to apply the specified type specifiers
    try
    {
        // Process until null terminator or an error occurs
        while (*str != 0)
        {
            // See if we need to move to the next argument
            if (*str == '|')
            {
                if (idx >= SQMOD_MAX_CMD_ARGS)
                {
                    SqThrowF("Extraneous type specifiers: %d >= %d", idx, SQMOD_MAX_CMD_ARGS);
                }
                // Move to the next character
                ++str;
                // Advance to the next argument
                ++idx;
            }
            // Simply ignore a type specifier delimiter
            else if (*str != ',')
            {
                // Ignore non-alphabetic characters
                while (*str != 0 &&  !isalpha(*str))
                {
                    ++str;
                }
                // Apply the type specifier
                switch(*str++)
                {
                    // Did we reached the end of the string?
                    case '\0':
                        break;
                    // Is this a greedy argument?
                    case 'g':
                    {
                        m_ArgSpec[idx] = CMDARG_GREEDY;
                    } break;
                    // Is this a integer type
                    case 'i':
                    {
                        m_ArgSpec[idx] |= CMDARG_INTEGER;
                        // Disable greedy argument flag if set
                        if (m_ArgSpec[idx] & CMDARG_GREEDY)
                        {
                            m_ArgSpec[idx] ^= CMDARG_GREEDY;
                        }
                    } break;
                    // Is this a float type
                    case 'f':
                    {
                        m_ArgSpec[idx] |= CMDARG_FLOAT;
                        // Disable greedy argument flag if set
                        if (m_ArgSpec[idx] & CMDARG_GREEDY)
                        {
                            m_ArgSpec[idx] ^= CMDARG_GREEDY;
                        }
                    } break;
                    // Is this a boolean type
                    case 'b':
                    {
                        m_ArgSpec[idx] |= CMDARG_BOOLEAN;
                        // Disable greedy argument flag if set
                        if (m_ArgSpec[idx] & CMDARG_GREEDY)
                        {
                            m_ArgSpec[idx] ^= CMDARG_GREEDY;
                        }
                    } break;
                    // Is this a string type
                    case 's':
                    {
                        m_ArgSpec[idx] |= CMDARG_STRING;
                        // Disable greedy argument flag if set
                        if (m_ArgSpec[idx] & CMDARG_GREEDY)
                        {
                            m_ArgSpec[idx] ^= CMDARG_GREEDY;
                        }
                    } break;
                    // Is this a lowercase string?
                    case 'l':
                    {
                        m_ArgSpec[idx] |= CMDARG_STRING;
                        m_ArgSpec[idx] |= CMDARG_LOWER;
                        // Disable greedy argument flag if set
                        if (m_ArgSpec[idx] & CMDARG_GREEDY)
                        {
                            m_ArgSpec[idx] ^= CMDARG_GREEDY;
                        }
                    } break;
                    // Is this a uppercase string?
                    case 'u':
                    {
                        m_ArgSpec[idx] |= CMDARG_STRING;
                        m_ArgSpec[idx] |= CMDARG_UPPER;
                        // Disable greedy argument flag if set
                        if (m_ArgSpec[idx] & CMDARG_GREEDY)
                        {
                            m_ArgSpec[idx] ^= CMDARG_GREEDY;
                        }
                    } break;
                    // Unknown type!
                    default: SqThrowF("Unknown type specifier (%c) at argument: %u", *str, idx);
                }
            }
        }
    }
    catch (const Sqrat::Exception & e)
    {
        // Reset all argument specifiers if failed
        memset(m_ArgSpec, CMDARG_ANY, sizeof(m_ArgSpec));
        // Propagate the exception back to the caller
        throw e;
    }
    // Attempt to generate an informational message
    GenerateInfo(false);
}

/* ------------------------------------------------------------------------------------------------
 * Forward the call to run a command.
*/
Int32 RunCommand(Int32 invoker, CSStr command)
{
    return _Cmd->Run(invoker, command);
}

/* ------------------------------------------------------------------------------------------------
 * Forward the call to terminate the command system.
*/
void TerminateCommand()
{
    _Cmd->Terminate();
}

// ------------------------------------------------------------------------------------------------
static void Cmd_Sort()
{
    _Cmd->Sort();
}

// ------------------------------------------------------------------------------------------------
static Uint32 Cmd_Count()
{
    return _Cmd->Count();
}

// ------------------------------------------------------------------------------------------------
static const Object & Cmd_FindByName(CSStr name)
{
    // Validate the specified name
    ValidateName(name);
    // Now perform the requested search
    return _Cmd->FindByName(name);
}

// ------------------------------------------------------------------------------------------------
static Function & Cmd_GetOnError()
{
    return _Cmd->GetOnError();
}

// ------------------------------------------------------------------------------------------------
static void Cmd_SetOnError(Object & env, Function & func)
{
    _Cmd->SetOnError(env, func);
}

// ------------------------------------------------------------------------------------------------
static Function & Cmd_GetOnAuth()
{
    return _Cmd->GetOnAuth();
}

// ------------------------------------------------------------------------------------------------
static void Cmd_SetOnAuth(Object & env, Function & func)
{
    _Cmd->SetOnAuth(env, func);
}

// ------------------------------------------------------------------------------------------------
static Object & Cmd_GetInvoker()
{
    return _Core->GetPlayer(_Cmd->GetInvoker()).mObj;
}

// ------------------------------------------------------------------------------------------------
static Int32 Cmd_GetInvokerID()
{
    return _Cmd->GetInvoker();
}

// ------------------------------------------------------------------------------------------------
static CSStr Cmd_GetCommand()
{
    return _Cmd->GetCommand();
}

// ------------------------------------------------------------------------------------------------
static CSStr Cmd_GetArgument()
{
    return _Cmd->GetArgument();
}

// ------------------------------------------------------------------------------------------------
Object & Cmd_Create(CSStr name)
{
    return _Cmd->Create(name);
}

Object & Cmd_Create(CSStr name, CSStr spec)
{
    return _Cmd->Create(name, spec);
}

Object & Cmd_Create(CSStr name, CSStr spec, Array & tags)
{
    return _Cmd->Create(name, spec, tags);
}

Object & Cmd_Create(CSStr name, CSStr spec, Uint8 min, Uint8 max)
{
    return _Cmd->Create(name,spec, min, max);
}

Object & Cmd_Create(CSStr name, CSStr spec, Array & tags, Uint8 min, Uint8 max)
{
    return _Cmd->Create(name, spec, tags, min, max);
}

// ================================================================================================
void Register_Command(HSQUIRRELVM vm)
{
    Table cmdns(vm);

    cmdns.Bind(_SC("Listener"), Class< CmdListener, NoConstructor< CmdListener > >(vm, _SC("SqCmdListener"))
        /* Metamethods */
        .Func(_SC("_cmp"), &CmdListener::Cmp)
        .SquirrelFunc(_SC("_typename"), &CmdListener::Typename)
        .Func(_SC("_tostring"), &CmdListener::ToString)
        /* Properties */
        .Prop(_SC("Attached"), &CmdListener::Attached)
        .Prop(_SC("Name"), &CmdListener::GetName, &CmdListener::SetName)
        .Prop(_SC("Spec"), &CmdListener::GetSpec, &CmdListener::SetSpec)
        .Prop(_SC("Specifier"), &CmdListener::GetSpec, &CmdListener::SetSpec)
        .Prop(_SC("Tags"), &CmdListener::GetArgTags, &CmdListener::SetArgTags)
        .Prop(_SC("Help"), &CmdListener::GetHelp, &CmdListener::SetHelp)
        .Prop(_SC("Info"), &CmdListener::GetInfo, &CmdListener::SetInfo)
        .Prop(_SC("Authority"), &CmdListener::GetAuthority, &CmdListener::SetAuthority)
        .Prop(_SC("Protected"), &CmdListener::GetProtected, &CmdListener::SetProtected)
        .Prop(_SC("Suspended"), &CmdListener::GetSuspended, &CmdListener::SetSuspended)
        .Prop(_SC("Associate"), &CmdListener::GetAssociate, &CmdListener::SetAssociate)
        .Prop(_SC("MinArgs"), &CmdListener::GetMinArgC, &CmdListener::SetMinArgC)
        .Prop(_SC("MaxArgs"), &CmdListener::GetMaxArgC, &CmdListener::SetMaxArgC)
        .Prop(_SC("OnExec"), &CmdListener::GetOnExec)
        .Prop(_SC("OnAuth"), &CmdListener::GetOnAuth)
        .Prop(_SC("OnPost"), &CmdListener::GetOnPost)
        .Prop(_SC("OnFail"), &CmdListener::GetOnFail)
        /* Functions */
        .Func(_SC("Attach"), &CmdListener::Attach)
        .Func(_SC("Detach"), &CmdListener::Detach)
        .Func(_SC("BindExec"), &CmdListener::SetOnExec)
        .Func(_SC("BindAuth"), &CmdListener::SetOnAuth)
        .Func(_SC("BindPost"), &CmdListener::SetOnPost)
        .Func(_SC("BindFail"), &CmdListener::SetOnFail)
        .Func(_SC("GetArgTag"), &CmdListener::GetArgTag)
        .Func(_SC("SetArgTag"), &CmdListener::SetArgTag)
        .Func(_SC("GenerateInfo"), &CmdListener::GenerateInfo)
        .Func(_SC("ArgCheck"), &CmdListener::ArgCheck)
        .Func(_SC("AuthCheck"), &CmdListener::AuthCheck)
        .Func(_SC("AuthCheckID"), &CmdListener::AuthCheckID)
    );

    cmdns.Func(_SC("Sort"), &Cmd_Sort);
    cmdns.Func(_SC("Count"), &Cmd_Count);
    cmdns.Func(_SC("FindByName"), &Cmd_FindByName);
    cmdns.Func(_SC("GetOnError"), &Cmd_GetOnError);
    cmdns.Func(_SC("SetOnError"), &Cmd_SetOnError);
    cmdns.Func(_SC("BindError"), &Cmd_SetOnError);
    cmdns.Func(_SC("GetOnAuth"), &Cmd_GetOnAuth);
    cmdns.Func(_SC("SetOnAuth"), &Cmd_SetOnAuth);
    cmdns.Func(_SC("BindAuth"), &Cmd_SetOnAuth);
    cmdns.Func(_SC("GetInvoker"), &Cmd_GetInvoker);
    cmdns.Func(_SC("GetInvokerID"), &Cmd_GetInvokerID);
    cmdns.Func(_SC("GetName"), &Cmd_GetCommand);
    cmdns.Func(_SC("GetText"), &Cmd_GetArgument);
    cmdns.Overload< Object & (CSStr) >(_SC("Create"), &Cmd_Create);
    cmdns.Overload< Object & (CSStr, CSStr) >(_SC("Create"), &Cmd_Create);
    cmdns.Overload< Object & (CSStr, CSStr, Array &) >(_SC("Create"), &Cmd_Create);
    cmdns.Overload< Object & (CSStr, CSStr, Uint8, Uint8) >(_SC("Create"), &Cmd_Create);
    cmdns.Overload< Object & (CSStr, CSStr, Array &, Uint8, Uint8) >(_SC("Create"), &Cmd_Create);

    RootTable(vm).Bind(_SC("SqCmd"), cmdns);

    ConstTable(vm).Enum(_SC("CmdArg"), Enumeration(vm)
        .Const(_SC("Any"),                  CMDARG_ANY)
        .Const(_SC("Integer"),              CMDARG_INTEGER)
        .Const(_SC("Float"),                CMDARG_FLOAT)
        .Const(_SC("Boolean"),              CMDARG_BOOLEAN)
        .Const(_SC("String"),               CMDARG_STRING)
        .Const(_SC("Lower"),                CMDARG_LOWER)
        .Const(_SC("Upper"),                CMDARG_UPPER)
        .Const(_SC("Greedy"),               CMDARG_GREEDY)
    );

    ConstTable(vm).Enum(_SC("CmdErr"), Enumeration(vm)
        .Const(_SC("Unknown"),              CMDERR_UNKNOWN)
        .Const(_SC("EmptyCommand"),         CMDERR_EMPTY_COMMAND)
        .Const(_SC("InvalidCommand"),       CMDERR_INVALID_COMMAND)
        .Const(_SC("SyntaxError"),          CMDERR_SYNTAX_ERROR)
        .Const(_SC("UnknownCommand"),       CMDERR_UNKNOWN_COMMAND)
        .Const(_SC("MissingExecuter"),      CMDERR_MISSING_EXECUTER)
        .Const(_SC("InsufficientAuth"),     CMDERR_INSUFFICIENT_AUTH)
        .Const(_SC("IncompleteArgs"),       CMDERR_INCOMPLETE_ARGS)
        .Const(_SC("ExtraneousArgs"),       CMDERR_EXTRANEOUS_ARGS)
        .Const(_SC("UnsupportedArg"),       CMDERR_UNSUPPORTED_ARG)
        .Const(_SC("BufferOverflow"),       CMDERR_BUFFER_OVERFLOW)
        .Const(_SC("ExecutionFailed"),      CMDERR_EXECUTION_FAILED)
        .Const(_SC("ExecutionAborted"),     CMDERR_EXECUTION_ABORTED)
        .Const(_SC("PostProcessingFailed"), CMDERR_POST_PROCESSING_FAILED)
        .Const(_SC("UnresolvedFailure"),    CMDERR_UNRESOLVED_FAILURE)
        .Const(_SC("Max"),                  CMDERR_MAX)
    );
}

} // Namespace:: SqMod
