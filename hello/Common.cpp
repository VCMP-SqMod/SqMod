// ------------------------------------------------------------------------------------------------
#include "Common.hpp"
// ------------------------------------------------------------------------------------------------
#ifdef SQMOD_OS_WINDOWS
    #include <windows.h>
#endif // SQMOD_OS_WINDOWS
// ------------------------------------------------------------------------------------------------
#include <cstdarg>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
PluginFuncs*        _Func = nullptr;
PluginCallbacks*    _Clbk = nullptr;
PluginInfo*         _Info = nullptr;

/* ------------------------------------------------------------------------------------------------
 * Common buffers to reduce memory allocations. To be immediately copied upon return!
*/
static SQChar g_Buffer[4096];

// ------------------------------------------------------------------------------------------------
void SqThrowF(const SQChar * str, ...)
{
    // Initialize the argument list
    va_list args;
    va_start (args, str);
    // Write the requested contents
    if (std::vsnprintf(g_Buffer, sizeof(g_Buffer), str, args) < 0)
    {
        // Write a generic message at least
        std::strcpy(g_Buffer, "Unknown error has occurred");
    }
    // Finalize the argument list
    va_end(args);
    // Throw the exception with the resulted message
    throw Sqrat::Exception(g_Buffer); // NOLINT(hicpp-exception-baseclass,cert-err60-cpp)
}

/* ------------------------------------------------------------------------------------------------
 * Raw console message output.
*/
static inline void OutputMessageImpl(const SQChar * msg, va_list args)
{
#ifdef SQMOD_OS_WINDOWS
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csb_before;
    GetConsoleScreenBufferInfo( hstdout, &csb_before);
    SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN);
    std::printf("[SQMOD] ");

    SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); // NOLINT(hicpp-signed-bitwise)
    std::vprintf(msg, args);
    std::puts("");

    SetConsoleTextAttribute(hstdout, csb_before.wAttributes);
#else
    std::printf("\033[21;32m[SQMOD]\033[0m");
    std::vprintf(msg, args);
    std::puts("");
#endif // SQMOD_OS_WINDOWS
}

/* ------------------------------------------------------------------------------------------------
 * Raw console error output.
*/
static inline void OutputErrorImpl(const SQChar * msg, va_list args)
{
#ifdef SQMOD_OS_WINDOWS
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csb_before;
    GetConsoleScreenBufferInfo( hstdout, &csb_before);
    SetConsoleTextAttribute(hstdout, FOREGROUND_RED | FOREGROUND_INTENSITY); // NOLINT(hicpp-signed-bitwise)
    std::printf("[SQMOD] ");

    SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY); // NOLINT(hicpp-signed-bitwise)
    std::vprintf(msg, args);
    std::puts("");

    SetConsoleTextAttribute(hstdout, csb_before.wAttributes);
#else
    std::printf("\033[21;91m[SQMOD]\033[0m");
    std::vprintf(msg, args);
    std::puts("");
#endif // SQMOD_OS_WINDOWS
}

// ------------------------------------------------------------------------------------------------
void OutputDebug(const SQChar * msg, ...)
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

// ------------------------------------------------------------------------------------------------
void OutputMessage(const SQChar * msg, ...)
{
    // Initialize the arguments list
    va_list args;
    va_start(args, msg);
    // Call the output function
    OutputMessageImpl(msg, args);
    // Finalize the arguments list
    va_end(args);
}

// ------------------------------------------------------------------------------------------------
void OutputError(const SQChar * msg, ...)
{
    // Initialize the arguments list
    va_list args;
    va_start(args, msg);
    // Call the output function
    OutputErrorImpl(msg, args);
    // Finalize the arguments list
    va_end(args);
}

// ------------------------------------------------------------------------------------------------
bool CheckModuleAPIVer(const SQChar * ver, const SQChar * mod)
{
    // Obtain the numeric representation of the API version
    const long int vernum = std::strtol(ver, nullptr, 10);
    // Check against version mismatch
    if (vernum == SQMOD_API_VER)
    {
        return true;
    }
    // Log the incident
    OutputError("API version mismatch on %s", mod);
    OutputMessage("=> Requested: %ld Have: %ld", vernum, SQMOD_API_VER);
    // Invoker should not attempt to communicate through the module API
    return false;
}

