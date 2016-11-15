// ------------------------------------------------------------------------------------------------
#include "Command.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {
namespace Cmd {

// ------------------------------------------------------------------------------------------------
Controllers Controller::s_Controllers;

// ------------------------------------------------------------------------------------------------
Listener *  Listener::s_Head = nullptr;

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(ManagerTypename, _SC("SqCmdManager"))
SQMODE_DECL_TYPENAME(ListenerTypename, _SC("SqCmdListener"))

// ------------------------------------------------------------------------------------------------
Guard::Guard(const CtrRef & ctr, Object & invoker)
    : mController(ctr)
    , mPrevious(mController->m_Context)
    , mCurrent(new Context(invoker))
{
    mController->m_Context = mCurrent;
}

// ------------------------------------------------------------------------------------------------
Guard::~Guard()
{
    mController->m_Context = mPrevious;
}

// ------------------------------------------------------------------------------------------------
Command::Command(std::size_t hash, const String & name, Listener * ptr, const CtrPtr & ctr)
    : mHash(hash), mName(name), mPtr(ptr), mObj(ptr), mCtr(ctr)
{
    if (mPtr)
    {
        mPtr->m_Controller = mCtr; // Create controller association
    }
}
// ------------------------------------------------------------------------------------------------
Command::Command(std::size_t hash, const String & name, const Object & obj, const CtrPtr & ctr)
    : mHash(hash), mName(name), mPtr(obj.Cast< Listener * >()), mObj(obj), mCtr(ctr)
{
    if (mPtr)
    {
        mPtr->m_Controller = mCtr; // Create controller association
    }
}

// ------------------------------------------------------------------------------------------------
Command::Command(std::size_t hash, const String & name, Object && obj, const CtrPtr & ctr)
    : mHash(hash), mName(name), mPtr(obj.Cast< Listener * >()), mObj(obj), mCtr(ctr)
{
    if (mPtr)
    {
        mPtr->m_Controller = mCtr; // Create controller association
    }
}

// ------------------------------------------------------------------------------------------------
Command::Command(std::size_t hash, const String & name, Listener * ptr, const Object & obj, const CtrPtr & ctr)
    : mHash(hash), mName(name), mPtr(ptr), mObj(obj), mCtr(ctr)
{
    if (mPtr)
    {
        mPtr->m_Controller = mCtr; // Create controller association
    }
}

// ------------------------------------------------------------------------------------------------
Command::Command(std::size_t hash, const String & name, Listener * ptr, Object && obj, const CtrPtr & ctr)
    : mHash(hash), mName(name), mPtr(ptr), mObj(obj), mCtr(ctr)
{
    if (mPtr)
    {
        mPtr->m_Controller = mCtr; // Create controller association
    }
}

// ------------------------------------------------------------------------------------------------
Command::~Command()
{
    if (mPtr)
    {
        mPtr->m_Controller.Reset(); // Break controller association
    }
}

// ------------------------------------------------------------------------------------------------
Object & Controller::Attach(Object && obj, Listener * ptr)
{
    // Is there anything that we can attach
    if (obj.IsNull() && ptr == nullptr)
    {
        STHROWF("Cannot attach invalid command listener");
    }
    // Are we supposed to grab the object?
    else if (obj.IsNull())
    {
        // Obtain the initial stack size
        const StackGuard sg;
        // Push the instance on the stack
        ClassType< Listener >::PushInstance(DefaultVM::Get(), ptr);
        // Grab the instance from the stack
        obj = Var< Object >(DefaultVM::Get(), -1).value;
    }
    // Are we supposed to grab the instance?
    else if (ptr == nullptr)
    {
        // Obtain the initial stack size
        const StackGuard sg(obj.GetVM());
        // Push the object on the stack
        Var< Object & >::push(obj.GetVM(), obj);
        // Grab the instance from the stack
        ptr = Var< Listener * >(obj.GetVM(), -1).value;
    }
    // At this point we should have both the instance and the object
    if (obj.IsNull() || ptr == nullptr)
    {
        STHROWF("Unable to obtain the command listener");
    }
    // Obtain the command name
    const String & name = ptr->GetName();
    // Validate the command name
    if (name.empty())
    {
        STHROWF("Cannot attach command without a name");
    }
    // Obtain the unique identifier of the specified name
    const std::size_t hash = std::hash< String >()(name);
    // Make sure the command doesn't already exist
    for (const auto & cmd : m_Commands)
    {
        // Are the hashes identical?
        if (cmd.mHash == hash)
        {
            // Include information necessary to help identify hash collisions!
            STHROWF("Command '%s' already exists as '%s' for hash (%zu)",
                        name.c_str(), cmd.mName.c_str(), hash);
        }
    }
    // Attempt to insert the command
    m_Commands.emplace_back(hash, name, ptr, std::move(obj), m_Manager->GetCtr());
    // Return the script object of the listener
    return m_Commands.back().mObj;
}

// ------------------------------------------------------------------------------------------------
Object Manager::Create(CSStr name, CSStr spec, Array & tags, Uint8 min, Uint8 max, SQInteger auth, bool prot, bool assoc)
{
    // Is there a controller to store the listener?
    if (!m_Controller)
    {
        m_Controller = CtrRef(new Controller(this)); // Then make one!
    }
    // Script object and listener instance to be created and managed
    Object obj;
    Listener * ptr = nullptr;
    // Create the command listener
    {
        // Create a new instance of this class and make sure it can't get leaked due to exceptions
        AutoDelete< Listener > ad(new Listener(name, spec, tags, min, max, auth, prot, assoc));
        // Transform the instance into a script object
        obj = Object(ad.Get());
        // Validate the obtained script object
        if (obj.IsNull())
        {
            STHROWF("Cannot create the command listener instance");
        }
        // Grab the instance from the auto-deleter and leave the destruction to the script object
        ptr = ad.Grab();
    }
    // Attempt to attach the command listener to the controller
    Object & o = m_Controller->Attach(obj, ptr);
    // Return the script object
    return o;
}

// ------------------------------------------------------------------------------------------------
Int32 Controller::Run(const Guard & guard, CCStr command)
{
    // Validate the string command
    if (!command || *command == '\0')
    {
        // Tell the script callback to deal with the error
        SqError(CMDERR_EMPTY_COMMAND, _SC("Invalid or empty command name"), guard.mCurrent->mInvoker);
        // Execution failed!
        return -1;
    }
    // Grab a direct reference to the context instance
    Context & ctx = *(guard.mCurrent);
    // Skip white-space until the command name
    while (std::isspace(*command))
    {
        ++command;
    }
    // Anything left to process?
    if (*command == '\0')
    {
        // Tell the script callback to deal with the error
        SqError(CMDERR_EMPTY_COMMAND, _SC("Invalid or empty command name"), guard.mCurrent->mInvoker);
        // Execution failed!
        return -1;
    }
    // Where the name ends and argument begins
    CCStr split = command;
    // Find where the command name ends
    while (*split != '\0' && !std::isspace(*split))
    {
        ++split;
    }
    // Are there any arguments specified?
    if (split != '\0')
    {
        // Save the command name
        ctx.mCommand.assign(command, (split - command));
        // Skip white space after command name
        while (std::isspace(*split))
        {
            ++split;
        }
        // Save the command argument
        ctx.mArgument.assign(split);
    }
    // No arguments specified
    else
    {
        // Save the command name
        ctx.mCommand.assign(command);
        // Leave argument empty
        ctx.mArgument.assign(_SC(""));
    }
    // Do we have a valid command name?
    try
    {
        ValidateName(ctx.mCommand.c_str());
    }
    catch (const Sqrat::Exception & e)
    {
        // Tell the script callback to deal with the error
        SqError(CMDERR_INVALID_COMMAND, e.what(), guard.mCurrent->mInvoker);
        // Execution failed!
        return -1;
    }
    catch (...)
    {
        // Tell the script callback to deal with the error
        SqError(CMDERR_INVALID_COMMAND, _SC("Cannot execute invalid command name"), guard.mCurrent->mInvoker);
        // Execution failed!
        return -1;
    }
    // Attempt to find the specified command
    ctx.mObject = FindByName(ctx.mCommand);
    // Have we found anything?
    if (ctx.mObject.IsNull())
    {
        // Tell the script callback to deal with the error
        SqError(CMDERR_UNKNOWN_COMMAND, _SC("Unable to find the specified command"), ctx.mCommand);
        // Execution failed!
        return -1;
    }
    // Save the command instance
    ctx.mInstance = ctx.mObject.Cast< Listener * >();
    // Is the command instance valid? (just in case)
    if (!ctx.mInstance)
    {
        // Tell the script callback to deal with the error
        SqError(CMDERR_UNKNOWN_COMMAND, _SC("Unable to find the specified command"), ctx.mCommand);
        // Execution failed!
        return -1;
    }
    // Attempt to execute the command
    try
    {
        return Exec(ctx);
    }
    catch (const Sqrat::Exception & e)
    {
        // Tell the script callback to deal with the error
        SqError(CMDERR_EXECUTION_FAILED, _SC("Exceptions occurred during execution"), e.what());
    }
    // Execution failed
    return -1;
}

// ------------------------------------------------------------------------------------------------
Int32 Controller::Exec(Context & ctx)
{
    // Clear previous arguments
    ctx.mArgv.clear();
    // Reset the argument counter
    ctx.mArgc = 0;
    // Is this command suspended from further executions?
    if (ctx.mInstance->GetSuspended())
    {
        // Tell the script callback to deal with the error
        SqError(CMDERR_COMMAND_SUSPENDED, _SC("The command is currently suspended"), ctx.mInvoker);
        // Execution failed!
        return -1;
    }
    // Make sure the invoker has enough authority to execute this command
    else if (!ctx.mInstance->AuthCheck(ctx.mInvoker))
    {
        // Tell the script callback to deal with the error
        SqError(CMDERR_INSUFFICIENT_AUTH, _SC("Insufficient authority to execute command"), ctx.mInvoker);
        // Execution failed!
        return -1;
    }
    // Make sure an executer was specified
    else if (ctx.mInstance->GetOnExec().IsNull())
    {
        // Tell the script callback to deal with the error
        SqError(CMDERR_MISSING_EXECUTER, _SC("No executer was specified for this command"), ctx.mInvoker);
        // Execution failed!
        return -1;
    }
    // See if there are any arguments to parse
    else if (!ctx.mArgument.empty() && !Parse(ctx))
    {
        // The error message was reported while parsing
        return -1;
    }
    // Make sure we have enough arguments specified
    else if (ctx.mInstance->GetMinArgC() > ctx.mArgc)
    {
        // Tell the script callback to deal with the error
        SqError(CMDERR_INCOMPLETE_ARGS, _SC("Insufficient command arguments"), ctx.mInstance->GetMinArgC());
        // Execution failed!
        return -1;
    }
    // The check during the parsing may omit the last argument
    else if (static_cast< Uint32 >(ctx.mInstance->GetMaxArgC()) < ctx.mArgc)
    {
        // Tell the script callback to deal with the error
        SqError(CMDERR_EXTRANEOUS_ARGS, _SC("Extraneous command arguments"), ctx.mInstance->GetMaxArgC());
        // Execution failed!
        return -1;
    }
    // Check argument types against the command specifiers
    for (Uint32 arg = 0; arg < ctx.mArgc; ++arg)
    {
        if (!ctx.mInstance->ArgCheck(arg, ctx.mArgv[arg].first))
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
    ctx.mBuffer.At(0) = '\0';
    // Whether the command execution failed
    bool failed = false;
    // Do we have to call the command with an associative container?
    if (ctx.mInstance->m_Associate)
    {
        // Create the associative container
        Table args(DefaultVM::Get());
        // Copy the arguments into the table
        for (Uint32 arg = 0; arg < ctx.mArgc; ++arg)
        {
            // Do we have use the argument index as the key?
            if (ctx.mInstance->m_ArgTags[arg].empty())
            {
                args.SetValue(SQInteger(arg), ctx.mArgv[arg].second);
            }
            // Nope, we have a name for this argument!
            else
            {
                args.SetValue(ctx.mInstance->m_ArgTags[arg].c_str(), ctx.mArgv[arg].second);
            }
        }
        // Attempt to execute the command with the specified arguments
        try
        {
            result = ctx.mInstance->Execute(ctx.mInvoker, args);
        }
        catch (const Sqrat::Exception & e)
        {
            // Let's store the exception message
            ctx.mBuffer.Write(0, e.what(), e.Message().size());
            // Specify that the command execution failed
            failed = true;
        }
    }
    else
    {
        // Reserve an array for the extracted arguments
        Array args(DefaultVM::Get(), ctx.mArgc);
        // Copy the arguments into the array
        for (Uint32 arg = 0; arg < ctx.mArgc; ++arg)
        {
            args.Bind(SQInteger(arg), ctx.mArgv[arg].second);
        }
        // Attempt to execute the command with the specified arguments
        try
        {
            result = ctx.mInstance->Execute(ctx.mInvoker, args);
        }
        catch (const Sqrat::Exception & e)
        {
            // Let's store the exception message
            ctx.mBuffer.Write(0, e.what(), e.Message().size());
            // Specify that the command execution failed
            failed = true;
        }
    }
    // Was there a runtime exception during the execution?
    if (failed)
    {
        // Tell the script callback to deal with the error
        SqError(CMDERR_EXECUTION_FAILED, _SC("Command execution failed"), ctx.mBuffer.Data());
        // Is there a script callback that handles failures?
        if (!ctx.mInstance->m_OnFail.IsNull())
        {
            // Then attempt to relay the result to that function
            try
            {
                ctx.mInstance->m_OnFail.Execute(ctx.mInvoker, result);
            }
            catch (const Sqrat::Exception & e)
            {
                // Tell the script callback to deal with the error
                SqError(CMDERR_UNRESOLVED_FAILURE, _SC("Unable to resolve command failure"), e.what());
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
        if (!ctx.mInstance->m_OnFail.IsNull())
        {
            // Then attempt to relay the result to that function
            try
            {
                ctx.mInstance->m_OnFail.Execute(ctx.mInvoker, result);
            }
            catch (const Sqrat::Exception & e)
            {
                // Tell the script callback to deal with the error
                SqError(CMDERR_UNRESOLVED_FAILURE, _SC("Unable to resolve command failure"), e.what());
            }
        }
    }
    // Is there a callback that must be executed after a successful execution?
    else if (!ctx.mInstance->m_OnPost.IsNull())
    {
            // Then attempt to relay the result to that function
            try
            {
                ctx.mInstance->m_OnPost.Execute(ctx.mInvoker, result);
            }
            catch (const Sqrat::Exception & e)
            {
                // Tell the script callback to deal with the error
                SqError(CMDERR_POST_PROCESSING_FAILED, _SC("Unable to complete command post processing"), e.what());
            }
    }
    // Return the result
    return ConvTo< Int32 >::From(result);
}

// ------------------------------------------------------------------------------------------------
bool Controller::Parse(Context & ctx)
{
    // Is there anything to parse?
    if (ctx.mArgument.empty())
    {
        return true; // Done parsing!
    }
    // Obtain the flags of the currently processed argument
    Uint8 arg_flags = ctx.mInstance->m_ArgSpec[ctx.mArgc];
    // Adjust the internal buffer if necessary (mostly never)
    ctx.mBuffer.Adjust(ctx.mArgument.size());
    // The iterator to the currently processed character
    String::const_iterator itr = ctx.mArgument.cbegin();
    // Previous and currently processed character
    String::value_type prev = 0, elem = 0;
    // Maximum arguments allowed to be processed
    const Uint8 max_arg = ctx.mInstance->m_MaxArgc;
    // Process loop result
    bool good = true;
    // Process the specified command text
    while (good)
    {
        // Extract the current characters before advancing
        prev = elem, elem = *itr;
        // See if we have anything left to parse or we have what we need already
        if (elem == '\0' || ctx.mArgc >= max_arg)
        {
            break; // We only parse what we need or what we have!
        }
        // Is this a greedy argument?
        else if (arg_flags & CMDARG_GREEDY)
        {
            // Remember the current stack size
            const StackGuard sg;
            // Skip white-space characters
            itr = std::find_if(itr, ctx.mArgument.cend(), IsNotCType(std::isspace));
            // Anything left to copy to the argument?
            if (itr != ctx.mArgument.end())
            {
                // Transform it into a script object
                sq_pushstring(DefaultVM::Get(), &(*itr), std::distance(itr, ctx.mArgument.cend()));
            }
            // Just push an empty string
            else
            {
                sq_pushstring(DefaultVM::Get(), _SC(""), 0);
            }
            // Get the object from the stack and add it to the argument list along with it's type
            ctx.mArgv.emplace_back(CMDARG_STRING, Var< Object >(DefaultVM::Get(), -1).value);
            // Include this argument into the count
            ++ctx.mArgc;
            // Nothing left to parse
            break;
        }
        // Do we have to extract a string argument?
        else if ((elem == '\'' || elem == '"') && prev != '\\')
        {
            // Obtain the beginning and ending of the internal buffer
            SStr str = ctx.mBuffer.Begin< SQChar >();
            SStr end = (ctx.mBuffer.End< SQChar >() - 1); // + null terminator
            // Save the closing quote type
            const SQChar close = elem;
            // Skip the opening quote
            ++itr;
            // Attempt to consume the string argument
            while (good)
            {
                // Extract the current characters before advancing
                prev = elem, elem = *itr;
                // See if there's anything left to parse
                if (elem == '\0')
                {
                    // Tell the script callback to deal with the error
                    SqError(CMDERR_SYNTAX_ERROR, _SC("String argument not closed properly"), ctx.mArgc);
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
                        *str = '\0';
                        // Stop parsing
                        break;
                    }
                    // Overwrite last character when replicating
                    else
                    {
                        --str;
                    }
                }
                // See if the internal buffer needs to scale
                else if  (str >= end)
                {
                    // We should already have a buffer as big as the entire command!
                    SqError(CMDERR_BUFFER_OVERFLOW, _SC("Command buffer was exceeded unexpectedly"), ctx.mInvoker);
                    // Parsing aborted
                    good = false;
                    // Stop parsing
                    break;
                }
                // Simply replicate the character to the internal buffer
                *str = elem;
                // Advance to the next character
                ++str, ++itr;
            }
            // See if the argument was valid
            if (!good)
            {
                break; // Propagate the failure!
            }
            // Swap the beginning and ending of the extracted string
            end = str, str = ctx.mBuffer.Begin();
            // Make sure the string is null terminated
            *end = '\0';
            // Do we have to make the string lowercase?
            if (arg_flags & CMDARG_LOWER)
            {
                for (CStr chr = str; chr < end; ++chr)
                {
                    *chr = static_cast< SQChar >(std::tolower(*chr));
                }
            }
            // Do we have to make the string uppercase?
            else if (arg_flags & CMDARG_UPPER)
            {
                for (CStr chr = str; chr < end; ++chr)
                {
                    *chr = static_cast< SQChar >(std::toupper(*chr));
                }
            }
            // Remember the current stack size
            const StackGuard sg;
            // Was the specified string empty?
            if (str >= end)
            {
                // Just push an empty string
                sq_pushstring(DefaultVM::Get(), _SC(""), 0);
            }
            // Add it to the argument list along with it's type
            else
            {
                // Transform it into a script object
                sq_pushstring(DefaultVM::Get(), str, end - str - 1);
            }
            // Get the object from the stack and add it to the argument list along with it's type
            ctx.mArgv.emplace_back(CMDARG_STRING, Var< Object >(DefaultVM::Get(), -1).value);
            // Advance to the next argument and obtain its flags
            arg_flags = ctx.mInstance->m_ArgSpec[++ctx.mArgc];
        }
        // Ignore white-space characters until another valid character is found
        else if (!std::isspace(elem) && (std::isspace(prev) || prev == '\0'))
        {
            // Find the first space character that marks the end of the argument
            String::const_iterator pos = std::find_if(itr, ctx.mArgument.cend(), IsCType(std::isspace));
            // Obtain both ends of the argument string
            CCStr str = &(*itr), end = &(*pos);
            // Compute the argument string size
            const Uint32 sz = std::distance(itr, pos);
            // Update the main iterator position
            itr = pos;
            // Update the currently processed character
            elem = *itr;
            // Used to exclude all other checks when a valid type was identified
            bool identified = false;
            // Attempt to treat the value as an integer number if possible
            if (!identified && (arg_flags & CMDARG_INTEGER))
            {
                // Let's us know if the whole argument was part of the resulted value
                CStr next = nullptr;
                // Attempt to extract the integer value from the string
                const Int64 value = std::strtoll(str, &next, 10);
                // See if this whole string was indeed an integer
                if (next == end)
                {
                    // Remember the current stack size
                    const StackGuard sg;
                    // Transform it into a script object
                    sq_pushinteger(DefaultVM::Get(), ConvTo< SQInteger >::From(value));
                    // Get the object from the stack and add it to the argument list along with it's type
                    ctx.mArgv.emplace_back(CMDARG_INTEGER, Var< Object >(DefaultVM::Get(), -1).value);
                    // We've identified the correct value type
                    identified = true;
                }
            }
            // Attempt to treat the value as an floating point number if possible
            if (!identified && (arg_flags & CMDARG_FLOAT))
            {
                // Let's us know if the whole argument was part of the resulted value
                CStr next = nullptr;
                // Attempt to extract the integer value from the string
#ifdef SQUSEDOUBLE
                const Float64 value = std::strtod(str, &next);
#else
                const Float32 value = std::strtof(str, &next);
#endif // SQUSEDOUBLE
                // See if this whole string was indeed an integer
                if (next == end)
                {
                    // Remember the current stack size
                    const StackGuard sg;
                    // Transform it into a script object
                    sq_pushfloat(DefaultVM::Get(), ConvTo< SQFloat >::From(value));
                    // Get the object from the stack and add it to the argument list along with it's type
                    ctx.mArgv.emplace_back(CMDARG_FLOAT, Var< Object >(DefaultVM::Get(), -1).value);
                    // We've identified the correct value type
                    identified = true;
                }

            }
            // Attempt to treat the value as a boolean if possible
            if (!identified && (arg_flags & CMDARG_BOOLEAN) && sz <= 5)
            {
                // Allocate memory for enough data to form a boolean value
                CharT lc[6];
                // Don't modify the original string or buffer pointer
                CStr bptr = lc;
                CSStr sptr = str;
                // Fill the temporary buffer with data from the internal buffer
                for (; sptr < end; ++sptr, ++bptr)
                {
                    *bptr = std::tolower(*sptr);
                }
                // Terminate the copied string portion
                *bptr = '\0';
                // Remember the current stack size
                const StackGuard sg;
                // Is this a boolean true value?
                if (std::strcmp(lc, "true") == 0 || std::strcmp(lc, "on") == 0)
                {
                    // Transform it into a script object
                    sq_pushbool(DefaultVM::Get(), true);
                    // We've identified the correct value type
                    identified = true;
                }
                // Is this a boolean false value?
                else if (std::strcmp(lc, "false") == 0 || std::strcmp(lc, "off") == 0)
                {
                    // Transform it into a script object
                    sq_pushbool(DefaultVM::Get(), false);
                    // We've identified the correct value type
                    identified = true;
                }
                // Could the value inside the string be interpreted as a boolean?
                if (identified)
                {
                    // Get the object from the stack and add it to the argument list along with it's type
                    ctx.mArgv.emplace_back(CMDARG_BOOLEAN, Var< Object >(DefaultVM::Get(), -1).value);
                }
            }
            // If everything else failed then simply treat the value as a string
            if (!identified)
            {
                // Remember the current stack size
                const StackGuard sg;
                // Do we have to make the string lowercase?
                if (arg_flags & CMDARG_LOWER)
                {
                    // Convert all characters from the argument string into the buffer
                    for (CStr chr = ctx.mBuffer.Data(); str < end; ++str, ++chr)
                    {
                        *chr = static_cast< CharT >(std::tolower(*str));
                    }
                    // Transform it into a script object
                    sq_pushstring(DefaultVM::Get(), ctx.mBuffer.Get< SQChar >(), sz);
                }
                // Do we have to make the string uppercase?
                else if (arg_flags & CMDARG_UPPER)
                {
                    // Convert all characters from the argument string into the buffer
                    for (CStr chr = ctx.mBuffer.Data(); str < end; ++str, ++chr)
                    {
                        *chr = static_cast< CharT >(std::toupper(*str));
                    }
                    // Transform it into a script object
                    sq_pushstring(DefaultVM::Get(), ctx.mBuffer.Get< SQChar >(), sz);
                }
                else
                {
                    // Transform it into a script object
                    sq_pushstring(DefaultVM::Get(), str, sz);
                }
                // Get the object from the stack and add it to the argument list along with it's type
                ctx.mArgv.emplace_back(CMDARG_STRING, Var< Object >(DefaultVM::Get(), -1).value);
            }
            // Advance to the next argument and obtain its flags
            arg_flags = ctx.mInstance->m_ArgSpec[++ctx.mArgc];
        }
        // Is there anything left to parse?
        if (itr >= ctx.mArgument.end())
        {
            break;
        }
        // Advance to the next character
        ++itr;
    }
    // Return whether the parsing was successful
    return good;
}

// ------------------------------------------------------------------------------------------------
void Listener::GenerateInfo(bool full)
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
void Listener::ProcSpec(CSStr str)
{
    // Reset current argument specifiers
    std::memset(m_ArgSpec, CMDARG_ANY, sizeof(m_ArgSpec));
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
                    STHROWF("Extraneous type specifiers: %d >= %d", idx, SQMOD_MAX_CMD_ARGS);
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
                while (*str != '\0' && !std::isalpha(*str))
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
                    default: STHROWF("Unknown type specifier (%c) at argument: %u", *str, idx);
                }
            }
        }
    }
    catch (const Sqrat::Exception & e)
    {
        // Reset all argument specifiers if failed
        std::memset(m_ArgSpec, CMDARG_ANY, sizeof(m_ArgSpec));
        // Propagate the exception back to the caller
        throw e;
    }
    // Attempt to generate an informational message
    GenerateInfo(false);
}

