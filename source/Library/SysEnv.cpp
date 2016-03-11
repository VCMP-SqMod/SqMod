// ------------------------------------------------------------------------------------------------
#include "Library/SysEnv.hpp"

// ------------------------------------------------------------------------------------------------
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <utility>

// ------------------------------------------------------------------------------------------------
#ifdef SQMOD_OS_WINDOWS
    #include <windows.h>
    #include <shlobj.h>
#else
    #include <sys/utsname.h>
    #include <unistd.h>
    #include <pwd.h>
#endif

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
#ifdef SQMOD_OS_WINDOWS
    // Maximum path size in characters
    #define SQMOD_MAX_PATH (sizeof(TCHAR) * MAX_PATH)
    // Character to be used when working with path
    typedef TCHAR PChar;
#else
    // Maximum path size in characters
    #define SQMOD_MAX_PATH (PATH_MAX)
    // Character to be used when working with path
    typedef CharT PChar;
#endif // SQMOD_OS_WINDOWS

// ------------------------------------------------------------------------------------------------
void SysEnv::Get(Buffer & b, CCStr name, CCStr fallback)
{
    // Make sure the requested variable name is valid
    if (name && *name != 0)
    {
        // Is there a buffer to work with?
        if (!b)
        {
            // Acquire a moderately sized buffer
            b = Buffer(128);
        }
#ifdef SQMOD_OS_WINDOWS
        // Retrieve the variable contents into the buffer that we have
        DWORD len = GetEnvironmentVariableA(name, &b.Cursor(), b.Remaining());
        // If the returned length is 0 then the variable doesn't exist
        if (!len)
        {
            // Write the fall-back value into the buffer instead
            len = b.WriteS(b.Position(), fallback);
        }
        // Did we have enough space left in the buffer?
        else if (len > b.Remaining())
        {
            // Acquire a new buffer with a more appropriate capacity this time
            b.Grow(len - b.Remaining() + 2);
            // Attempt to retrieve the variable contents one more time
            len = GetEnvironmentVariableA(name, &b.Cursor(), b.Remaining());
        }
        // Move the edit cursor to the end of the appended data
        b.Advance(len);
#else
        // Retrieve the pointer to the variable contents
        CSStr val = getenv(name);
        // If the returned pointer is null then the variable doesn't exist
        if (!val)
        {
            // Write the fall-back value into the buffer instead
            Buffer.AppendS(fallback);
        }
        else
        {
            // Write the variable contents to the buffer
            b.AppendS(val);
        }
#endif
    }
    // Make sure that whatever string is in the buffer is null terminated
    b.Cursor() = '\0';
}

// ------------------------------------------------------------------------------------------------
bool SysEnv::Has(CCStr name)
{
#ifdef SQMOD_OS_WINDOWS
    return (GetEnvironmentVariableA(name, nullptr, 0) > 0);
#else
    return (getenv(name) != 0);
#endif
}

// ------------------------------------------------------------------------------------------------
bool SysEnv::Has(const String & name)
{
#ifdef SQMOD_OS_WINDOWS
    return (GetEnvironmentVariableA(name.c_str(), nullptr, 0) > 0);
#else
    return (getenv(name.c_str()) != 0);
#endif
}

// ------------------------------------------------------------------------------------------------
Buffer SysEnv::Get(CCStr name, CCStr fallback)
{
    // Allocate a moderately sized buffer
    Buffer b(128);
    // Forward the call to the shared function
    Get(b, name, fallback);
    // Return ownership of the buffer
    return std::move(b);
}

// ------------------------------------------------------------------------------------------------
bool SysEnv::Set(CCStr name, CCStr value)
{
#ifdef SQMOD_OS_WINDOWS
    // Set the specified environment variable and return the result
    return (SetEnvironmentVariableA(name, value) != 0);
#else
    // Allocated a moderately sized buffer
    Buffer b(256);
    // Generate the necessary set command
    b.WriteF(0, "%s=%s", name, value);
    // Set the resulted environment variable and return the result
    return (putenv(b.Data()) == 0);
#endif
}

// ------------------------------------------------------------------------------------------------
bool SysEnv::Set(const String & name, const String & value)
{
#ifdef SQMOD_OS_WINDOWS
    // Set the specified environment variable and return the result
    return (SetEnvironmentVariableA(name.c_str(), value.c_str()) != 0);
#else
    // Obtain a temporary buffer capable of holding the set command
    Buffer b(name.size() + value.size() + 2);
    // Generate the necessary set command
    b.WriteF(0, "%s=%s", name.c_str(), value.c_str());
    // Set the resulted environment variable and return the result
    return (putenv(b.Data()) == 0);
#endif
}

