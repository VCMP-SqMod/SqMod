// ------------------------------------------------------------------------------------------------
#include "Library/System/Path.hpp"
#include "Library/System/Env.hpp"

// ------------------------------------------------------------------------------------------------
#include <cctype>
#include <cstring>

// ------------------------------------------------------------------------------------------------
#include <sqratConst.h>

// ------------------------------------------------------------------------------------------------
#ifdef SQMOD_OS_WINDOWS
    #include <windows.h>
#else
    #include <linux/limits.h>
    #include <unistd.h>
#endif // SQMOD_OS_WINDOWS

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
SQMOD_DECL_TYPENAME(Typename, _SC("SqSysPath"))

// ------------------------------------------------------------------------------------------------
Buffer GetRealFilePath(const SQChar * path)
{
    // Make sure the specified path is valid
    if (!path || *path == '\0')
    {
        STHROWF("Cannot obtain real path of empty or invalid path");
    }
    // Allocate a buffer large enough to hold a full path
    Buffer b(SQMOD_MAX_PATH);

#ifdef SQMOD_OS_WINDOWS
    // Attempt to obtain the full path to the file
    DWORD ret = ::GetFullPathNameA(path, b.Size< char >(), b.Get< char >(), nullptr);
    // Should we allocate a bigger buffer?
    if (ret > b.Size< PChar >())
    {
        // Grab a bigger buffer
        b.Adjust(ret);
        // Grab the path again
        ret = GetFullPathNameA(path, b.Size< char >(), b.Get< char >(), nullptr);
    }
    // Did we fail to obtain a path?
    if (ret == 0 && ::GetLastError() != 0)
    {
        STHROWLASTF("Cannot obtain real path of (%s)", path);
    }
    // Adjust the buffer cursor
    b.Move(ret);
#else
    // Attempt to obtain the full path to the file
    if (!realpath(path, b.Data()))
    {
        STHROWLASTF("Cannot obtain real path of (%s)", path);
    }
    // Adjust the buffer cursor
    b.Move(std::strlen(b.Data()));
#endif // SQMOD_OS_WINDOWS

    // Return ownership of the buffer
    return b;
}

// ------------------------------------------------------------------------------------------------
SysPath::SysPath()
    : m_Dirs()
    , m_Name()
    , m_Drive(0)
    , m_Absolute(false)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
SysPath::SysPath(bool absolute)
    : m_Dirs()
    , m_Name()
    , m_Drive(0)
    , m_Absolute(absolute)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
SysPath::SysPath(const SQChar * path)
    : m_Dirs()
    , m_Name()
    , m_Drive(0)
    , m_Absolute(false)
{
    Assign(path);
}

// ------------------------------------------------------------------------------------------------
SysPath::SysPath(const SQChar * path, int32_t style)
    : m_Dirs()
    , m_Name()
    , m_Drive(0)
    , m_Absolute(false)
{
    Assign(path, static_cast< Style >(style));
}

// ------------------------------------------------------------------------------------------------
SysPath::SysPath(const SQChar * path, Style style)
    : m_Dirs()
    , m_Name()
    , m_Drive(0)
    , m_Absolute(false)
{
    Assign(path, style);
}

// ------------------------------------------------------------------------------------------------
SysPath::SysPath(const Buffer & path, int32_t size)
    : m_Dirs()
    , m_Name()
    , m_Drive(0)
    , m_Absolute(false)
{
    Assign(path, Style::Guess, size);
}

// ------------------------------------------------------------------------------------------------
SysPath::SysPath(const Buffer & path, Style style, int32_t size)
    : m_Dirs()
    , m_Name()
    , m_Drive(0)
    , m_Absolute(false)
{
    Assign(path, style, size);
}

// ------------------------------------------------------------------------------------------------
SysPath::SysPath(const String & path)
    : m_Dirs()
    , m_Name()
    , m_Drive(0)
    , m_Absolute(false)
{
    Assign(path, Style::Guess);
}

// ------------------------------------------------------------------------------------------------
SysPath::SysPath(const String & path, Style style)
    : m_Dirs()
    , m_Name()
    , m_Drive(0)
    , m_Absolute(false)
{
    Assign(path, style);
}

// ------------------------------------------------------------------------------------------------
SysPath::SysPath(const SysPath & parent, const SQChar *  name)
    : m_Dirs(parent.m_Dirs)
    , m_Name(name ? name : "")
    , m_Drive(parent.m_Drive)
    , m_Absolute(parent.m_Absolute)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
SysPath::SysPath(const SysPath & parent, const String &  name) // NOLINT(modernize-pass-by-value)
    : m_Dirs(parent.m_Dirs)
    , m_Name(name)
    , m_Drive(parent.m_Drive)
    , m_Absolute(parent.m_Absolute)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
