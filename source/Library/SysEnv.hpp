#ifndef _LIBRARY_SYSENV_HPP_
#define _LIBRARY_SYSENV_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"
#include "Base/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * This class provides access to environment variables and some general system information.
*/
struct SysEnv
{
    /* --------------------------------------------------------------------------------------------
     * Default constructor. (disabled)
    */
    SysEnv() = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    SysEnv(const SysEnv &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    SysEnv(SysEnv &&) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor. (disabled)
    */
    ~SysEnv() = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    SysEnv & operator = (const SysEnv &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    SysEnv & operator = (SysEnv &&) = delete;

public:

    /* --------------------------------------------------------------------------------------------
     * Returns true if an environment variable with the given name is defined.
    */
    static bool Has(CCStr name);

    /* --------------------------------------------------------------------------------------------
     * Returns true if an environment variable with the given name is defined.
    */
    static bool Has(const String & name);

    /* --------------------------------------------------------------------------------------------
     * Returns the value of the environment variable with the given name.
     * If the environment variable is undefined, returns fallback value instead.
    */
    static void Get(Buffer & b, CCStr name, CCStr fallback);

    /* --------------------------------------------------------------------------------------------
     * Returns the value of the environment variable with the given name.
    */
    static Buffer Get(CCStr name)
    {
        return Get(name, nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the value of the environment variable with the given name.
    */
    static Buffer Get(const String & name)
    {
        return Get(name.c_str(), nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the value of the environment variable with the given name.
     * If the environment variable is undefined, returns fallback value instead.
    */
    static Buffer Get(CCStr name, CCStr fallback);

    /* --------------------------------------------------------------------------------------------
     * Returns the value of the environment variable with the given name.
     * If the environment variable is undefined, returns fallback value instead.
    */
    static Buffer Get(CCStr name, const String & fallback)
    {
        return Get(name, fallback.c_str());
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the value of the environment variable with the given name.
     * If the environment variable is undefined, returns fallback value instead.
    */
    static Buffer Get(const String & name, CCStr fallback)
    {
        return Get(name.c_str(), fallback);
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the value of the environment variable with the given name.
     * If the environment variable is undefined, returns fallback value instead.
    */
    static Buffer Get(const String & name, const String & fallback)
    {
        return Get(name.c_str(), fallback.c_str());
    }

    /* --------------------------------------------------------------------------------------------
     * Sets the environment variable with the given name to the given value.
    */
    static bool Set(CCStr name, CCStr value);

    /* --------------------------------------------------------------------------------------------
     * Sets the environment variable with the given name to the given value.
    */
    static bool Set(const String & name, const String & value);

    /* --------------------------------------------------------------------------------------------
     * Returns the operating system name.
    */
    static String OSName();

    /* --------------------------------------------------------------------------------------------
     * Returns the operating system name in a more "user-friendly" way. This only affects Windows.
    */
    static String OSDisplayName();

    /* --------------------------------------------------------------------------------------------
     * Returns the operating system version.
    */
    static String OSVersion();

    /* --------------------------------------------------------------------------------------------
     * Returns the operating system architecture.
    */
    static String OSArchitecture();

    /* --------------------------------------------------------------------------------------------
     * Returns the node (or host) name.
    */
    static String NodeName();

    /* --------------------------------------------------------------------------------------------
     * Returns the number of processors installed in the system. If the number of processors
     * cannot be determined, returns 1.
    */
    static Uint32 ProcessorCount();

protected:

    /* --------------------------------------------------------------------------------------------
     * Make sure that the path in the specified buffer contains a trailing slash.
    */
    static void TerminatePath(Buffer & b);

    /* --------------------------------------------------------------------------------------------
     * Expands all environment variables contained in the string.
    */
    static void ExpandVars(Buffer & b, CCStr pos, CCStr end);

    /* --------------------------------------------------------------------------------------------
     * Expands all environment variables contained in the path. Uses the Unix variable style.
    */
    static void ExpandPath(Buffer & b, CCStr pos, CCStr end);

public:

    /* --------------------------------------------------------------------------------------------
     * Expands all environment variables contained in the string.
    */
    static void ExpandVars(Buffer & b, CCStr str);

    /* --------------------------------------------------------------------------------------------
     * Expands all environment variables contained in the string.
    */
    static void ExpandVars(Buffer & b, const String & str);

    /* --------------------------------------------------------------------------------------------
     * Expands all environment variables contained in the string.
    */
    static Buffer ExpandVars(CCStr str);

    /* --------------------------------------------------------------------------------------------
     * Expands all environment variables contained in the string.
    */
    static Buffer ExpandVars(const String & str);

    /* --------------------------------------------------------------------------------------------
     * Expands all environment variables contained in the path. Uses the Unix variable style.
    */
    static void ExpandPath(Buffer & b, CCStr path);

    /* --------------------------------------------------------------------------------------------
     * Expands all environment variables contained in the path. Uses the Unix variable style.
    */
    static void ExpandPath(Buffer & b, const String & path);

    /* --------------------------------------------------------------------------------------------
     * Expands all environment variables contained in the path. Uses the Unix variable style.
    */
    static Buffer ExpandPath(CCStr path);

    /* --------------------------------------------------------------------------------------------
     * Expands all environment variables contained in the path. Uses the Unix variable style.
    */
    static Buffer ExpandPath(const String & path);

    /* --------------------------------------------------------------------------------------------
     * Obtain the current working directory within the specified buffer.
    */
    static void WorkingDir(Buffer & b);

    /* --------------------------------------------------------------------------------------------
     * Obtain the current working directory within a buffer and return it.
    */
    static Buffer WorkingDir();

    /* --------------------------------------------------------------------------------------------
     * Obtain the user's home directory within the specified buffer.
    */
    static void HomeDir(Buffer & b);

    /* --------------------------------------------------------------------------------------------
     * Obtain the user's home directory within a buffer and return it.
    */
    static Buffer HomeDir();

    /* --------------------------------------------------------------------------------------------
     * Obtain the user's config directory within the specified buffer.
    */
    static void ConfigHomeDir(Buffer & b);

    /* --------------------------------------------------------------------------------------------
     * Obtain the user's config directory within a buffer and return it.
    */
    static Buffer ConfigHomeDir();

    /* --------------------------------------------------------------------------------------------
     * Obtain the user's data directory within the specified buffer.
    */
    static void DataHomeDir(Buffer & b);

    /* --------------------------------------------------------------------------------------------
     * Obtain the user's data directory within a buffer and return it.
    */
    static Buffer DataHomeDir();

    /* --------------------------------------------------------------------------------------------
     * Obtain the user's temporary directory within the specified buffer.
    */
    static void TempHomeDir(Buffer & b);

    /* --------------------------------------------------------------------------------------------
     * Obtain the user's temporary directory within a buffer and return it.
    */
    static Buffer TempHomeDir();

    /* --------------------------------------------------------------------------------------------
     * Obtain the user's cache directory within the specified buffer.
    */
    static void CacheHomeDir(Buffer & b);

    /* --------------------------------------------------------------------------------------------
     * Obtain the user's cache directory within a buffer and return it.
    */
    static Buffer CacheHomeDir();

    /* --------------------------------------------------------------------------------------------
     * Obtain the temporary directory within the specified buffer.
    */
    static void TempDir(Buffer & b);

    /* --------------------------------------------------------------------------------------------
     * Obtain the temporary directory within a buffer and return it.
    */
    static Buffer TempDir();

    /* --------------------------------------------------------------------------------------------
     * Obtain the systemwide config directory within the specified buffer.
    */
    static void ConfigDir(Buffer & b);

    /* --------------------------------------------------------------------------------------------
     * Obtain the systemwide config directory within a buffer and return it.
    */
    static Buffer ConfigDir();

    /* --------------------------------------------------------------------------------------------
     * Obtain the system directory within the specified buffer.
    */
    static void SystemDir(Buffer & b);

    /* --------------------------------------------------------------------------------------------
     * Obtain the system directory within a buffer and return it.
    */
    static Buffer SystemDir();

    /* --------------------------------------------------------------------------------------------
     * Obtain the null directory within the specified buffer.
    */
    static void NullDir(Buffer & b);

    /* --------------------------------------------------------------------------------------------
     * Obtain the null directory within a buffer and return it.
    */
    static Buffer NullDir();
};

} // Namespace:: SqMod

#endif // _LIBRARY_SYSENV_HPP_