// ------------------------------------------------------------------------------------------------
String SysEnv::OSName()
{
#ifdef SQMOD_OS_WINDOWS
    // Prepare the structure in which the OS information is retrieved
    OSVERSIONINFO vi;
    // Specify the size of the structure
    vi.dwOSVersionInfoSize = sizeof(vi);
    // Attempt to populate the previously created structure with information
    if (GetVersionEx(&vi) == 0)
    {
        return "Unknown Windows";
    }
    // Identify the platform from the obtained information
    switch (vi.dwPlatformId)
    {
    case VER_PLATFORM_WIN32s:
        return "Windows 3.x";
    case VER_PLATFORM_WIN32_WINDOWS:
        return vi.dwMinorVersion == 0 ? "Windows 95" : "Windows 98";
    case VER_PLATFORM_WIN32_NT:
        return "Windows NT";
    default:
        return "Windows [Unknown]";
    }
#else
    // Prepare the structure in which the OS information is retrieved
    struct utsname uts;
    // Attempt to populate the previously created structure with information
    if (uname(&uts) < 0)
    {
        return String("Unknown Unix");
    }
    // Return the requested information
    return uts.sysname;
#endif
}

// ------------------------------------------------------------------------------------------------
String SysEnv::OSDisplayName()
{
#ifdef SQMOD_OS_WINDOWS
    // Prepare the structure in which the OS information is retrieved
    OSVERSIONINFO vi;
    // Specify the size of the structure
    vi.dwOSVersionInfoSize = sizeof(vi);
    // Attempt to populate the previously created structure with information
    if (GetVersionEx(&vi) == 0)
    {
        return "Unknown Windows";
    }
    // Identify the platform from the obtained information
    switch(vi.dwMajorVersion)
    {
    case 6:
        switch (vi.dwMinorVersion)
        {
        case 0:     return "Windows Vista/Server 2008";
        case 1:     return "Windows 7/Server 2008 R2";
        case 2:     return "Windows 8/Server 2012";
        default:    return "Windows 6.x [Unknown]";
        }
    case 5:
        switch (vi.dwMinorVersion)
        {
        case 0:     return "Windows 2000";
        case 1:     return "Windows XP";
        case 2:     return "Windows Server 2003/Windows Server 2003 R2";
        default:    return "Windows 5.x [Unknown]";
        }
    case 4:
        switch (vi.dwMinorVersion)
        {
        case 0:     return "Windows 95/Windows NT 4.0";
        case 10:    return "Windows 98";
        case 90:    return "Windows ME";
        default:    return "Windows 4.x [Unknown]";
        }
    default:        return "Windows [Unknown]";
    }
#else
    // Use the same same output from OSName
    return OSName();
#endif
}

// ------------------------------------------------------------------------------------------------
String SysEnv::OSVersion()
{
#ifdef SQMOD_OS_WINDOWS
    // Prepare the structure in which the OS information is retrieved
    OSVERSIONINFO vi;
    // Specify the size of the structure
    vi.dwOSVersionInfoSize = sizeof(vi);
    // Attempt to populate the previously created structure with information
    if (GetVersionEx(&vi) == 0)
    {
        String("Unknown");
    }
    // Obtain a temporary buffer capable of holding the version string
    Buffer b(128);
    // The amount of data written to the buffer
    Uint32 sz = 0;
    // Generate the version string with the received information
    if (vi.szCSDVersion[0])
    {
        sz = b.WriteF(0, "%lu.%lu (Build %lu : %s)",
                        vi.dwMajorVersion, vi.dwMinorVersion, vi.dwBuildNumber, vi.szCSDVersion);
    }
    else
    {
        sz = b.WriteF(0, "%lu.%lu (Build %lu)", vi.dwMajorVersion, vi.dwMinorVersion, vi.dwBuildNumber);
    }
    // Return a string with the buffer contents and leave the buffer clean after itself
    return String(b.Get< String::value_type >(), sz);
#else
    // Prepare the structure in which the OS information is retrieved
    struct utsname uts;
    // Attempt to populate the previously created structure with information
    if (uname(&uts) < 0)
    {
        return String("Unknown");
    }
    // Return the requested information
    return uts.release;
#endif
}

// ------------------------------------------------------------------------------------------------
String SysEnv::OSArchitecture()
{
#ifdef SQMOD_OS_WINDOWS
    // Prepare the structure in which the system information is retrieved
    SYSTEM_INFO si;
    // Attempt to populate the previously created structure with information
    GetSystemInfo(&si);
    // Identify the architecture from the obtained information
    switch (si.wProcessorArchitecture)
    {
    case PROCESSOR_ARCHITECTURE_INTEL:          return "IA32";
    case PROCESSOR_ARCHITECTURE_MIPS:           return "MIPS";
    case PROCESSOR_ARCHITECTURE_ALPHA:          return "ALPHA";
    case PROCESSOR_ARCHITECTURE_PPC:            return "PPC";
    case PROCESSOR_ARCHITECTURE_IA64:           return "IA64";
#ifdef PROCESSOR_ARCHITECTURE_IA32_ON_WIN64
    case PROCESSOR_ARCHITECTURE_IA32_ON_WIN64:  return "IA64/32";
#endif
#ifdef PROCESSOR_ARCHITECTURE_AMD64
    case PROCESSOR_ARCHITECTURE_AMD64:          return "AMD64";
#endif
    default:                                    return "Unknown";
    }
#else
    // Prepare the structure in which the OS information is retrieved
    struct utsname uts;
    // Attempt to populate the previously created structure with information
    if (uname(&uts) < 0)
    {
        return String("Unknown");
    }
    // Return the requested information
    return uts.machine;
#endif
}