SysPath::SysPath(const SysPath & parent, const SysPath &  relative)
    : m_Dirs(parent.m_Dirs)
    , m_Name(parent.m_Name)
    , m_Drive(parent.m_Drive)
    , m_Absolute(parent.m_Absolute)
{
    // Resolve the specified path
    Resolve(relative);
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::operator = (const SQChar * path)
{
    Assign(path);
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::operator = (const String & path)
{
    Assign(path);
    return *this;
}

// ------------------------------------------------------------------------------------------------
bool SysPath::operator == (const SysPath & o) const
{
    return (m_Drive == o.m_Drive) && (m_Absolute == o.m_Absolute)
            && (m_Name == o.m_Name) && (m_Dirs == o.m_Dirs);
}

// ------------------------------------------------------------------------------------------------
bool SysPath::operator != (const SysPath & o) const
{
    return !(*this == o);
}

// ------------------------------------------------------------------------------------------------
SysPath::operator bool () const
{
    return (m_Dirs.empty() && m_Name.empty() && (m_Drive == 0) && !m_Absolute);
}

// ------------------------------------------------------------------------------------------------
const String & SysPath::operator [] (uint32_t n) const
{
    // Is this within the bounds of the directory list?
    if (n < m_Dirs.size())
    {
        return m_Dirs[n];
    }
    // Fall back to the file name
    else
    {
        return m_Name;
    }
}

// ------------------------------------------------------------------------------------------------
int32_t SysPath::Cmp(const SysPath & o) const
{
    if (*this == o)
        return 0;
    else if (ToBuffer().Position() > o.ToBuffer().Position())
        return 1;
    else
        return -1;
}

// ------------------------------------------------------------------------------------------------
Object SysPath::ToString() const
{
    return BufferToStrObj(ToBuffer());
}

// ------------------------------------------------------------------------------------------------
void SysPath::Swap(SysPath & path)
{
    m_Dirs.swap(path.m_Dirs);
    m_Name.swap(path.m_Name);
    std::swap(m_Drive, path.m_Drive);
    std::swap(m_Absolute, path.m_Absolute);
}

// ------------------------------------------------------------------------------------------------
void SysPath::Clear()
{
    m_Dirs.clear();
    m_Name.clear();
    m_Drive = 0;
    m_Absolute = false;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::Assign(const SQChar * path)
{
    // Is the specified path valid?
    if (!path || *path == '\0')
    {
        // Just clear current path
        Clear();
    }
    else
    {
#ifdef SQMOD_OS_WINDOWS
        ParseWindows(path, path + strlen(path));
#else
        ParseUnix(path, path + strlen(path));
#endif // SQMOD_OS_WINDOWS
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::Assign(const SQChar * path, int32_t style)
{
    return Assign(path, static_cast< Style >(style));
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::Assign(const SQChar * path, Style style)
{
    // Is the specified path valid?
    if (!path || *path == '\0')
    {
        // Just clear current path
        Clear();
    }
    else
    {
        // Identify which style was requested
        switch (style)
        {
        case Style::Unix:
            ParseUnix(path, path + strlen(path));
        break;
        case Style::Windows: // NOLINT(bugprone-branch-clone)
            ParseWindows(path, path + strlen(path));
        break;
        case Style::Native:
#ifdef SQMOD_OS_WINDOWS
            ParseWindows(path, path + strlen(path));
#else
            ParseUnix(path, path + strlen(path));
#endif // SQMOD_OS_WINDOWS
        break;
        case Style::Guess:
            ParseGuess(path, path + strlen(path));
        break;
        case Style::Dynamic:
            ParseDynamic(path, path + strlen(path));
        break;
        }
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::Assign(const Buffer & path, int32_t size)
{
    // Is the specified path valid?
    if (!path)
    {
        // Just clear current path
        Clear();
    }
    else if (size < 0)
    {
#ifdef SQMOD_OS_WINDOWS
        ParseWindows(path.Data(), &path.Cursor());
#else
        ParseUnix(path.Data(), &path.Cursor());
#endif // SQMOD_OS_WINDOWS
    }
    else if (static_cast< uint32_t >(size) < path.Capacity())
    {
#ifdef SQMOD_OS_WINDOWS
        ParseWindows(path.Data(), path.Data() + size);
#else
        ParseUnix(path.Data(), path.Data() + size);
#endif // SQMOD_OS_WINDOWS
    }
    else
    {
        STHROWF("The specified path size is out of range: {} >= {}", size, path.Capacity());
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::Assign(const Buffer & path, Style style, int32_t size)
{
    // Is the specified path valid?
    if (!path)
    {
        // Just clear current path
        Clear();
    }
    else if (size < 0)
    {
        // Identify which style was requested
        switch (style)
        {
        case Style::Unix:
            ParseUnix(path.Data(), &path.Cursor());
        break;
        case Style::Windows: // NOLINT(bugprone-branch-clone)
            ParseWindows(path.Data(), &path.Cursor());
        break;
        case Style::Native:
#ifdef SQMOD_OS_WINDOWS
        ParseWindows(path.Data(), &path.Cursor());
#else
        ParseUnix(path.Data(), &path.Cursor());
#endif // SQMOD_OS_WINDOWS
        break;
        case Style::Guess:
            ParseGuess(path.Data(), &path.Cursor());
        break;
        case Style::Dynamic:
            ParseDynamic(path.Data(), &path.Cursor());
        break;
        }
    }
    else if (static_cast< uint32_t >(size) < path.Capacity())
    {
        // Identify which style was requested
        switch (style)
        {
        case Style::Unix:
            ParseUnix(path.Data(), path.Data() + size);
        break;
        case Style::Windows: // NOLINT(bugprone-branch-clone)
            ParseWindows(path.Data(), path.Data() + size);
        break;
        case Style::Native:
#ifdef SQMOD_OS_WINDOWS
        ParseWindows(path.Data(), path.Data() + size);
#else
        ParseUnix(path.Data(), path.Data() + size);
#endif // SQMOD_OS_WINDOWS
        break;
        case Style::Guess:
            ParseGuess(path.Data(), path.Data() + size);
        break;
        case Style::Dynamic:
            ParseDynamic(path.Data(), path.Data() + size);
        break;
        }
    }
    else
    {
        STHROWF("The specified path size is out of range: {} >= {}", size, path.Capacity());
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::Assign(const String & path)
{
    // Is the specified path valid?
    if (path.empty())
    {
        // Just clear current path
        Clear();
    }
    else
    {
#ifdef SQMOD_OS_WINDOWS
        ParseWindows(path.data(), path.data() + path.size());
#else
        ParseUnix(path.data(), path.data() + path.size());
#endif // SQMOD_OS_WINDOWS
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::Assign(const String & path, Style style)
{
    // Is the specified path valid?
    if (path.empty())
    {
        // Just clear current path
        Clear();
    }
    else
    {
        // Identify which style was requested
        switch (style)
        {
        case Style::Unix:
            ParseUnix(path.data(), path.data() + path.size());
        break;
        case Style::Windows: // NOLINT(bugprone-branch-clone)
            ParseWindows(path.data(), path.data() + path.size());
        break;
        case Style::Native:
#ifdef SQMOD_OS_WINDOWS
        ParseWindows(path.data(), path.data() + path.size());
#else
        ParseUnix(path.data(), path.data() + path.size());
#endif // SQMOD_OS_WINDOWS
        break;
        case Style::Guess:
            ParseGuess(path.data(), path.data() + path.size());
        break;
        case Style::Dynamic:
            ParseDynamic(path.data(), path.data() + path.size());
        break;
        }
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::Assign(const SysPath & parent, const SQChar *  name)
{
    // Copy the parent values
    *this = parent;
    // Set the specified file name
    SetFilename(name);
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::Assign(const SysPath & parent, const String &  name)
{
    // Copy the parent values
    *this = parent;
    // Set the specified file name
    SetFilename(name);
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::Assign(const SysPath & parent, const SysPath &  relative)
{
    // Copy the parent values
    *this = parent;
    // Resolve the specified path
    Resolve(relative);
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::Assign(const SysPath & path)
{
    // Just use regular assignment
    *this = path;
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::Assign(SysPath && path)
{
    // Just use regular assignment
    *this = path;
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::AssignDir(const SQChar * path)
{
    // Assign the specified path
    Assign(path);
    // Force it to be a directory and allow chaining
    return MakeDirectory();
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::AssignDir(const SQChar * path, int32_t style)
{
    // Assign the specified path
    Assign(path, style);
    // Force it to be a directory and allow chaining
    return MakeDirectory();
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::AssignDir(const SQChar * path, Style style)
{
    // Assign the specified path
    Assign(path, style);
    // Force it to be a directory and allow chaining
    return MakeDirectory();
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::AssignDir(const Buffer & path, int32_t size)
{
    // Assign the specified path
    Assign(path, size);
    // Force it to be a directory and allow chaining
    return MakeDirectory();
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::AssignDir(const Buffer & path, Style style, int32_t size)
{
    // Assign the specified path
    Assign(path, style, size);
    // Force it to be a directory and allow chaining
    return MakeDirectory();
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::AssignDir(const String & path)
{
    // Assign the specified path
    Assign(path);
    // Force it to be a directory and allow chaining
    return MakeDirectory();
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::AssignDir(const String & path, Style style)
{
    // Assign the specified path
    Assign(path, style);
    // Force it to be a directory and allow chaining
    return MakeDirectory();
}

// ------------------------------------------------------------------------------------------------
Buffer SysPath::ToBuffer() const
{
#ifdef SQMOD_OS_WINDOWS
    return BuildWindows();
#else
    return BuildUnix();
#endif // SQMOD_OS_WINDOWS
}

// ------------------------------------------------------------------------------------------------
Buffer SysPath::ToBuffer(Style style) const
{
    if (style == Style::Unix)
    {
        return BuildUnix();
    }
    else if (style == Style::Windows)
    {
        return BuildWindows();
    }
#ifdef SQMOD_OS_WINDOWS
    return BuildWindows();
#else
    return BuildUnix();
#endif // SQMOD_OS_WINDOWS
}

// ------------------------------------------------------------------------------------------------
Object SysPath::ToStr(int32_t style) const
{
    return BufferToStrObj(ToBuffer(static_cast< Style >(style)));
}

// ------------------------------------------------------------------------------------------------
void SysPath::FromString(const SQChar * path)
{
    Assign(path);
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::MakeDirectory()
{
    // Do we even have a name?
    if (!m_Name.empty())
    {
        // Make it a directory
        m_Dirs.push_back(std::move(m_Name));
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::MakeFile()
{
    // Do we have some directories and no existing file?
    if (!m_Dirs.empty() && m_Name.empty())
    {
        // Use the last directory as a file
        m_Name = std::move(m_Dirs.back());
        // Remove it from the directory list
        m_Dirs.pop_back();
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::MakeParent()
{
    // Do we have a name?
    if (m_Name.empty())
    {
        // Do we have any existing directories?
        if (m_Dirs.empty())
        {
            // Make sure this path isn't absolute
            if (!m_Absolute)
            {
                // Reference the parent directory
                m_Dirs.emplace_back("..");
            }
        }
        else
        {
            // Are we already referencing a parent?
            if (m_Dirs.back().compare("..") == 0) // NOLINT(readability-string-compare)
            {
                // Then reference the parent of that parent
                m_Dirs.emplace_back("..");
            }
            // Just pop the last directory
            else
            {
                m_Dirs.pop_back();
            }
        }
    }
    // Just clear the name
    else
    {
        m_Name.clear();
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::MakeAbsolute()
{
    // Is this path already absolute?
    if (!m_Absolute)
    {
        MakeAbsolute(Working());
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::MakeAbsolute(const SysPath & base)
{
    // Is this path already absolute?
    if (!m_Absolute)
    {
        MakeAbsolute(base);
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::MakeAbsolute(SysPath && base)
{
    // Is this path already absolute?
    if (!m_Absolute)
    {
        // Reserve the space upfront
        base.m_Dirs.reserve(base.m_Dirs.size() + m_Dirs.size());
        // Move our directories at the back
        for (auto & dir : m_Dirs)
        {
            base.m_Dirs.push_back(std::move(dir));
        }
        // Take ownership of base directories
        m_Dirs.swap(base.m_Dirs);
        // Copy the drive letter
        m_Drive = base.m_Drive;
        // Make absolute only if base is
        m_Absolute = base.m_Absolute;
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::Append(const SysPath & path)
{
    // Make sure this is a directory
    MakeDirectory();
    // Only attempt to append if not empty
    if (!path.Empty())
    {
        // Append the directories from the specified path
        m_Dirs.insert(m_Dirs.end(), path.m_Dirs.begin(), path.m_Dirs.end());
        // Copy the file name if any
        m_Name = path.m_Name;
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::Append(SysPath && path)
{
    // Make sure this is a directory
    MakeDirectory();
    // Only attempt to append if not empty
    if (!path.Empty())
    {
        // Request the necessary directory list size upfront
        m_Dirs.reserve(m_Dirs.size() + path.m_Dirs.size());
        // Move all directories at the back of our list
        for (auto & dir : path.m_Dirs)
        {
            m_Dirs.push_back(std::move(dir));
        }
        // Move the file name if any
        m_Name = std::move(path.m_Name);
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::Append(const SQChar * path)
{
    return Append(SysPath(path));
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::Append(const SQChar * path, int32_t style)
{
    return Append(SysPath(path, style));
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::Append(const SQChar * path, Style style)
{
    return Append(SysPath(path, style));
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::Append(const Buffer & path, int32_t size)
{
    return Append(SysPath(path, size));
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::Append(const Buffer & path, Style style, int32_t size)
{
    return Append(SysPath(path, style, size));
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::Append(const String & path)
{
    return Append(SysPath(path));
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::Append(const String & path, Style style)
{
    return Append(SysPath(path, style));
}

// ------------------------------------------------------------------------------------------------
const String & SysPath::Directory(uint32_t n) const
{
    // Is this within the bounds of the directory list?
    if (n < m_Dirs.size())
    {
        return m_Dirs[n];
    }
    // Fall back to the file name
    else
    {
        return m_Name;
    }
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::Push(const SQChar * dir)
{
    // Is the specified directory valid?
    if (dir && *dir != 0)
    {
        Push(String(dir));
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::Push(const String & dir)
{
    // Is the specified directory valid?
    if (!dir.empty() && dir.compare(".") != 0) // NOLINT(readability-string-compare)
    {
        Push(String(dir));
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::Push(String && dir)
{
    // Is the specified directory valid?
    if (!dir.empty() && dir.compare(".") != 0) // NOLINT(readability-string-compare)
    {
        // Does it refer to a parent directory?
        if (dir.compare("..") == 0) // NOLINT(readability-string-compare)
        {
            // Is our last directory already a reference to a parent?
            if (!m_Dirs.empty() && m_Dirs.back().compare("..") != 0) // NOLINT(readability-string-compare)
            {
                m_Dirs.pop_back();
            }
            // Move it at the back of our list
            else if (!m_Absolute)
            {
                m_Dirs.push_back(dir);
            }
        }
        // Move it at the back of our list
        else
        {
            m_Dirs.push_back(dir);
        }
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::PopBack()
{
    // Do we even have any directories?
    if (!m_Dirs.empty())
    {
        // Erase the last one
        m_Dirs.pop_back();
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::PopFront()
{
    // Do we even have any directories?
    if (!m_Dirs.empty())
    {
        // Erase the first one
        m_Dirs.erase(m_Dirs.begin());
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::SetFilename(const SQChar * name)
{
    // Is the file name even valid?
    if (name)
    {
        m_Name.assign(name);
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::SetFilename(const String & name)
{
    // Is the file name even valid?
    m_Name.assign(name);
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::SetFilename(String && name)
{
    // Is the file name even valid?
    m_Name.assign(name);
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::SetBasename(const SQChar * name)
{
    // Is the file name even valid?
    if (name)
    {
        // Extract the current extension
        String ext = GetExtension();
        // Assign the new base name
        m_Name.assign(name);
        // Was there an extension before?
        if (!ext.empty())
        {
            // Add the extension separator
            m_Name.push_back('.');
            // Add the original extension
            m_Name.append(ext);
        }
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::SetBasename(const String & name)
{
    // Extract the current extension
    String ext = GetExtension();
    // Assign the new base name
    m_Name.assign(name);
    // Was there an extension before?
    if (!ext.empty())
    {
        // Add the extension separator
        m_Name.push_back('.');
        // Add the original extension
        m_Name.append(ext);
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::SetBasename(String && name)
{
    // Extract the current extension
    String ext = GetExtension();
    // Assign the new base name
    m_Name.assign(name);
    // Was there an extension before?
    if (!ext.empty())
    {
        // Add the extension separator
        m_Name.push_back('.');
        // Add the original extension
        m_Name.append(ext);
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
String SysPath::GetBasename() const
{
    // Attempt to find the last dot in the file name
    const String::size_type pos = m_Name.rfind('.');
    // Was there an extension separator?
    if (pos != String::npos)
    {
        // Return everything before the separator
        return m_Name.substr(0, pos);
    }
    // Return the whole name
    return m_Name;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::SetExtension(const SQChar * ext)
{
    // Attempt to find the last dot in the file name
    const String::size_type pos = m_Name.rfind('.');
    // Was there an extension separator?
    if (pos != String::npos)
    {
        // Erase the current extension
        m_Name.resize(pos);
    }
    // Is there an extension to append?
    if (ext && *ext != 0)
    {
        m_Name.push_back('.');
        m_Name.append(ext);
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::SetExtension(const String & ext)
{
    // Attempt to find the last dot in the file name
    const String::size_type pos = m_Name.rfind('.');
    // Was there an extension separator?
    if (pos != String::npos)
    {
        // Erase the current extension
        m_Name.resize(pos);
    }
    // Is there an extension to append?
    if (!ext.empty())
    {
        m_Name.push_back('.');
        m_Name.append(ext);
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
String SysPath::GetExtension() const
{
    // Attempt to find the last dot in the file name
    const String::size_type pos = m_Name.rfind('.');
    // Was there an extension separator?
    if (pos != String::npos)
    {
        // Return everything after the separator
        return m_Name.substr(pos + 1);
    }
    // Default to an empty string
    return String();
}

// ------------------------------------------------------------------------------------------------
const SQChar * SysPath::GetExtensionC() const
{
    // Attempt to find the last dot in the file name
    const String::size_type pos = m_Name.rfind('.');
    // Was there an extension separator?
    if (pos != String::npos)
    {
        // Because indexing starts from 0, the separator is skipped
        return &m_Name[pos+1];
    }
    // Default to an empty string
    return "";
}

// ------------------------------------------------------------------------------------------------
SysPath SysPath::Parent() const
{
    // Make a copy
    SysPath p(*this);
    // Force the copy to be parent
    p.MakeParent();
    // Return ownership of copy
    return p;
}

// ------------------------------------------------------------------------------------------------
SysPath & SysPath::Resolve(const SysPath & path)
{
    // Is the specified path absolute?
    if (path.m_Absolute)
    {
        // Copy it's values
        Assign(path);
    }
    else
    {
        // Append its directories
        for (const auto & dir : path.m_Dirs)
        {
            Push(dir);
        }
        // Copy its file name
        m_Name = path.m_Name;
    }
    // Allow chaining
    return *this;
}

// ------------------------------------------------------------------------------------------------
void SysPath::ParseUnix(const SQChar * pos, const SQChar * end)
{
    // Clear previous path information
    Clear();
    // Is there even a path to parse?
    if (pos == end)
    {
        return;
    }
    // If the path starts with a forward slash then the path is absolute
    else if (*pos == '/')
    {
        // This is now an absolute path
        m_Absolute = true;
        // Skip this directory separator
        ++pos;
    }
    // If the path starts with the tilde character then the home directory was requested
    else if (*pos == '~')
    {
        // To be expanded, the tilde character must be followed by a slash or no other character
        if (pos == end || *(++pos) == '/')
        {
            // Obtain the home path
            SysPath up(Home());
            // Take ownership, don't copy
            m_Dirs.swap(up.m_Dirs);
            // This is now an absolute path
            m_Absolute = true;
        }
        // Go back to the previous character and use it literally
        else
        {
            --pos;
        }
    }
    // Make another iterator to slice directory names in one go
    const SQChar * itr = pos;
    // Extract the remaining directories from the specified path
    while (itr != end)
    {
        // Have we encountered a directory separator?
        if (*itr == '/')
        {
            // Slice the name from the path if valid
            if (itr != pos)
            {
                m_Dirs.emplace_back(pos, itr);
            }
            // Move the iterator to the current position
            pos = ++itr;
        }
        else
        {
            ++itr;
        }
    }
    // Grab the last name if any
    if (pos != end)
    {
        m_Name.assign(pos, end);
    }
}

// ------------------------------------------------------------------------------------------------
void SysPath::ParseWindows(const SQChar * pos, const SQChar * end)
{
    // Clear previous path information
    Clear();
    // Is there even a path to parse?
    if (pos == end)
    {
        return;
    }
    // If the path starts with a forward slash then the path is absolute
    else if (*pos == '\\')
    {
        // This is now an absolute path
        m_Absolute = true;
        // Skip this directory separator
        ++pos;
    }
    // If the path starts with the tilde character then the home directory was requested
    else if (*pos == '~')
    {
        // To be expanded, the tilde character must be followed by a slash or no other character
        if (pos == end || *(++pos) == '\\')
        {
            // Obtain the home path
            SysPath up(Home());
            // Take ownership, don't copy
            m_Dirs.swap(up.m_Dirs);
            // Also copy the drive letter
            m_Drive = up.m_Drive;
            // This is now an absolute path
            m_Absolute = true;
        }
        // Go back to the previous character and use it literally
        else
        {
            --pos;
        }
    }
    // Is it possible to have a drive letter?
    else if ((end - pos) > 2 && pos[1] == ':' && pos[2] == '\\' && isalpha(*pos) != 0)
    {
        // Grab the drive letter
        m_Drive = *pos;
        // Skip the drive path and colon
        pos += 2;
    }
    // Is it possible to have just the drive letter?
    else if ((end - pos) == 2 && pos[1] == ':' && isalpha(*pos) != 0)
    {
        // Grab the drive letter
        m_Drive = *pos;
        // Nothing left to parse
        return;
    }
    // Make another iterator to slice directory names in one go
    const SQChar * itr = pos;
    // Extract the remaining directories from the specified path
    while (itr != end)
    {
        // Have we encountered a directory separator?
        if (*itr == '\\')
        {
            // Slice the name from the path if valid
            if (itr != pos)
            {
                m_Dirs.emplace_back(pos, itr);
            }
            // Move the iterator to the current position
            pos = ++itr;
        }
        else
        {
            ++itr;
        }
    }
    // Grab the last name if any
    if (pos != end)
    {
        m_Name.assign(pos, end);
    }
}

// ------------------------------------------------------------------------------------------------
void SysPath::ParseDynamic(const SQChar * pos, const SQChar * end)
{
    // Clear previous path information
    Clear();
    // Is there even a path to parse?
    if (pos == end)
    {
        return;
    }
    // If the path starts with a slash then the path is absolute
    else if (*pos == '\\' || *pos == '/')
    {
        // This is now an absolute path
        m_Absolute = true;
        // Skip this directory separator
        ++pos;
    }
    // If the path starts with the tilde character then the home directory was requested
    else if (*pos == '~')
    {
        // Skip the tilde character
        ++pos;
        // The tilde character must be followed by a slash or no other character to be expanded
        if (pos == end || *pos == '/' || *pos == '\\')
        {
            // Obtain the home path
            SysPath up(Home());
            // Take ownership, don't copy
            m_Dirs.swap(up.m_Dirs);
#ifdef SQMOD_OS_WINDOWS
            // Also copy the drive letter
            m_Drive = up.m_Drive;
#endif // SQMOD_OS_WINDOWS
            // This is now an absolute path
            m_Absolute = true;
        }
        // Go back to the previous character and use it literally
        else
        {
            --pos;
        }
    }
#ifdef SQMOD_OS_WINDOWS
    // Is it possible to have a drive letter?
    else if ((end - pos) > 2 && pos[1] == ':' && (pos[2] == '\\' || pos[2] == '/') && isalpha(*pos) != 0)
    {
        // Grab the drive letter
        m_Drive = *pos;
        // Skip the drive path and colon
        pos += 2;
    }
    // Is it possible to have just the drive letter?
    else if ((end - pos) == 2 && pos[1] == ':' && isalpha(*pos) != 0)
    {
        // Grab the drive letter
        m_Drive = *pos;
        // Nothing left to parse
        return;
    }
#endif // SQMOD_OS_WINDOWS
    // Make another iterator to slice directory names in one go
    const SQChar * itr = pos;
    // Extract the remaining directories from the specified path
    while (itr != end)
    {
        // Have we encountered a directory separator?
        if (*itr == '/' || *itr == '\\')
        {
            // Slice the name from the path if valid
            if (itr != pos)
            {
                m_Dirs.emplace_back(pos, itr);
            }
            // Move the iterator to the current position
            pos = ++itr;
        }
        else
        {
            ++itr;
        }
    }
    // Grab the last name if any
    if (pos != end)
    {
        m_Name.assign(pos, end);
    }
}

// ------------------------------------------------------------------------------------------------
void SysPath::ParseGuess(const SQChar * pos, const SQChar * end)
{
    // Scan for forward slash
    const bool has_fwslash = (strchr(pos, '/') != nullptr);
    const bool has_bwslash = (strchr(pos, '\\') != nullptr);
    // Does it contain both forward and backward slashes?
    if (has_fwslash && has_bwslash)
    { // NOLINT(bugprone-branch-clone)
        ParseDynamic(pos, end);
    }
    // Does it contain the forward slash?
    else if (has_fwslash)
    {
        ParseUnix(pos, end);
    }
    // Does it contain the backward slash?
    else if (has_bwslash)
    { // NOLINT(bugprone-branch-clone)
        ParseWindows(pos, end);
    }
    // Does it contain a drive letter?
    else if ((end - pos) == 2 && pos[1] == ':' && isalpha(*pos) != 0)
    {
        ParseWindows(pos, end);
    }
    // Try to parse it as a dynamic path
    else
    {
        ParseDynamic(pos, end);
    }
}

// ------------------------------------------------------------------------------------------------
Buffer SysPath::BuildUnix() const
{
    // Obtain a buffer capable of storing a full path
    Buffer b(SQMOD_MAX_PATH);
    // Is this an absolute path?
    if (m_Absolute)
    {
        // Start with a slash
        b.Push('/');
    }
    // Concatenate all directories
    for (const auto & dir : m_Dirs)
    {
        // Append the name
        b.AppendS(dir.c_str(), static_cast< uint32_t >(dir.size()));
        // Separate from next
        b.Push('/');
    }
    // Is there a file name to add?
    if (!m_Name.empty())
    {
        b.AppendS(m_Name.c_str(), static_cast< uint32_t >(m_Name.size()));
    }
    // Make sure the string is null terminated
    b.Cursor() = '\0';
    // Return ownership of buffer
    return b;
}

// ------------------------------------------------------------------------------------------------
Buffer SysPath::BuildWindows() const
{
    // Obtain a buffer capable of storing a full path
    Buffer b(SQMOD_MAX_PATH);
    // Does it have a drive letter?
    if (isalpha(m_Drive) != 0)
    {
        // Add the drive letter
        b.Push(m_Drive);
        // Add the colon
        b.Push(':');
        // Add the slash
        b.Push('\\');
    }
    // Is this an absolute path?
    else if (m_Absolute)
    {
        // Start with a slash
        b.Push('\\');
    }
    // Concatenate all directories
    for (const auto & dir : m_Dirs)
    {
        // Append the name
        b.AppendS(dir.c_str(), static_cast< uint32_t >(dir.size()));
        // Separate from next
        b.Push('\\');
    }
    // Is there a file name to add?
    if (!m_Name.empty())
    {
        b.AppendS(m_Name.c_str(), static_cast< uint32_t >(m_Name.size()));
    }
    // Make sure the string is null terminated
    b.Cursor() = '\0';
    // Return ownership of buffer
    return b;
}

// ------------------------------------------------------------------------------------------------
SysPath SysPath::ForDirectory(const SQChar * path)
{
    return SysPath(path).MakeDirectory();
}

// ------------------------------------------------------------------------------------------------
SysPath SysPath::ForDirectory(const SQChar * path, int32_t style)
{
    return SysPath(path, style).MakeDirectory();
}

// ------------------------------------------------------------------------------------------------
SysPath SysPath::ForDirectory(const SQChar * path, Style style)
{
    return SysPath(path, style).MakeDirectory();
}

// ------------------------------------------------------------------------------------------------
SysPath SysPath::ForDirectory(const String & path)
{
    return SysPath(path).MakeDirectory();
}

// ------------------------------------------------------------------------------------------------
SysPath SysPath::ForDirectory(const String & path, Style style)
{
    return SysPath(path, style).MakeDirectory();
}

// ------------------------------------------------------------------------------------------------
SysPath SysPath::Expand(const SQChar * path)
{
    return SysPath(SysEnv::ExpandPath(path));
}

// ------------------------------------------------------------------------------------------------
SysPath SysPath::Home()
{
    return SysPath(SysEnv::HomeDir());
}

// ------------------------------------------------------------------------------------------------
SysPath SysPath::ConfigHome()
{
    return SysPath(SysEnv::ConfigHomeDir());
}

// ------------------------------------------------------------------------------------------------
SysPath SysPath::DataHome()
{
    return SysPath(SysEnv::DataHomeDir());
}

// ------------------------------------------------------------------------------------------------
SysPath SysPath::TempHome()
{
    return SysPath(SysEnv::TempHomeDir());
}

// ------------------------------------------------------------------------------------------------
SysPath SysPath::CacheHome()
{
    return SysPath(SysEnv::CacheHomeDir());
}

// ------------------------------------------------------------------------------------------------
SysPath SysPath::Working()
{
    return SysPath(SysEnv::WorkingDir());
}

// ------------------------------------------------------------------------------------------------
SysPath SysPath::Temp()
{
    return SysPath(SysEnv::TempDir());
}

// ------------------------------------------------------------------------------------------------
SysPath SysPath::Config()
{
    return SysPath(SysEnv::ConfigDir());
}

// ------------------------------------------------------------------------------------------------
SysPath SysPath::System()
{
    return SysPath(SysEnv::SystemDir());
}

// ------------------------------------------------------------------------------------------------
SysPath SysPath::Null()
{
    return SysPath(SysEnv::NullDir());
}

// ------------------------------------------------------------------------------------------------
SysPath SysPath::Real(const SQChar * path)
{
    return SysPath(GetRealFilePath(path));
}

// ------------------------------------------------------------------------------------------------
SysPath SysPath::With(const SysPath & parent, const SQChar * name)
{
    return SysPath(parent, name);
}

// ------------------------------------------------------------------------------------------------
SysPath SysPath::MakeUnix(const SQChar * path)
{
    return SysPath(path, Style::Unix);
}

// ------------------------------------------------------------------------------------------------
SysPath SysPath::MakeWindows(const SQChar * path)
{
    return SysPath(path, Style::Windows);
}

// ------------------------------------------------------------------------------------------------
SysPath SysPath::MakeNative(const SQChar * path)
{
    return SysPath(path, Style::Native);
}

// ------------------------------------------------------------------------------------------------
SysPath SysPath::MakeGuess(const SQChar * path)
{
    return SysPath(path, Style::Guess);
}

// ------------------------------------------------------------------------------------------------
SysPath SysPath::MakeDynamic(const SQChar * path)
{
    return SysPath(path, Style::Dynamic);
}

// ------------------------------------------------------------------------------------------------
String SysPath::NormalizePath(SQInteger s, StackStrF & val)
{
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.Proc(true)))
    {
        STHROWLASTF("Invalid string");
    }
    // Is the string empty?
    else if (!val.mLen)
    {
        return String();
    }
    // Turn it into a string that we can edit
    String str(val.mPtr, static_cast< size_t >(val.mLen));
    // Replace all occurrences of the specified character
    for (String::reference c : str)
    {
        if (c == '/' || c == '\\')
        {
            c = static_cast< String::value_type >(s);
        }
    }
    // Return the new string
    return str;
}
// ================================================================================================
void Register_SysPath(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(Typename::Str,
        Class< SysPath >(vm, Typename::Str)
        // Constructors
        .Ctor()
        .Ctor< const SQChar * >()
        .Ctor< const SQChar *, int32_t >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
        .Func(_SC("_tostring"), &SysPath::ToString)
        .Func(_SC("cmp"), &SysPath::Cmp)
        // Properties
        .Prop(_SC("String"), &SysPath::ToString, &SysPath::FromString)
        .Prop(_SC("IsAbs"), &SysPath::IsAbsolute)
        .Prop(_SC("IsAbsolute"), &SysPath::IsAbsolute)
        .Prop(_SC("IsRel"), &SysPath::IsRelative)
        .Prop(_SC("IsRelative"), &SysPath::IsRelative)
        .Prop(_SC("IsDir"), &SysPath::IsDirectory)
        .Prop(_SC("IsDirectory"), &SysPath::IsDirectory)
        .Prop(_SC("IsFile"), &SysPath::IsFile)
        .Prop(_SC("IsEmpty"), &SysPath::Empty)
        .Prop(_SC("Drive"), &SysPath::GetDrive, &SysPath::SetDrive)
        .Prop(_SC("Depth"), &SysPath::Depth)
        .Prop(_SC("Filename"), &SysPath::GetFilename, &SysPath::SqSetFilename)
        .Prop(_SC("Basename"), &SysPath::GetBasename, &SysPath::SqSetBasename)
        .Prop(_SC("Extension"), &SysPath::GetExtensionC, &SysPath::SqSetExtension)
        .Prop(_SC("Parent"), &SysPath::Parent)
        // Member Methods
        .Func(_SC("Swap"), &SysPath::Swap)
        .Func(_SC("Clear"), &SysPath::Clear)
        .Func< SysPath & (SysPath::*)(const SysPath &) >(_SC("AssignPath"), &SysPath::Assign)
        .Func< SysPath & (SysPath::*)(const SysPath &) >(_SC("AppendPath"), &SysPath::Append)
        .Func(_SC("Build"), &SysPath::ToStr)
        .Func(_SC("ToStr"), &SysPath::ToStr)
        .Func(_SC("MakeDir"), &SysPath::MakeDirectory)
        .Func(_SC("MakeDirectory"), &SysPath::MakeDirectory)
        .Func(_SC("MakeFile"), &SysPath::MakeFile)
        .Func(_SC("MakeParent"), &SysPath::MakeParent)
        .Func(_SC("Dir"), &SysPath::Directory)
        .Func(_SC("Directory"), &SysPath::Directory)
        .Func< SysPath & (SysPath::*)(const SQChar *) >(_SC("Push"), &SysPath::Push)
        .Func(_SC("PopBack"), &SysPath::PopBack)
        .Func(_SC("PopFront"), &SysPath::PopFront)
        .Func< SysPath & (SysPath::*)(const SQChar *) >(_SC("SetFilename"), &SysPath::SetFilename)
        .Func(_SC("GetFilename"), &SysPath::GetFilename)
        .Func< SysPath & (SysPath::*)(const SQChar *) >(_SC("SetBasename"), &SysPath::SetBasename)
        .Func(_SC("GetBasename"), &SysPath::GetBasename)
        .Func< SysPath & (SysPath::*)(const SQChar *) >(_SC("SetExtension"), &SysPath::SetExtension)
        .Func(_SC("GetExtension"), &SysPath::GetExtension)
        .Func(_SC("Resolve"), &SysPath::Resolve)
        // Member Overloads
        .Overload< SysPath & (SysPath::*)(const SQChar *) >(_SC("Assign"), &SysPath::Assign)
        .Overload< SysPath & (SysPath::*)(const SQChar *, int32_t) >(_SC("Assign"), &SysPath::Assign)
        .Overload< SysPath & (SysPath::*)(const SQChar *) >(_SC("AssignDir"), &SysPath::AssignDir)
        .Overload< SysPath & (SysPath::*)(const SQChar *, int32_t) >(_SC("AssignDir"), &SysPath::AssignDir)
        .Overload< SysPath & (SysPath::*)(const SQChar *) >(_SC("Append"), &SysPath::Append)
        .Overload< SysPath & (SysPath::*)(const SQChar *, int32_t) >(_SC("Append"), &SysPath::Append)
        .Overload< SysPath & (SysPath::*)(void) >(_SC("MakeAbsolute"), &SysPath::MakeAbsolute)
        .Overload< SysPath & (SysPath::*)(const SysPath &) >(_SC("MakeAbsolute"), &SysPath::MakeAbsolute)
        // Static Functions
        .StaticFunc(_SC("Separator"), &SysPath::Separator)
        .StaticFunc(_SC("PathSeparator"), &SysPath::PathSeparator)
        .StaticFunc< SysPath (*)(const SQChar *) >(_SC("Expand"), &SysPath::Expand)
        .StaticFunc(_SC("Home"), &SysPath::Home)
        .StaticFunc(_SC("ConfigHome"), &SysPath::ConfigHome)
        .StaticFunc(_SC("DataHome"), &SysPath::DataHome)
        .StaticFunc(_SC("TempHome"), &SysPath::TempHome)
        .StaticFunc(_SC("CacheHome"), &SysPath::CacheHome)
        .StaticFunc(_SC("Current"), &SysPath::Working)
        .StaticFunc(_SC("Working"), &SysPath::Working)
        .StaticFunc(_SC("Temp"), &SysPath::Temp)
        .StaticFunc(_SC("Config"), &SysPath::Config)
        .StaticFunc(_SC("System"), &SysPath::System)
        .StaticFunc(_SC("Null"), &SysPath::Null)
        .StaticFunc(_SC("Real"), &SysPath::Real)
        .StaticFunc(_SC("With"), &SysPath::With)
        .StaticFunc(_SC("Unix"), &SysPath::MakeUnix)
        .StaticFunc(_SC("Windows"), &SysPath::MakeWindows)
        .StaticFunc(_SC("Native"), &SysPath::MakeNative)
        .StaticFunc(_SC("Guess"), &SysPath::MakeGuess)
        .StaticFunc(_SC("Dynamic"), &SysPath::MakeDynamic)
        .StaticFmtFunc(_SC("Normalize"), &SysPath::NormalizePath)
        // Static Overloads
        .StaticOverload< SysPath (*)(const SQChar *) >(_SC("ForDir"), &SysPath::ForDirectory)
        .StaticOverload< SysPath (*)(const SQChar *, int32_t) >(_SC("ForDir"), &SysPath::ForDirectory)
        .StaticOverload< SysPath (*)(const SQChar *) >(_SC("ForDirectory"), &SysPath::ForDirectory)
        .StaticOverload< SysPath (*)(const SQChar *, int32_t) >(_SC("ForDirectory"), &SysPath::ForDirectory)
    );

    ConstTable(vm).Enum(_SC("SqSysPathStyle"), Enumeration(vm)
        .Const(_SC("Unix"),                     static_cast< int32_t >(SysPath::Style::Unix))
        .Const(_SC("Windows"),                  static_cast< int32_t >(SysPath::Style::Windows))
        .Const(_SC("Native"),                   static_cast< int32_t >(SysPath::Style::Native))
        .Const(_SC("Guess"),                    static_cast< int32_t >(SysPath::Style::Guess))
        .Const(_SC("Dynamic"),                  static_cast< int32_t >(SysPath::Style::Dynamic))
    );
}

} // Namespace:: SqMod
