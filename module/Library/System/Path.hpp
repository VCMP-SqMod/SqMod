#ifndef _LIBRARY_SYSPATH_HPP_
#define _LIBRARY_SYSPATH_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
#include <vector>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Retrieve the full path of file.
*/
Buffer GetRealFilePath(CSStr path);

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
    SysPath(bool absolute);

    /* --------------------------------------------------------------------------------------------
     * Creates a path in native format from a string.
    */
    SysPath(CSStr path);

    /* --------------------------------------------------------------------------------------------
     * Creates a path from a string.
    */
    SysPath(CSStr path, Int32 style);

    /* --------------------------------------------------------------------------------------------
     * Creates a path from a string.
    */
    SysPath(CSStr path, Style style);

    /* --------------------------------------------------------------------------------------------
     * Creates a path in native format from a string.
    */
    SysPath(const Buffer & path, Int32 size = -1);

    /* --------------------------------------------------------------------------------------------
     * Creates a path from a string.
    */
    SysPath(const Buffer & path, Style style, Int32 size = -1);

    /* --------------------------------------------------------------------------------------------
     * Creates a path in native format from a string.
    */
    SysPath(const String & path);

    /* --------------------------------------------------------------------------------------------
     * Creates a path from a string.
    */
    SysPath(const String & path, Style style);

    /* --------------------------------------------------------------------------------------------
     * Creates a path from a parent path and a file name. The parent path is expected to reference
     * a directory.
    */
    SysPath(const SysPath & parent, CSStr  name);

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
    SysPath(const SysPath & o);

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    SysPath(SysPath && o);

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~SysPath();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    SysPath & operator = (const SysPath & o);

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    SysPath & operator = (SysPath && o);

    /* --------------------------------------------------------------------------------------------
     * Assigns a string containing a path in native format.
    */
    SysPath & operator = (CSStr path);

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
    operator bool () const;

    /* --------------------------------------------------------------------------------------------
     * Returns the n'th directory in the directory list. If n == depth(), returns the file name.
    */
    const String & operator [] (Uint32 n) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const SysPath & o) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    Object ToString() const;

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
    SysPath & Assign(CSStr path);

    /* --------------------------------------------------------------------------------------------
     * Assigns a string containing a path.
    */
    SysPath & Assign(CSStr path, Int32 style);

    /* --------------------------------------------------------------------------------------------
     * Assigns a string containing a path.
    */
    SysPath & Assign(CSStr path, Style style);

    /* --------------------------------------------------------------------------------------------
     * Assigns a string containing a path in native format.
    */
    SysPath & Assign(const Buffer & path, Int32 size = -1);

    /* --------------------------------------------------------------------------------------------
     * Assigns a string containing a path.
    */
    SysPath & Assign(const Buffer & path, Style style, Int32 size = -1);

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
    SysPath & Assign(const SysPath & parent, CSStr  name);

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
    SysPath & AssignDir(CSStr path);

    /* --------------------------------------------------------------------------------------------
     * The resulting path always refers to a directory and the filename part is empty.
    */
    SysPath & AssignDir(CSStr path, Int32 style);

    /* --------------------------------------------------------------------------------------------
     * The resulting path always refers to a directory and the filename part is empty.
    */
    SysPath & AssignDir(CSStr path, Style style);

    /* --------------------------------------------------------------------------------------------
     * The resulting path always refers to a directory and the filename part is empty.
    */
    SysPath & AssignDir(const Buffer & path, Int32 size = -1);

    /* --------------------------------------------------------------------------------------------
     * The resulting path always refers to a directory and the filename part is empty.
    */
    SysPath & AssignDir(const Buffer & path, Style style, Int32 size = -1);

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
    Buffer ToBuffer() const;

    /* --------------------------------------------------------------------------------------------
     * Returns a string containing the path in the given format.
    */
    Buffer ToBuffer(Style style) const;

    /* --------------------------------------------------------------------------------------------
     * Returns a string containing the path in the given format.
    */
    Object ToStr(Int32 style) const;

    /* --------------------------------------------------------------------------------------------
     * Assigns a string containing a path.
    */
    void FromString(CSStr path);

    /* --------------------------------------------------------------------------------------------
     * See whether the path is absolute.
    */
    bool IsAbsolute() const
    {
        return m_Absolute;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the path is relative.
    */
    bool IsRelative() const
    {
        return !m_Absolute;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the path references a directory.
    */
    bool IsDirectory() const
    {
        return m_Name.empty();
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the path references a file.
    */
    bool IsFile() const
    {
        return !m_Name.empty();
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the path Does not contain a drive, directories or file name.
    */
    bool Empty() const
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
    SysPath & Append(CSStr path);

    /* --------------------------------------------------------------------------------------------
     * Parse the given string and append the resulted path.
    */
    SysPath & Append(CSStr path, Int32 style);

    /* --------------------------------------------------------------------------------------------
     * Parse the given string and append the resulted path.
    */
    SysPath & Append(CSStr path, Style style);

    /* --------------------------------------------------------------------------------------------
     * Parse the given string and append the resulted path.
    */
    SysPath & Append(const Buffer & path, Int32 size = -1);

    /* --------------------------------------------------------------------------------------------
     * Parse the given string and append the resulted path.
    */
    SysPath & Append(const Buffer & path, Style style, Int32 size = -1);

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
    CharT GetDrive() const
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
    Uint32 Depth() const
    {
        return m_Dirs.size();
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the n'th directory in the directory list. If n == depth(), returns the file name.
    */
    const String & Directory(Uint32 n) const;

    /* --------------------------------------------------------------------------------------------
     * Adds a directory to the directory list.
    */
    SysPath & Push(CSStr dir);

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
    SysPath & SetFilename(CSStr name);

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
    const String & GetFilename() const
    {
        return m_Name;
    }

    /* --------------------------------------------------------------------------------------------
     * Set the specified file name.
    */
    void SqSetFilename(CSStr name)
    {
        SetFilename(name);
    }

    /* --------------------------------------------------------------------------------------------
     * Sets the basename part of the file name and does not change the extension.
    */
    SysPath & SetBasename(CSStr name);

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
    String GetBasename() const;

    /* --------------------------------------------------------------------------------------------
     * Sets the basename part of the file name and does not change the extension.
    */
    void SqSetBasename(CSStr name)
    {
        SetBasename(name);
    }

    /* --------------------------------------------------------------------------------------------
     * Sets the file name extension.
    */
    SysPath & SetExtension(CSStr ext);

    /* --------------------------------------------------------------------------------------------
     * Sets the file name extension.
    */
    SysPath & SetExtension(const String & ext);

    /* --------------------------------------------------------------------------------------------
     * Returns the file name extension.
    */
    String GetExtension() const;

    /* --------------------------------------------------------------------------------------------
     * Sets the file name extension.
    */
    void SqSetExtension(CSStr ext)
    {
        SetExtension(ext);
    }

    /* --------------------------------------------------------------------------------------------
     * Returns a pointer to the internal name string where the extension starts.
    */
    CSStr GetExtensionC() const;

    /* --------------------------------------------------------------------------------------------
     * Returns a path referring to the path's directory.
    */
    SysPath Parent() const;

    /* --------------------------------------------------------------------------------------------
     * Resolves the given path against the current one. If the given path is absolute, it replaces
     * the current one. Otherwise, the relative path is appended to the current path.
    */
    SysPath & Resolve(const SysPath & path);

protected:

    /* --------------------------------------------------------------------------------------------
     * Parse a path using the unix standards.
    */
    void ParseUnix(CSStr pos, CSStr end);

    /* --------------------------------------------------------------------------------------------
     * Parse a path using the windows standards.
    */
    void ParseWindows(CSStr pos, CSStr end);

    /* --------------------------------------------------------------------------------------------
     * Parse a path and expect combined windows and unix styles.
    */
    void ParseDynamic(CSStr pos, CSStr end);

    /* --------------------------------------------------------------------------------------------
     * Parse a path and try to detect it's type automatically.
    */
    void ParseGuess(CSStr pos, CSStr end);

    /* --------------------------------------------------------------------------------------------
     * Build a path string using the Unix conventions.
    */
    Buffer BuildUnix() const;

    /* --------------------------------------------------------------------------------------------
     * Build a path string using the Windows conventions.
    */
    Buffer BuildWindows() const;

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
#ifdef GMOD_OS_WINDOWS
        return '\\';
#else
        return '/';
#endif // GMOD_OS_WINDOWS
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the platform's path separator, which separates single paths in a list of paths.
    */
    static CharT PathSeparator()
    {
#ifdef GMOD_OS_WINDOWS
        return ';';
#else
        return ':';
#endif // GMOD_OS_WINDOWS
    }

    /* --------------------------------------------------------------------------------------------
     * Creates a path referring to a directory.
    */
    static SysPath ForDirectory(CSStr path);

    /* --------------------------------------------------------------------------------------------
     * Creates a path referring to a directory.
    */
    static SysPath ForDirectory(CSStr path, Int32 style);

    /* --------------------------------------------------------------------------------------------
     * Creates a path referring to a directory.
    */
    static SysPath ForDirectory(CSStr path, Style style);

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
    static SysPath Expand(CSStr path);

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
     * Returns the systemwide config directory.
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
    static SysPath Real(CSStr path);

    /* --------------------------------------------------------------------------------------------
     * Creates a path from a parent path and a file name. The parent path is expected to reference
     * a directory.
    */
    static SysPath With(const SysPath & parent, CSStr name);

    /* --------------------------------------------------------------------------------------------
     * Creates a path in unix format from a string.
    */
    static SysPath MakeUnix(CSStr path);

    /* --------------------------------------------------------------------------------------------
     * Creates a path in windows format from a string.
    */
    static SysPath MakeWindows(CSStr path);

    /* --------------------------------------------------------------------------------------------
     * Creates a path in native format from a string.
    */
    static SysPath MakeNative(CSStr path);

    /* --------------------------------------------------------------------------------------------
     * Creates a path in and guess the format from a string.
    */
    static SysPath MakeGuess(CSStr path);

    /* --------------------------------------------------------------------------------------------
     * Creates a path in dynamic format from a string.
    */
    static SysPath MakeDynamic(CSStr path);

    /* --------------------------------------------------------------------------------------------
     * Makes sure all separators from a path are the same.
    */
    static String NormalizePath(SQInteger s, StackStrF & val);
};

} // Namespace:: SqMod

#endif // _LIBRARY_SYSPATH_HPP_