// ------------------------------------------------------------------------------------------------
String SysEnv::NodeName()
{
#ifdef SQMOD_OS_WINDOWS
    // Obtain a temporary buffer capable of holding the node name string
    Buffer b(MAX_COMPUTERNAME_LENGTH + 1);
    // Used to tell the size of our buffer and the size of data written to it
    DWORD size = b.Size< TCHAR >();
    // Attempt to obtain the requested information
    if (GetComputerNameA(b.Data(), &size) == 0)
    {
        return String();
    }
    // Return a string with the buffer contents and leave the buffer clean after itself
    return String(b.Get< String::value_type >(), size);
#else
    // Prepare the structure in which the OS information is retrieved
    struct utsname uts;
    // Attempt to populate the previously created structure with information
    if (uname(&uts) < 0)
    {
        return String("Unknown");
    }
    // Return the requested information
    return uts.nodename;
#endif
}

// ------------------------------------------------------------------------------------------------
Uint32 SysEnv::ProcessorCount()
{
#ifdef SQMOD_OS_WINDOWS
    // Prepare the structure in which the system information is retrieved
    SYSTEM_INFO si;
    // Attempt to populate the previously created structure with information
    GetSystemInfo(&si);
    // Return the requested information
    return si.dwNumberOfProcessors;
#elif defined(_SC_NPROCESSORS_ONLN)
    // Attempt to obtain the number of processors available on the system
    const Int32 count = sysconf(_SC_NPROCESSORS_ONLN);
    // Validate the result and return the appropriate value
    return (count < 0) ? 1 : static_cast< Uint32 >(count);
#else
    // Obviously at least one processor should be available
    return 1;
#endif
}

// ------------------------------------------------------------------------------------------------
void SysEnv::TerminatePath(Buffer & b)
{
    // Is there any path to terminate?
    if (!b)
    {
        return;
    }
    // Make sure that the path contains a trailing slash if necessary
    else if (b.Cursor() == 0 && b.Before() != SQMOD_DIRSEP_CHAR)
    {
        b.Push(SQMOD_DIRSEP_CHAR);
    }
    // Make sure that whatever string is in the buffer, if any, is null terminated
    b.Cursor() = '\0';
}

// ------------------------------------------------------------------------------------------------
void SysEnv::ExpandVars(Buffer & b, CCStr pos, CCStr end)
{
    // Let's have a string to store the extracted variable name and value
    String var;
    // Extract the remaining directories from the specified path
    while (pos != end)
    {
        // Should we start looking for a variable name?
        if (*pos == '$')
        {
            // Clear previous name, if any
            var.clear();
            // Where the name of the variable starts and where it ends
            CCStr start = ++pos, stop = pos;
            // Is this variable name enclosed within curly braces?
            if (*start == '{')
            {
                // Find the closing brace
                stop = strchr(start, '}');
                // Was there a closing brace?
                if (!stop)
                {
                    // Append the rest of the string to the buffer
                    b.AppendS(pos - 1, end - pos + 1);
                    // Stop parsing here
                    break;
                }
                // Is there anything between the brace?
                else if ((stop - start) >= 1)
                {
                    // Slice the variable name
                    var.assign(start + 1, stop - start - 1);
                    // Skip the ending brace
                    ++stop;
                }
            }
            // Is the dollar character followed by a character allowed in variable names?
            else if (isalnum(*start) != 0 || *start == '_')
            {
                // Find the first character that isn't allowed in variable names
                while (stop != end && (isalnum(*stop) != 0 || *stop == '_'))
                {
                    ++stop;
                }
                // Have we found anything?
                if (start != stop)
                {
                    // Slice the variable name
                    var.assign(start, stop - start);
                }
            }
            else
            {
                // Just add the character to the buffer as is
                b.Push('$');
                // Skip to the next character
                continue;
            }
            // Update the position
            pos = stop;
            // Do we have a valid variable name and does it exist?
            if (!var.empty() && Has(var))
            {
                // Append the variable contents to our buffer
                Get(b, var.c_str(), nullptr);
            }
        }
        // Just add the character to the buffer as is
        else
        {
            b.Push(*(pos++));
        }
    }
    // Make sure the string in the buffer is null terminated
    b.Cursor() = '\0';
}