// ================================================================================================
void Register(HSQUIRRELVM vm)
{
    Table cmdns(vm);

    cmdns.Bind(_SC("Manager"),
        Class< Manager, NoCopy< Manager > >(vm, ManagerTypename::Str)
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &ManagerTypename::Fn)
        .Func(_SC("cmp"), &Manager::Cmp)
        .Func(_SC("_tostring"), &Manager::ToString)
        // Member Properties
        .Prop(_SC("Count"), &Manager::GetCount)
        .Prop(_SC("References"), &Manager::GetRefCount)
        .Prop(_SC("IsContext"), &Manager::IsContext)
        .Prop(_SC("OnFail"), &Manager::GetOnFail)
        .Prop(_SC("OnAuth"), &Manager::GetOnAuth)
        .Prop(_SC("Invoker"), &Manager::GetInvoker)
        .Prop(_SC("Listener"), &Manager::GetListener)
        .Prop(_SC("Command"), &Manager::GetCommand)
        .Prop(_SC("Argument"), &Manager::GetArgument)
        // Member Methods
        .Func(_SC("Run"), &Manager::Run)
        .Func(_SC("Sort"), &Manager::Sort)
        .Func(_SC("Clear"), &Manager::Clear)
        .Func(_SC("Attach"), &Manager::Attach)
        .Func(_SC("FindByName"), &Manager::FindByName)
        .Func(_SC("BindFail"), &Manager::SetOnFail)
        .Func(_SC("BindAuth"), &Manager::SetOnAuth)
        .Func(_SC("GetArray"), &Manager::GetCommandsArray)
        .Func(_SC("GetTable"), &Manager::GetCommandsTable)
        .Func(_SC("Foreach"), &Manager::ForeachCommand)
        // Member Overloads
        .Overload< Object (Manager::*)(CSStr) >(_SC("Create"), &Manager::Create)
        .Overload< Object (Manager::*)(CSStr, CSStr) >(_SC("Create"), &Manager::Create)
        .Overload< Object (Manager::*)(CSStr, CSStr, Array &) >(_SC("Create"), &Manager::Create)
        .Overload< Object (Manager::*)(CSStr, CSStr, Uint8, Uint8) >(_SC("Create"), &Manager::Create)
        .Overload< Object (Manager::*)(CSStr, CSStr, Array &, Uint8, Uint8) >(_SC("Create"), &Manager::Create)
        .Overload< Object (Manager::*)(CSStr, CSStr, Array &, Uint8, Uint8, SQInteger) >(_SC("Create"), &Manager::Create)
        .Overload< Object (Manager::*)(CSStr, CSStr, Array &, Uint8, Uint8, SQInteger, bool) >(_SC("Create"), &Manager::Create)
        .Overload< Object (Manager::*)(CSStr, CSStr, Array &, Uint8, Uint8, SQInteger, bool, bool) >(_SC("Create"), &Manager::Create)
    );

    cmdns.Bind(_SC("Listener"),
        Class< Listener, NoCopy< Listener > >(vm, ListenerTypename::Str)
        // Constructors
        .Ctor< CSStr >()
        .Ctor< CSStr, CSStr >()
        .Ctor< CSStr, CSStr, Array & >()
        .Ctor< CSStr, CSStr, Uint8, Uint8 >()
        .Ctor< CSStr, CSStr, Array &, Uint8, Uint8 >()
        .Ctor< CSStr, CSStr, Array &, Uint8, Uint8, SQInteger >()
        .Ctor< CSStr, CSStr, Array &, Uint8, Uint8, SQInteger, bool >()
        .Ctor< CSStr, CSStr, Array &, Uint8, Uint8, SQInteger, bool, bool >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &ListenerTypename::Fn)
        .Func(_SC("_cmp"), &Listener::Cmp)
        .Func(_SC("_tostring"), &Listener::ToString)
        // Member Properties
        .Prop(_SC("References"), &Listener::GetRefCount)
        .Prop(_SC("Attached"), &Listener::Attached)
        .Prop(_SC("Manager"), &Listener::GetManager)
        .Prop(_SC("Name"), &Listener::GetName, &Listener::SetName)
        .Prop(_SC("Data"), &Listener::GetData, &Listener::SetData)
        .Prop(_SC("Spec"), &Listener::GetSpec, &Listener::SetSpec)
        .Prop(_SC("Specifier"), &Listener::GetSpec, &Listener::SetSpec)
        .Prop(_SC("Tags"), &Listener::GetArgTags, &Listener::SetArgTags)
        .Prop(_SC("Help"), &Listener::GetHelp, &Listener::SetHelp)
        .Prop(_SC("Info"), &Listener::GetInfo, &Listener::SetInfo)
        .Prop(_SC("Authority"), &Listener::GetAuthority, &Listener::SetAuthority)
        .Prop(_SC("Protected"), &Listener::GetProtected, &Listener::SetProtected)
        .Prop(_SC("Suspended"), &Listener::GetSuspended, &Listener::SetSuspended)
        .Prop(_SC("Associate"), &Listener::GetAssociate, &Listener::SetAssociate)
        .Prop(_SC("MinArgs"), &Listener::GetMinArgC, &Listener::SetMinArgC)
        .Prop(_SC("MaxArgs"), &Listener::GetMaxArgC, &Listener::SetMaxArgC)
        .Prop(_SC("OnExec"), &Listener::GetOnExec)
        .Prop(_SC("OnAuth"), &Listener::GetOnAuth)
        .Prop(_SC("OnPost"), &Listener::GetOnPost)
        .Prop(_SC("OnFail"), &Listener::GetOnFail)
        // Member Methods
        .Func(_SC("Attach"), &Listener::Attach)
        .Func(_SC("Detach"), &Listener::Detach)
        .Func(_SC("BindExec"), &Listener::SetOnExec)
        .Func(_SC("BindAuth"), &Listener::SetOnAuth)
        .Func(_SC("BindPost"), &Listener::SetOnPost)
        .Func(_SC("BindFail"), &Listener::SetOnFail)
        .Func(_SC("GetArgTag"), &Listener::GetArgTag)
        .Func(_SC("SetArgTag"), &Listener::SetArgTag)
        .Func(_SC("GetArgFlags"), &Listener::GetArgFlags)
        .Func(_SC("ArgCheck"), &Listener::ArgCheck)
        .Func(_SC("AuthCheck"), &Listener::AuthCheck)
        .Func(_SC("GenerateInfo"), &Listener::GenerateInfo)
    );

    RootTable(vm).Bind(_SC("SqCmd"), cmdns);

    ConstTable(vm).Enum(_SC("SqCmdArg"), Enumeration(vm)
        .Const(_SC("Any"),                  CMDARG_ANY)
        .Const(_SC("Integer"),              CMDARG_INTEGER)
        .Const(_SC("Float"),                CMDARG_FLOAT)
        .Const(_SC("Boolean"),              CMDARG_BOOLEAN)
        .Const(_SC("String"),               CMDARG_STRING)
        .Const(_SC("Lower"),                CMDARG_LOWER)
        .Const(_SC("Upper"),                CMDARG_UPPER)
        .Const(_SC("Greedy"),               CMDARG_GREEDY)
    );

    ConstTable(vm).Enum(_SC("SqCmdErr"), Enumeration(vm)
        .Const(_SC("Unknown"),              CMDERR_UNKNOWN)
        .Const(_SC("EmptyCommand"),         CMDERR_EMPTY_COMMAND)
        .Const(_SC("InvalidCommand"),       CMDERR_INVALID_COMMAND)
        .Const(_SC("SyntaxError"),          CMDERR_SYNTAX_ERROR)
        .Const(_SC("UnknownCommand"),       CMDERR_UNKNOWN_COMMAND)
        .Const(_SC("ListenerSuspended"),    CMDERR_COMMAND_SUSPENDED)
        .Const(_SC("InsufficientAuth"),     CMDERR_INSUFFICIENT_AUTH)
        .Const(_SC("MissingExecuter"),      CMDERR_MISSING_EXECUTER)
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

} // Namespace:: Cmd

// ------------------------------------------------------------------------------------------------
void Register_Command(HSQUIRRELVM vm)
{
    Cmd::Register(vm);
}

/* ------------------------------------------------------------------------------------------------
 * Forward the call to terminate the command manager.
*/
void TerminateCommands()
{
    Cmd::Controller::Terminate();
    Cmd::Listener::Terminate();
}

} // Namespace:: SqMod
