#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Common.hpp"

// ------------------------------------------------------------------------------------------------
#include <vector>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Retrieve the full path of file.
*/
Buffer GetRealFilePath(const SQChar * path);

/* ------------------------------------------------------------------------------------------------
 * This class represents filesystem paths in a platform-independent manner.
*/
class SysPath
{
public:

    // --------------------------------------------------------------------------------------------
    typedef std::vector< String > StrVec; // Directory list.

    /* --------------------------------------------------------------------------------------------
     * Styles of directories to expect when parsing or to export.
    */
    enum struct Style
    {
        Unix = 0,
        Windows,
        Native,
        Guess,
        Dynamic
    };

    /* --------------------------------------------------------------------------------------------
     * Creates an empty relative path.
    */
    SysPath();

    /* --------------------------------------------------------------------------------------------
     * Creates an empty absolute or relative path.
    */
    explicit SysPath(bool absolute);

    /* --------------------------------------------------------------------------------------------
     * Creates a path in native format from a string.
    */
    explicit SysPath(const SQChar * path);

    /* --------------------------------------------------------------------------------------------
     * Creates a path from a string.
    */
    SysPath(const SQChar * path, int32_t style);

    /* --------------------------------------------------------------------------------------------
     * Creates a path from a string.
    */
    SysPath(const SQChar * path, Style style);

    /* --------------------------------------------------------------------------------------------
     * Creates a path in native format from a string.
    */
    explicit SysPath(const Buffer & path, int32_t size = -1);

    /* --------------------------------------------------------------------------------------------
     * Creates a path from a string.
    */
    SysPath(const Buffer & path, Style style, int32_t size = -1);

    /* --------------------------------------------------------------------------------------------
     * Creates a path in native format from a string.
    */
    explicit SysPath(const String & path);

    /* --------------------------------------------------------------------------------------------
     * Creates a path from a string.
    */
    SysPath(const String & path, Style style);

    /* --------------------------------------------------------------------------------------------
     * Creates a path from a parent path and a file name. The parent path is expected to reference
     * a directory.
    */
    SysPath(const SysPath & parent, const SQChar *  name);

    /* --------------------------------------------------------------------------------------------
     * Creates a path from a parent path and a file name. The parent path is expected to reference
     * a directory.
    */
    SysPath(const SysPath & parent, const String &  name);