// ------------------------------------------------------------------------------------------------
void SysEnv::ExpandPath(Buffer & b, CCStr pos, CCStr end)
{
    // Does the path even contain something to be expanded?
    if (pos == end || *pos == '\0')
    {
        return; // Nothing to expand!
    }
    // If the path starts with the tilde character then the home directory was requested
    else if (*pos == '~')
    {
        // To be expanded, the tilde character must be followed by a slash
        if (*(++pos) == SQMOD_DIRSEP_CHAR)
        {
            // Let's expand this tilde to the home directory
            HomeDir(b);
            // Let's skip the slash as well
            ++pos;
        }
        // Go back to the previous character and use it literally
        else
        {
            --pos;
        }
    }
    // The remaining string can be expanded normally
    ExpandVars(b, pos, end);
}

// ------------------------------------------------------------------------------------------------
void SysEnv::ExpandVars(Buffer & b, CCStr str)
{
    // Do we have anything to expand?
    if (!str || *str == '\0')
    {
        // Make sure the string in the specified buffer, if any, is null terminated
        if (b)
        {
            b.Cursor() = '\0';
        }
        // Nothing to expand!
        return;
    }
    // Calculate the size of the specified string
    const Uint32 len = strlen(str);
    // Forward the call to the internal function
    ExpandVars(b, str, str + len);
}

// ------------------------------------------------------------------------------------------------
void SysEnv::ExpandVars(Buffer & b, const String & str)
{
    // Do we have anything to expand?
    if (str.empty())
    {
        // Make sure the string in the specified buffer, if any, is null terminated
        if (b)
        {
            b.Cursor() = '\0';
        }
        // Nothing to expand!
        return;
    }
    // Forward the call to the internal function
    ExpandVars(b, str.c_str(), str.c_str() + str.size());
}

// ------------------------------------------------------------------------------------------------
Buffer SysEnv::ExpandVars(CCStr str)
{
    // Do we have anything to expand?
    if (!str || *str == '\0')
    {
        return Buffer(); // Nothing to expand!
    }
    // Calculate the size of the specified string
    const Uint32 len = strlen(str);
    // Allocate a moderately sized buffer
    Buffer b(len + 128);
    // Forward the call to the internal function
    ExpandVars(b, str, str + len);
    // Return ownership of the buffer
    return std::move(b);
}

// ------------------------------------------------------------------------------------------------
Buffer SysEnv::ExpandVars(const String & str)
{
    // Do we have anything to expand?
    if (str.empty())
    {
        return Buffer(); // Nothing to expand!
    }
    // Allocate a moderately sized buffer
    Buffer b(str.size() + 128);
    // Forward the call to the internal function
    ExpandVars(b, str.c_str(), str.c_str() + str.size());
    // Return ownership of the buffer
    return std::move(b);
}

// ------------------------------------------------------------------------------------------------
void SysEnv::ExpandPath(Buffer & b, CCStr path)
{
    // Do we have anything to expand?
    if (!path || *path == '\0')
    {
        // Make sure the string in the specified buffer, if any, is null terminated
        if (b)
        {
            b.Cursor() = '\0';
        }
        // Nothing to expand!
        return;
    }
    // Calculate the size of the specified string
    const Uint32 len = strlen(path);
    // Forward the call to the internal function
    ExpandPath(b, path, path + len);
}

// ------------------------------------------------------------------------------------------------
void SysEnv::ExpandPath(Buffer & b, const String & path)
{
    // Do we have anything to expand?
    if (path.empty())
    {
        // Make sure the string in the specified buffer, if any, is null terminated
        if (b)
        {
            b.Cursor() = '\0';
        }
        // Nothing to expand!
        return;
    }
    // Forward the call to the internal function
    ExpandPath(b, path.c_str(), path.c_str() + path.size());
}

// ------------------------------------------------------------------------------------------------
Buffer SysEnv::ExpandPath(CCStr path)
{
    // Do we have anything to expand?
    if (!path || *path == '\0')
    {
        return Buffer(); // Nothing to expand!
    }
    // Calculate the size of the specified string
    const Uint32 len = strlen(path);
    // Allocate buffer capable of storing a full path
    Buffer b(SQMOD_MAX_PATH);
    // Forward the call to the internal function
    ExpandPath(b, path, path + len);
    // Return ownership of the buffer
    return std::move(b);
}

// ------------------------------------------------------------------------------------------------
Buffer SysEnv::ExpandPath(const String & path)
{
    // Do we have anything to expand?
    if (path.empty())
    {
        return Buffer(); // Nothing to expand!
    }
    // Allocate buffer capable of storing a full path
    Buffer b(SQMOD_MAX_PATH);
    // Forward the call to the internal function
    ExpandPath(b, path.c_str(), path.c_str() + path.size());
    // Return ownership of the buffer
    return std::move(b);
}

