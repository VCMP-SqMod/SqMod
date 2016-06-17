// ------------------------------------------------------------------------------------------------
#include "Base/ScriptSrc.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <algorithm>
#include <stdexcept>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Helper class to ensure the file handle is closed regardless of the situation.
*/
class FileHandle
{
public:

    // --------------------------------------------------------------------------------------------
    std::FILE * mFile; // Handle to the opened file.

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    FileHandle(CSStr path)
        : mFile(std::fopen(path, "rb"))
    {
        if (!mFile)
        {
            throw std::runtime_error(ToStrF("Unable to open script source (%s)", path));
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    FileHandle(const FileHandle & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    FileHandle(FileHandle && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~FileHandle()
    {
        if (mFile)
        {
            std::fclose(mFile);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    FileHandle & operator = (const FileHandle & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    FileHandle & operator = (FileHandle && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed file handle.
    */
    operator std::FILE * ()
    {
        return mFile;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed file handle.
    */
    operator std::FILE * () const
    {
        return mFile;
    }
};


// ------------------------------------------------------------------------------------------------
void ScriptSrc::Process()
{
    // Attempt to open the specified file
    FileHandle fp(mPath.c_str());
    // First 2 bytes of the file will tell if this is a compiled script
    std::uint16_t tag;
    // Read the first 2 bytes of the file and determine the file type
    if (std::fread(&tag, 1, 2, fp) != 2 || tag == SQ_BYTECODE_STREAM_TAG)
    {
        return; // Probably an empty file or compiled script
    }
    // Go to the end of the file
    std::fseek(fp, 0, SEEK_END);
    // Calculate buffer size from beginning to current position
    const LongI length = std::ftell(fp);
    // Go back to the beginning
    std::fseek(fp, 0, SEEK_SET);
    // Allocate enough space to hold the file data
    mData.resize(length, 0);
    // Read the file contents into allocated data
    std::fread(&mData[0], 1, length, fp);
    // Where the last line ended
    unsigned line = 0;
    // Process the file data and locate new lines
    for (String::const_iterator itr = mData.cbegin(); itr != mData.cend();)
    {
        // Is this a Unix style line ending?
        if (*itr == '\n')
        {
            // Store the beginning of the line
            mLine.push_back(line);
            // Advance to the next line
            line = std::distance(mData.cbegin(), ++itr);
        }
        // Is this a Windows style line ending?
        else if (*itr == '\r')
        {
            if (*(++itr) == '\n')
            {
                // Store the beginning of the line
                mLine.push_back(line);
                // Advance to the next line
                line = std::distance(mData.cbegin(), ++itr);
            }
        }
        else
        {
            ++itr;
        }
    }
    // Should we add the last line as well?
    if (line)
    {
        mLine.push_back(line);
    }
}

// ------------------------------------------------------------------------------------------------
ScriptSrc::ScriptSrc(HSQUIRRELVM vm, const String & path, bool info)
    : mExec(vm)
    , mPath(path)
    , mData()
    , mLine()
{
    // Is the specified path empty?
    if (mPath.empty())
    {
        throw std::runtime_error("Invalid or empty script path");
    }
    // Is the specified virtual machine invalid?
    else if (!vm)
    {
        throw std::runtime_error("Invalid virtual machine pointer");
    }
    // Should we load the file contents for debugging purposes?
    else if (info)
    {
        Process();
    }
}

} // Namespace::  SqMod