    /* --------------------------------------------------------------------------------------------
     * Creates a path from a parent path and a relative path. The parent path is expected
     * to reference a directory. The relative path is appended to the parent path.
    */
    SysPath(const SysPath & parent, const SysPath &  relative);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    SysPath(const SysPath & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    SysPath(SysPath && o) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~SysPath() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    SysPath & operator = (const SysPath & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    SysPath & operator = (SysPath && o) noexcept = default;

    /* --------------------------------------------------------------------------------------------
     * Assigns a string containing a path in native format.
    */
    SysPath & operator = (const SQChar * path);

    /* --------------------------------------------------------------------------------------------
     * Assigns a string containing a path in native format.
    */
    SysPath & operator = (const String & path);

    /* --------------------------------------------------------------------------------------------
     * Equality comparison.
    */
    bool operator == (const SysPath & o) const;

    /* --------------------------------------------------------------------------------------------
     * Inequality comparison.
    */
    bool operator != (const SysPath & o) const;

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean operator.
    */
    operator bool () const; // NOLINT(google-explicit-constructor)

    /* --------------------------------------------------------------------------------------------
     * Returns the n'th directory in the directory list. If n == depth(), returns the file name.
    */
    const String & operator [] (uint32_t n) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    SQMOD_NODISCARD int32_t Cmp(const SysPath & o) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    SQMOD_NODISCARD Object ToString() const;

    /* --------------------------------------------------------------------------------------------
     * Swaps the path with another one.
    */
    void Swap(SysPath & path);

    /* --------------------------------------------------------------------------------------------
     * Clears all components.
    */
    void Clear();

    /* --------------------------------------------------------------------------------------------
     * Assigns a string containing a path in native format.
    */
    SysPath & Assign(const SQChar * path);

    /* --------------------------------------------------------------------------------------------
     * Assigns a string containing a path.
    */
    SysPath & Assign(const SQChar * path, int32_t style);

    /* --------------------------------------------------------------------------------------------
     * Assigns a string containing a path.
    */
    SysPath & Assign(const SQChar * path, Style style);

    /* --------------------------------------------------------------------------------------------
     * Assigns a string containing a path in native format.
    */
    SysPath & Assign(const Buffer & path, int32_t size = -1);

    /* --------------------------------------------------------------------------------------------
     * Assigns a string containing a path.
    */
    SysPath & Assign(const Buffer & path, Style style, int32_t size = -1);

    /* --------------------------------------------------------------------------------------------
     * Assigns a string containing a path in native format.
    */
    SysPath & Assign(const String & path);

    /* --------------------------------------------------------------------------------------------
     * Assigns a string containing a path.
    */
    SysPath & Assign(const String & path, Style style);

    /* --------------------------------------------------------------------------------------------
     * Creates a path from a parent path and a file name. The parent path is expected to reference
     * a directory.
    */
    SysPath & Assign(const SysPath & parent, const SQChar *  name);

    /* --------------------------------------------------------------------------------------------
     * Creates a path from a parent path and a file name. The parent path is expected to reference
     * a directory.
    */
    SysPath & Assign(const SysPath & parent, const String &  name);

    /* --------------------------------------------------------------------------------------------
     * Creates a path from a parent path and a relative path. The parent path is expected
     * to reference a directory. The relative path is appended to the parent path.
    */
    SysPath & Assign(const SysPath & parent, const SysPath &  relative);

    /* --------------------------------------------------------------------------------------------
     * Copy the components from another path.
    */
    SysPath & Assign(const SysPath & path);

    /* --------------------------------------------------------------------------------------------
     * Move the components of another path into this instance.
    */
    SysPath & Assign(SysPath && path);

    /* --------------------------------------------------------------------------------------------
     * The resulting path always refers to a directory and the filename part is empty.
    */
    SysPath & AssignDir(const SQChar * path);

    /* --------------------------------------------------------------------------------------------
     * The resulting path always refers to a directory and the filename part is empty.
    */
    SysPath & AssignDir(const SQChar * path, int32_t style);

    /* --------------------------------------------------------------------------------------------
     * The resulting path always refers to a directory and the filename part is empty.
    */
    SysPath & AssignDir(const SQChar * path, Style style);

    /* --------------------------------------------------------------------------------------------
     * The resulting path always refers to a directory and the filename part is empty.
    */
    SysPath & AssignDir(const Buffer & path, int32_t size = -1);

    /* --------------------------------------------------------------------------------------------
     * The resulting path always refers to a directory and the filename part is empty.
    */
    SysPath & AssignDir(const Buffer & path, Style style, int32_t size = -1);

    /* --------------------------------------------------------------------------------------------
     * The resulting path always refers to a directory and the filename part is empty.
    */
    SysPath & AssignDir(const String & path);

    /* --------------------------------------------------------------------------------------------
     * The resulting path always refers to a directory and the filename part is empty.
    */
    SysPath & AssignDir(const String & path, Style style);

    /* --------------------------------------------------------------------------------------------
     * Returns a string containing the path in native format.
    */
    SQMOD_NODISCARD Buffer ToBuffer() const;

    /* --------------------------------------------------------------------------------------------
     * Returns a string containing the path in the given format.
    */
    SQMOD_NODISCARD Buffer ToBuffer(Style style) const;

    /* --------------------------------------------------------------------------------------------
     * Returns a string containing the path in the given format.
    */
    SQMOD_NODISCARD Object ToStr(int32_t style) const;

    /* --------------------------------------------------------------------------------------------
     * Assigns a string containing a path.
    */
    void FromString(const SQChar * path);

    /* --------------------------------------------------------------------------------------------
     * See whether the path is absolute.
    */
    SQMOD_NODISCARD bool IsAbsolute() const
    {
        return m_Absolute;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the path is relative.
    */
    SQMOD_NODISCARD bool IsRelative() const
    {
        return !m_Absolute;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the path references a directory.
    */
    SQMOD_NODISCARD bool IsDirectory() const
    {
        return m_Name.empty();
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the path references a file.
    */
    SQMOD_NODISCARD bool IsFile() const
    {
        return !m_Name.empty();
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the path Does not contain a drive, directories or file name.
    */
    SQMOD_NODISCARD bool Empty() const
    {
        return (m_Dirs.empty() && m_Name.empty() && m_Drive == 0);
    }

    /* --------------------------------------------------------------------------------------------
     * If the path contains a file name, the file name is appended to the directory list and cleared.
    */
    SysPath & MakeDirectory();

    /* --------------------------------------------------------------------------------------------
     * If the path contains no file name, the last directory becomes the file name.
    */
    SysPath & MakeFile();

    /* --------------------------------------------------------------------------------------------
     * Makes the path refer to its parent.
    */
    SysPath & MakeParent();

    /* --------------------------------------------------------------------------------------------
     * Makes the path absolute if it is relative. The current working directory is taken
     * as base directory.
    */
    SysPath & MakeAbsolute();

    /* --------------------------------------------------------------------------------------------
     * Makes the path absolute if it is relative. The given path is taken as base.
    */
    SysPath & MakeAbsolute(const SysPath & base);

    /* --------------------------------------------------------------------------------------------
     * Makes the path absolute if it is relative. The given path is taken as base.
    */
    SysPath & MakeAbsolute(SysPath && base);

    /* --------------------------------------------------------------------------------------------
     * Appends the given path.
    */
    SysPath & Append(const SysPath & path);

    /* --------------------------------------------------------------------------------------------
     * Appends the given path.
    */
    SysPath & Append(SysPath && path);

    /* --------------------------------------------------------------------------------------------
     * Parse the given string and append the resulted path.
    */
    SysPath & Append(const SQChar * path);

    /* --------------------------------------------------------------------------------------------
     * Parse the given string and append the resulted path.
    */
    SysPath & Append(const SQChar * path, int32_t style);

    /* --------------------------------------------------------------------------------------------
     * Parse the given string and append the resulted path.
    */
    SysPath & Append(const SQChar * path, Style style);

    /* --------------------------------------------------------------------------------------------
     * Parse the given string and append the resulted path.
    */
    SysPath & Append(const Buffer & path, int32_t size = -1);

    /* --------------------------------------------------------------------------------------------
     * Parse the given string and append the resulted path.
    */
    SysPath & Append(const Buffer & path, Style style, int32_t size = -1);

    /* --------------------------------------------------------------------------------------------
     * Parse the given string and append the resulted path.
    */
    SysPath & Append(const String & path);

    /* --------------------------------------------------------------------------------------------
     * Parse the given string and append the resulted path.
    */
    SysPath & Append(const String & path, Style style);

    /* --------------------------------------------------------------------------------------------
     * Returns the drive letter.
    */
    SQMOD_NODISCARD CharT GetDrive() const
    {
        return m_Drive;
    }

    /* --------------------------------------------------------------------------------------------
     * Modifies the drive letter.
    */
    void SetDrive(CharT drive)
    {
        m_Drive = drive;
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the number of directories in the directory list.
    */
    SQMOD_NODISCARD uint32_t Depth() const
    {
        return static_cast< uint32_t >(m_Dirs.size());
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the n'th directory in the directory list. If n == depth(), returns the file name.
    */
    SQMOD_NODISCARD const String & Directory(uint32_t n) const;

    /* --------------------------------------------------------------------------------------------
     * Adds a directory to the directory list.
    */
    SysPath & Push(const SQChar * dir);

    /* --------------------------------------------------------------------------------------------
     * Adds a directory to the directory list.
    */
    SysPath & Push(const String & dir);

    /* --------------------------------------------------------------------------------------------
     * Adds a directory to the directory list.
    */
    SysPath & Push(String && dir);

    /* --------------------------------------------------------------------------------------------
     * Removes the last directory from the directory list.
    */
    SysPath & PopBack();

    /* --------------------------------------------------------------------------------------------
     * Removes the first directory from the directory list.
    */
    SysPath & PopFront();

    /* --------------------------------------------------------------------------------------------
     * Set the specified file name.
    */
    SysPath & SetFilename(const SQChar * name);

    /* --------------------------------------------------------------------------------------------
     * Set the specified file name.
    */
    SysPath & SetFilename(const String & name);

    /* --------------------------------------------------------------------------------------------
     * Set the specified file name.
    */
    SysPath & SetFilename(String && name);

    /* --------------------------------------------------------------------------------------------
     * Retrieves the file name.
    */
    SQMOD_NODISCARD const String & GetFilename() const
    {
        return m_Name;
    }

    /* --------------------------------------------------------------------------------------------
     * Set the specified file name.
    */
    void SqSetFilename(const SQChar * name)
    {
        SetFilename(name);
    }

    /* --------------------------------------------------------------------------------------------
     * Sets the basename part of the file name and does not change the extension.
    */
    SysPath & SetBasename(const SQChar * name);

    /* --------------------------------------------------------------------------------------------
     * Sets the basename part of the file name and does not change the extension.
    */
    SysPath & SetBasename(const String & name);

    /* --------------------------------------------------------------------------------------------
     * Sets the basename part of the file name and does not change the extension.
    */
    SysPath & SetBasename(String && name);

    /* --------------------------------------------------------------------------------------------
     * Returns the basename (the file name without extension) of the path.
    */
    SQMOD_NODISCARD String GetBasename() const;

    /* --------------------------------------------------------------------------------------------
     * Sets the basename part of the file name and does not change the extension.
    */
    void SqSetBasename(const SQChar * name)
    {
        SetBasename(name);
    }

    /* --------------------------------------------------------------------------------------------
     * Sets the file name extension.
    */
    SysPath & SetExtension(const SQChar * ext);

    /* --------------------------------------------------------------------------------------------
     * Sets the file name extension.
    */
    SysPath & SetExtension(const String & ext);

    /* --------------------------------------------------------------------------------------------
     * Returns the file name extension.
    */
    SQMOD_NODISCARD String GetExtension() const;

    /* --------------------------------------------------------------------------------------------
     * Sets the file name extension.
    */
    void SqSetExtension(const SQChar * ext)
    {
        SetExtension(ext);
    }

    /* --------------------------------------------------------------------------------------------
     * Returns a pointer to the internal name string where the extension starts.
    */
    SQMOD_NODISCARD const SQChar * GetExtensionC() const;

    /* --------------------------------------------------------------------------------------------
     * Returns a path referring to the path's directory.
    */
    SQMOD_NODISCARD SysPath Parent() const;

    /* --------------------------------------------------------------------------------------------
     * Resolves the given path against the current one. If the given path is absolute, it replaces
     * the current one. Otherwise, the relative path is appended to the current path.
    */
    SysPath & Resolve(const SysPath & path);

protected:

    /* --------------------------------------------------------------------------------------------
     * Parse a path using the unix standards.
    */
    void ParseUnix(const SQChar * pos, const SQChar * end);

    /* --------------------------------------------------------------------------------------------
     * Parse a path using the windows standards.
    */
    void ParseWindows(const SQChar * pos, const SQChar * end);

    /* --------------------------------------------------------------------------------------------
     * Parse a path and expect combined windows and unix styles.
    */
    void ParseDynamic(const SQChar * pos, const SQChar * end);

    /* --------------------------------------------------------------------------------------------
     * Parse a path and try to detect it's type automatically.
    */
    void ParseGuess(const SQChar * pos, const SQChar * end);

    /* --------------------------------------------------------------------------------------------
     * Build a path string using the Unix conventions.
    */
    SQMOD_NODISCARD Buffer BuildUnix() const;

    /* --------------------------------------------------------------------------------------------
     * Build a path string using the Windows conventions.
    */
    SQMOD_NODISCARD Buffer BuildWindows() const;

private:

    // --------------------------------------------------------------------------------------------
    StrVec  m_Dirs; /* The list of directories that form the path. */
    String  m_Name; /* The file name if one was specified. */
    CharT   m_Drive; /* The drive letter if one was specified. */
    bool    m_Absolute; /* Whether this path is an absolute path. */

public:

    /* --------------------------------------------------------------------------------------------
     * Returns the platform's path name separator, which separates the components (names) in a path.
    */
    static CharT Separator()
    {
#ifdef SQMOD_OS_WINDOWS
        return '\\';
#else
        return '/';
#endif // SQMOD_OS_WINDOWS
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the platform's path separator, which separates single paths in a list of paths.
    */
    static CharT PathSeparator()
    {
#ifdef SQMOD_OS_WINDOWS
        return ';';
#else
        return ':';
#endif // SQMOD_OS_WINDOWS
    }

    /* --------------------------------------------------------------------------------------------
     * Creates a path referring to a directory.
    */
    static SysPath ForDirectory(const SQChar * path);

    /* --------------------------------------------------------------------------------------------
     * Creates a path referring to a directory.
    */
    static SysPath ForDirectory(const SQChar * path, int32_t style);

    /* --------------------------------------------------------------------------------------------
     * Creates a path referring to a directory.
    */
    static SysPath ForDirectory(const SQChar * path, Style style);

    /* --------------------------------------------------------------------------------------------
     * Creates a path referring to a directory.
    */
    static SysPath ForDirectory(const String & path);

    /* --------------------------------------------------------------------------------------------
     * Creates a path referring to a directory.
    */
    static SysPath ForDirectory(const String & path, Style style);

    /* --------------------------------------------------------------------------------------------
     * Expands all environment variables contained in the path. On Unix, a tilde as first character
     * in the path is replaced with the path to user's home directory.
    */
    static SysPath Expand(const SQChar * path);

    /* --------------------------------------------------------------------------------------------
     * Expands all environment variables contained in the path.
    */
    static SysPath Expand(const String & path)
    {
        return Expand(path.c_str());
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the user's home directory.
    */
    static SysPath Home();

    /* --------------------------------------------------------------------------------------------
     * Returns the user's config directory.
    */
    static SysPath ConfigHome();

    /* --------------------------------------------------------------------------------------------
     * Returns the user's data directory.
    */
    static SysPath DataHome();

    /* --------------------------------------------------------------------------------------------
     * Returns the user's temp directory.
    */
    static SysPath TempHome();

    /* --------------------------------------------------------------------------------------------
     * Returns the user's temp directory.
    */
    static SysPath CacheHome();

    /* --------------------------------------------------------------------------------------------
     * Returns the current working directory.
    */
    static SysPath Working();

    /* --------------------------------------------------------------------------------------------
     * Returns the temporary directory.
    */
    static SysPath Temp();

    /* --------------------------------------------------------------------------------------------
     * Returns the system-wide config directory.
    */
    static SysPath Config();

    /* --------------------------------------------------------------------------------------------
     * Returns the system directory.
    */
    static SysPath System();

    /* --------------------------------------------------------------------------------------------
     * Returns the name of the null device.
    */
    static SysPath Null();

    /* --------------------------------------------------------------------------------------------
     * Returns the real path to the specified file or directory.
    */
    static SysPath Real(const SQChar * path);

    /* --------------------------------------------------------------------------------------------
     * Creates a path from a parent path and a file name. The parent path is expected to reference
     * a directory.
    */
    static SysPath With(const SysPath & parent, const SQChar * name);

    /* --------------------------------------------------------------------------------------------
     * Creates a path in unix format from a string.
    */
    static SysPath MakeUnix(const SQChar * path);

    /* --------------------------------------------------------------------------------------------
     * Creates a path in windows format from a string.
    */
    static SysPath MakeWindows(const SQChar * path);

    /* --------------------------------------------------------------------------------------------
     * Creates a path in native format from a string.
    */
    static SysPath MakeNative(const SQChar * path);

    /* --------------------------------------------------------------------------------------------
     * Creates a path in and guess the format from a string.
    */
    static SysPath MakeGuess(const SQChar * path);

    /* --------------------------------------------------------------------------------------------
     * Creates a path in dynamic format from a string.
    */
    static SysPath MakeDynamic(const SQChar * path);

    /* --------------------------------------------------------------------------------------------
     * Makes sure all separators from a path are the same.
    */
    static String NormalizePath(SQInteger s, StackStrF & val);
};

} // Namespace:: SqMod