// ------------------------------------------------------------------------------------------------
void SysEnv::WorkingDir(Buffer & b)
{
#ifdef SQMOD_OS_WINDOWS
    // Is there a buffer to work with?
    if (!b)
    {
        // Allocate buffer capable of storing a full path
        b = Buffer(SQMOD_MAX_PATH);
    }
    // Retrieve the current directory for the current process
    DWORD len = GetCurrentDirectoryA(b.Remaining(), &b.Cursor());
    // Did we have enough space left in the buffer?
    if (len > b.Remaining())
    {
        // Acquire a new buffer with a more appropriate capacity this time
        b.Grow(len - b.Remaining() + 2);
        // Attempt to retrieve the working directory one more time
        len = GetCurrentDirectoryA(b.Remaining(), &b.Cursor());
        // ^ On failure the null terminator is included in the length
    }
    // Move the edit cursor to the end of the appended data
    b.Advance(len);
#else
    // Do we have enough space to store a full path?
    if (b.Remaining() < SQMOD_MAX_PATH)
    {
        b.Grow(SQMOD_MAX_PATH - b.Remaining() + 2);
    }
    // Attempt to retrieve the current working directory and validate result
    if (getcwd(&b.Cursor(), b.Remaining()))
    {
        // Move the edit cursor to the end of the appended data
        b.Advance(strlen(&b.Cursor()));
    }
#endif // SQMOD_OS_WINDOWS
    // Make sure that the path is properly terminated
    TerminatePath(b);
}

// ------------------------------------------------------------------------------------------------
Buffer SysEnv::WorkingDir()
{
    // Allocate buffer capable of storing a full path
    Buffer b(SQMOD_MAX_PATH);
    // Forward the call to the regular function
    WorkingDir(b);
    // Return ownership of the buffer
    return std::move(b);
}

// ------------------------------------------------------------------------------------------------
void SysEnv::HomeDir(Buffer & b)
{
#ifdef SQMOD_OS_WINDOWS
    // Do we have enough space to store a full path?
    if (b.Remaining() < SQMOD_MAX_PATH)
    {
        b.Grow(SQMOD_MAX_PATH - b.Remaining() + 2);
    }
    // Try the primary method of retrieving the home directory
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PROFILE, nullptr, 0, &b.Cursor())))
    {
        // Move the edit cursor to the end of the appended data
        b.Advance(strlen(&b.Cursor()));
    }
    // Try the secondary method of retrieving the home directory
    else if (Has("USERPROFILE"))
    {
        // Append the contents of the USERPROFILE environment variable
        Get(b, "USERPROFILE", nullptr);
    }
    else if (Has("HOMEDRIVE") && Has("HOMEPATH"))
    {
        // Append the contents of the HOMEDRIVE environment variable
        Get(b, "HOMEDRIVE", nullptr);
        // Append the contents of the HOMEPATH environment variable
        Get(b, "HOMEPATH", nullptr);
    }
#else
    // Try the primary method of retrieving the home directory
    struct passwd * pwd = getpwuid(getuid());
    // Validate the success of the previous operation
    if (pwd)
    {
        // Append the path to our buffer
        b.AppendS(pwd->pw_dir);
    }
    else
    {
        // Try the secondary method of retrieving the home directory
        pwd = getpwuid(geteuid());
        // Validate the success of the previous operation
        if (pwd)
        {
            // Write the path to our buffer and store the size
            b.AppendS(pwd->pw_dir);
        }
        // Fall back to the system environment variables
        else if (Has("HOME");)
        {
            // Append the contents of the HOME environment variable
            Get(b, "HOME", nullptr);
        }
    }
#endif // SQMOD_OS_WINDOWS
    // Make sure that the path is properly terminated
    TerminatePath(b);
}

// ------------------------------------------------------------------------------------------------
Buffer SysEnv::HomeDir()
{
    // Allocate buffer capable of storing a full path
    Buffer b(SQMOD_MAX_PATH);
    // Forward the call to the regular function
    HomeDir(b);
    // Return ownership of the buffer
    return std::move(b);
}

// ------------------------------------------------------------------------------------------------
void SysEnv::ConfigHomeDir(Buffer & b)
{
#ifdef SQMOD_OS_WINDOWS
    // Is there a buffer to work with?
    if (!b)
    {
        // Allocate buffer capable of storing a full path
        b = Buffer(SQMOD_MAX_PATH);
    }
    // Does the APPDATA environment variable exist?
    if (Has("APPDATA"))
    {
        // Obtain the contents of the APPDATA environment variable
        Get(b, "APPDATA", nullptr);
    }
    else
    {
        // Default to the home directory
        HomeDir(b);
    }
#else
    // Obtain the home directory path (should contain a trailing slash)
    HomeDir(b);
    // Use the home directory and append the ".config" sub folder
    b.AppendS(".config");
#endif // SQMOD_OS_WINDOWS
    // Make sure that the path is properly terminated
    TerminatePath(b);
}