// ------------------------------------------------------------------------------------------------
bool CheckModuleOrder(PluginFuncs * vcapi, uint32_t mod_id, const SQChar * mod)
{
    // Make sure a valid server API was provided
    if (!vcapi)
    {
        OutputError("Invalid pointer to server API structure");
        // Validation failed!
        return false;
    }
    // Attempt to find the host plug-in identifier
    const int plugin_id = vcapi->FindPlugin(SQMOD_HOST_NAME);
    // See if our module was loaded after the host plug-in
    if (plugin_id < 0)
    {
        OutputError("%s: could find the host plug-in", mod);
        // Validation failed!
        return false;
    }
    // Should never reach this point but just in case
    else if (static_cast< uint32_t >(plugin_id) > mod_id)
    {
        OutputError("%s: loaded after the host plug-in", mod);
        // Validation failed!
        return false;
    }
    // Loaded in the correct order
    return true;
}

// ------------------------------------------------------------------------------------------------
void ImportModuleAPI(PluginFuncs * vcapi, const SQChar * mod)
{
    // Make sure a valid server API was provided
    if (!vcapi)
    {
        STHROWF("%s: Invalid pointer to server API structure", mod);
    }

    size_t exports_struct_size;

    // Attempt to find the host plug-in identifier
    int plugin_id = vcapi->FindPlugin(SQMOD_HOST_NAME);
    // Validate the obtained plug-in identifier
    if (plugin_id < 0)
    {
        STHROWF("%s: Unable to obtain the host plug-in identifier", mod);
    }

    // Attempt to retrieve the host plug-in exports
    const void ** raw_plugin_exports = vcapi->GetPluginExports(plugin_id, &exports_struct_size);
    // See if the size of the exports structure matches
    if (exports_struct_size <= 0)
    {
        STHROWF("%s: Incompatible host plug-in exports structure", mod);
    }
    // See if we have any exports from the host plug-in
    else if (raw_plugin_exports == nullptr)
    {
        STHROWF("%s: Unable to obtain pointer host plug-in exports", mod);
    }

    // Obtain pointer to the exports structure
    const SQMODEXPORTS * plugin_exports = *reinterpret_cast< const SQMODEXPORTS ** >(raw_plugin_exports);
    // See if we have a valid pointer to the exports structure
    if (plugin_exports == nullptr)
    {
        STHROWF("%s: Invalid pointer to host plug-in exports structure", mod);
    }
    else if (plugin_exports->PopulateModuleAPI == nullptr || plugin_exports->PopulateSquirrelAPI == nullptr)
    {
        STHROWF("%s: Invalid pointer to host plug-in import functions", mod);
    }

    // Prepare a structure to obtain the module API
    SQMODAPI sqmodapi;
    // Attempt to populate the structure
    switch (plugin_exports->PopulateModuleAPI(&sqmodapi, sizeof(SQMODAPI)))
    {
        case -1:    STHROWF("%s: Incompatible module API structure", mod);
        // fall through
        case 0:     STHROWF("%s: Invalid pointer to module API structure", mod);
    }

    // Prepare a structure to obtain the squirrel API
    SQLIBAPI sqlibapi;
    // Attempt to populate the structure
    switch (plugin_exports->PopulateSquirrelAPI(&sqlibapi, sizeof(SQLIBAPI)))
    {
        case -1:    STHROWF("%s: Incompatible squirrel API structure", mod);
        // fall through
        case 0:     STHROWF("%s: Invalid pointer to squirrel API structure", mod);
    }

    // Attempt to expand the obtained API
    if (!sqmod_api_expand(&sqmodapi))
    {
        // Collapse the API first
        sqmod_api_collapse();
        // Now it's safe to throw the exception
        STHROWF("%s: Unable to expand module API structure", mod);
    }
    else if (!sqlib_api_expand(&sqlibapi))
    {
        // Collapse the API first
        sqmod_api_collapse();
        sqlib_api_collapse();
        // Now it's safe to throw the exception
        STHROWF("%s: Unable to expand module API structure", mod);
    }
}

// ------------------------------------------------------------------------------------------------
int SampleFunction()
{
    OutputMessage("Hello from the sample plug-in function!");
    return rand();
}

// ------------------------------------------------------------------------------------------------
void SampleType::SampleMethod() const
{
    OutputMessage("I have the values %d and %d", m_MyVal, mMyNum);
}

} // Namespace:: SqMod