// ------------------------------------------------------------------------------------------------
Buffer SysEnv::ConfigHomeDir()
{
    // Allocate buffer capable of storing a full path
    Buffer b(SQMOD_MAX_PATH);
    // Forward the call to the regular function
    ConfigHomeDir(b);
    // Return ownership of the buffer
    return std::move(b);
}

// ------------------------------------------------------------------------------------------------
void SysEnv::DataHomeDir(Buffer & b)
{
#ifdef SQMOD_OS_WINDOWS
    // Is there a buffer to work with?
    if (!b)
    {
        // Allocate buffer capable of storing a full path
        b = Buffer(SQMOD_MAX_PATH);
    }
    // Does the LOCALAPPDATA environment variable exist?
    if (Has("LOCALAPPDATA"))
    {
        // Obtain the contents of the LOCALAPPDATA environment variable
        return Get(b, "LOCALAPPDATA", nullptr);
    }
    // Default to the home config directory
    return ConfigHomeDir(b);
#else
    // Obtain the home directory path (should contain a trailing slash)
    HomeDir(b);
    // Use the home directory and append the ".local/share" sub folder
    b.AppendS(".config/share");
#endif // SQMOD_OS_WINDOWS
    // Make sure that the path is properly terminated
    TerminatePath(b);
}

// ------------------------------------------------------------------------------------------------
Buffer SysEnv::DataHomeDir()
{
    // Allocate buffer capable of storing a full path
    Buffer b(SQMOD_MAX_PATH);
    // Forward the call to the regular function
    DataHomeDir(b);
    // Return ownership of the buffer
    return std::move(b);
}

// ------------------------------------------------------------------------------------------------
void SysEnv::TempHomeDir(Buffer & b)
{
#ifdef SQMOD_OS_WINDOWS
    // Use the regular temp directory
    TempDir(b);
#else
    // Obtain the home directory path (should contain a trailing slash)
    HomeDir(b);
    // Use the home directory and append the ".local/tmp" folder
    b.AppendS(".local/tmp");
    // Make sure that the path is properly terminated
    TerminatePath(b);
#endif // SQMOD_OS_WINDOWS
}

// ------------------------------------------------------------------------------------------------
Buffer SysEnv::TempHomeDir()
{
    // Allocate buffer capable of storing a full path
    Buffer b(SQMOD_MAX_PATH);
    // Forward the call to the regular function
    TempHomeDir(b);
    // Return ownership of the buffer
    return std::move(b);
}

// ------------------------------------------------------------------------------------------------
void SysEnv::CacheHomeDir(Buffer & b)
{
#ifdef SQMOD_OS_WINDOWS
    // Use the regular temp directory
    TempDir(b);
#else
    // Obtain the home directory path (should contain a trailing slash)
    HomeDir(b);
    // Use the home directory and append the ".cache" folder
    b.AppendS(".cache");
    // Make sure that the path is properly terminated
    TerminatePath(b);
#endif // SQMOD_OS_WINDOWS
}

// ------------------------------------------------------------------------------------------------
Buffer SysEnv::CacheHomeDir()
{
    // Allocate buffer capable of storing a full path
    Buffer b(SQMOD_MAX_PATH);
    // Forward the call to the regular function
    CacheHomeDir(b);
    // Return ownership of the buffer
    return std::move(b);
}

// ------------------------------------------------------------------------------------------------
void SysEnv::TempDir(Buffer & b)
{
#ifdef SQMOD_OS_WINDOWS
    // Is there a buffer to work with?
    if (!b)
    {
        // Allocate buffer capable of storing a full path
        b = Buffer(SQMOD_MAX_PATH);
    }
    // Retrieve the path of the directory designated for temporary files
    DWORD len = GetTempPathA(b.Remaining(), &b.Cursor());
    // Did we failed to retrieve the path?
    if (len == 0)
    {
        return; // Unable to retrieve the path!
    }
    // Did we have enough space left in the buffer?
    else if (len > b.Remaining())
    {
        // Acquire a new buffer with a more appropriate capacity this time
        b.Grow(len - b.Remaining() + 2);
        // Attempt to retrieve the temporary directory one more time
        len = GetTempPathA(b.Remaining(), &b.Cursor());
        // ^ On failure the null terminator is included in the length
    }
    // Convert the acquired path to its long form
    len = GetLongPathNameA(&b.Cursor(), &b.Cursor(), b.Remaining());
    // Did we failed to convert the path?
    if (len == 0)
    {
        return; // Unable to convert the path!
    }
    // Did we have enough space left in the buffer?
    else if (len > b.Remaining())
    {
        // Acquire a new buffer with a more appropriate capacity this time
        b.Grow(len - b.Remaining() + 2);
        // Attempt to retrieve the temporary directory again because we reused the buffer
        GetTempPathA(b.Remaining(), &b.Cursor());
        // Attempt to convert the acquired path to its long form one more time
        len = GetLongPathNameA(&b.Cursor(), &b.Cursor(), b.Remaining());
        // ^ On failure the null terminator is included in the length
    }
    // Move the edit cursor to the end of the appended data
    b.Advance(len);
#else
    // Does the TMPDIR environment variable exist?
    if (SysEnv::Has("TMPDIR"))
    {
        // Obtain the contents of the TMPDIR environment variable
        Get(b, "TMPDIR", nullptr);
    }
    else
    {
        // Default to the "/tmp" directory
        b.AppendS("/tmp/");
    }
#endif // SQMOD_OS_WINDOWS
    // Make sure that the path is properly terminated
    TerminatePath(b);
}

// ------------------------------------------------------------------------------------------------
Buffer SysEnv::TempDir()
{
    // Allocate buffer capable of storing a full path
    Buffer b(SQMOD_MAX_PATH);
    // Forward the call to the regular function
    TempDir(b);
    // Return ownership of the buffer
    return std::move(b);
}

// ------------------------------------------------------------------------------------------------
void SysEnv::ConfigDir(Buffer & b)
{
#ifdef SQMOD_OS_WINDOWS
    // Is there a buffer to work with?
    if (!b)
    {
        // Allocate buffer capable of storing a full path
        b = Buffer(SQMOD_MAX_PATH);
    }
    // Does the PROGRAMDATA environment variable exist?
    if (Has("PROGRAMDATA"))
    {
        // Obtain the contents of the PROGRAMDATA environment variable
        Get(b, "PROGRAMDATA", nullptr);
    }
    else
    {
        // Make sure that whatever string is in the buffer, if any, is null terminated
        b.Cursor() = '\0';
        // Unable to retrieve the path!
        return;
    }
#else
    // Default to "/etc" directory
    b.WriteS("/etc/");
#endif // SQMOD_OS_WINDOWS
    // Make sure that the path is properly terminated
    TerminatePath(b);
}

// ------------------------------------------------------------------------------------------------
Buffer SysEnv::ConfigDir()
{
    // Allocate buffer capable of storing a full path
    Buffer b(SQMOD_MAX_PATH);
    // Forward the call to the regular function
    ConfigDir(b);
    // Return ownership of the buffer
    return std::move(b);
}

// ------------------------------------------------------------------------------------------------
void SysEnv::SystemDir(Buffer & b)
{
#ifdef SQMOD_OS_WINDOWS
    // Is there a buffer to work with?
    if (!b)
    {
        // Allocate buffer capable of storing a full path
        b = Buffer(SQMOD_MAX_PATH);
    }
    // Retrieve the path of the system directory
    DWORD len = GetSystemDirectoryA(&b.Cursor(), b.Remaining());
    // Did we failed to retrieve the path?
    if (len == 0)
    {
        return; // Unable to retrieve the path!
    }
    // Did we have enough space left in the buffer?
    else if (len > b.Remaining())
    {
        // Acquire a new buffer with a more appropriate capacity this time
        b.Grow(len - b.Remaining() + 2);
        // Attempt to retrieve the path of the system directory one more time
        len = GetSystemDirectoryA(&b.Cursor(), b.Remaining());
        // ^ On failure the null terminator is included in the length
    }
    // Move the edit cursor to the end of the appended data
    b.Advance(len);
#else
    // Use a dummy directory for now
    b.WriteS("/sys/");
#endif // SQMOD_OS_WINDOWS
    // Make sure that the path is properly terminated
    TerminatePath(b);
}

// ------------------------------------------------------------------------------------------------
Buffer SysEnv::SystemDir()
{
    // Allocate buffer capable of storing a full path
    Buffer b(SQMOD_MAX_PATH);
    // Forward the call to the regular function
    SystemDir(b);
    // Return ownership of the buffer
    return std::move(b);
}

// ------------------------------------------------------------------------------------------------
void SysEnv::NullDir(Buffer & b)
{
#ifdef SQMOD_OS_WINDOWS
    b.AppendS("NUL:");
#else
    b.AppendS("/dev/null/");
#endif // SQMOD_OS_WINDOWS
    // Make sure that whatever string is in the buffer, if any, is null terminated
    b.Cursor() = '\0';
}

// ------------------------------------------------------------------------------------------------
Buffer SysEnv::NullDir()
{
    // Allocate buffer capable of storing a full path
    Buffer b(SQMOD_MAX_PATH);
    // Append the null path
#ifdef SQMOD_OS_WINDOWS
    b.AppendS("NUL:");
#else
    b.AppendS("/dev/null/");
#endif // SQMOD_OS_WINDOWS
    // Make sure that whatever string is in the buffer, if any, is null terminated
    b.Cursor() = '\0';
    // Return ownership of the buffer
    return std::move(b);
}

// ------------------------------------------------------------------------------------------------
static Object BufferToObj(const Buffer & b)
{
    // Obtain the initial stack size
    const StackGuard sg(DefaultVM::Get());
    // Push the string onto the stack
    sq_pushstring(DefaultVM::Get(), b.Data(), b.Position());
    // Obtain the object from the stack and return it
    return Var< Object >(DefaultVM::Get(), -1).value;
}

// ------------------------------------------------------------------------------------------------
static bool SqEnv_Has(CCStr name)
{
    return SysEnv::Has(name);
}

// ------------------------------------------------------------------------------------------------
static Object SqEnv_Get(CCStr name)
{
    return BufferToObj(SysEnv::Get(name, nullptr));
}

// ------------------------------------------------------------------------------------------------
static Object SqEnv_GetOr(CCStr name, CCStr fallback)
{
    return BufferToObj(SysEnv::Get(name, fallback));
}

// ------------------------------------------------------------------------------------------------
static void SqEnv_Set(CCStr name, CCStr value)
{
    SysEnv::Set(name, value);
}

// ------------------------------------------------------------------------------------------------
static Object SqEnv_ExpandVars(CCStr str)
{
    return BufferToObj(SysEnv::ExpandVars(str));
}

// ------------------------------------------------------------------------------------------------
static Object SqEnv_ExpandPath(CCStr path)
{
    return BufferToObj(SysEnv::ExpandPath(path));
}

// ------------------------------------------------------------------------------------------------
static Object SqEnv_WorkingDir()
{
    return BufferToObj(SysEnv::WorkingDir());
}

// ------------------------------------------------------------------------------------------------
static Object SqEnv_HomeDir()
{
    return BufferToObj(SysEnv::HomeDir());
}

// ------------------------------------------------------------------------------------------------
static Object SqEnv_ConfigHomeDir()
{
    return BufferToObj(SysEnv::ConfigHomeDir());
}

// ------------------------------------------------------------------------------------------------
static Object SqEnv_DataHomeDir()
{
    return BufferToObj(SysEnv::DataHomeDir());
}

// ------------------------------------------------------------------------------------------------
static Object SqEnv_TempHomeDir()
{
    return BufferToObj(SysEnv::TempHomeDir());
}

// ------------------------------------------------------------------------------------------------
static Object SqEnv_CacheHomeDir()
{
    return BufferToObj(SysEnv::CacheHomeDir());
}

// ------------------------------------------------------------------------------------------------
static Object SqEnv_TempDir()
{
    return BufferToObj(SysEnv::TempDir());
}

// ------------------------------------------------------------------------------------------------
static Object SqEnv_ConfigDir()
{
    return BufferToObj(SysEnv::ConfigDir());
}

// ------------------------------------------------------------------------------------------------
static Object SqEnv_SystemDir()
{
    return BufferToObj(SysEnv::SystemDir());
}

// ------------------------------------------------------------------------------------------------
static Object SqEnv_NullDir()
{
    return BufferToObj(SysEnv::NullDir());
}

// ================================================================================================
void Register_SysEnv(HSQUIRRELVM vm)
{
    Table sens(vm);

    sens.Func(_SC("Has"), &SqEnv_Has);
    sens.Func(_SC("Get"), &SqEnv_Get);
    sens.Func(_SC("GetOr"), &SqEnv_GetOr);
    sens.Func(_SC("Set"), &SqEnv_Set);
    sens.Func(_SC("OSName"), &SysEnv::OSName);
    sens.Func(_SC("OSDisplayName"), &SysEnv::OSDisplayName);
    sens.Func(_SC("OSVersion"), &SysEnv::OSVersion);
    sens.Func(_SC("OSArchitecture"), &SysEnv::OSArchitecture);
    sens.Func(_SC("NodeName"), &SysEnv::NodeName);
    sens.Func(_SC("ProcessorCount"), &SysEnv::ProcessorCount);
    sens.Func(_SC("ExpandVars"), &SqEnv_ExpandVars);
    sens.Func(_SC("ExpandPath"), &SqEnv_ExpandPath);
    sens.Func(_SC("WorkingDir"), &SqEnv_WorkingDir);
    sens.Func(_SC("HomeDir"), &SqEnv_HomeDir);
    sens.Func(_SC("ConfigHomeDir"), &SqEnv_ConfigHomeDir);
    sens.Func(_SC("DataHomeDir"), &SqEnv_DataHomeDir);
    sens.Func(_SC("TempHomeDir"), &SqEnv_TempHomeDir);
    sens.Func(_SC("CacheHomeDir"), &SqEnv_CacheHomeDir);
    sens.Func(_SC("TempDir"), &SqEnv_TempDir);
    sens.Func(_SC("ConfigDir"), &SqEnv_ConfigDir);
    sens.Func(_SC("SystemDir"), &SqEnv_SystemDir);
    sens.Func(_SC("NullDir"), &SqEnv_NullDir);

    RootTable(vm).Bind(_SC("SqSysEnv"), sens);
}

} // Namespace:: SqMod
