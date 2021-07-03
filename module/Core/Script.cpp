// ------------------------------------------------------------------------------------------------
#include "Core/Script.hpp"

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
    explicit FileHandle(const SQChar * path)
        : mFile(std::fopen(path, "rb"))
    {
        if (!mFile)
        {
            STHROWF("Unable to open script source ({})", path);
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
    operator std::FILE * () const // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
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
    // Go to the end of the file
    std::fseek(fp, 0, SEEK_END);
    // Calculate buffer size from beginning to current position
    const long length = std::ftell(fp);
    // Go back to the beginning
    std::fseek(fp, 0, SEEK_SET);
    // Read the first 2 bytes of the file and determine the file type
    if ((length >= 2) && (std::fread(&tag, 1, 2, fp) != 2 || tag == SQ_BYTECODE_STREAM_TAG))
    {
        return; // Probably an empty file or compiled script
    }
    // Allocate enough space to hold the file data
    mData.resize(static_cast< size_t >(length), 0);
    // Go back to the beginning
    std::fseek(fp, 0, SEEK_SET);
    // Read the file contents into allocated data
    size_t r = std::fread(&mData[0], 1, static_cast< size_t >(length), fp);
    // Read completely?
    if (r != static_cast< size_t >(length))
    {
        return SqThrowF(fmt::runtime("Failed to read script contents.")); // Not cool
    }
    // Where the last line ended
    size_t line_start = 0, line_end = 0;
    // Process the file data and locate new lines
    for (String::const_iterator itr = mData.cbegin(); itr != mData.cend();)
    {
        // Is this a Unix style line ending?
        if (*itr == '\n')
        {
            // Extract the line length
            line_end = static_cast< size_t >(std::distance(mData.cbegin(), itr));
            // Store the beginning of the line
            mLine.emplace_back(line_start, line_end);
            // Advance to the next line
            line_start = line_end+1;
            // The line end character was not included
            ++itr;
        }
        // Is this a Windows style line ending?
        else if (*itr == '\r')
        {
            if (*(++itr) == '\n')
            {
                // Extract the line length
                line_end = static_cast< size_t >(std::distance(mData.cbegin(), itr) - 1);
                // Store the beginning of the line
                mLine.emplace_back(line_start, line_end);
                // Advance to the next line
                line_start = line_end+2;
                // The line end character was not included
                ++itr;
            }
        }
        else
        {
            ++itr;
        }
    }
    // Should we add the last line as well?
    if (mData.size() - line_start > 0)
    {
        mLine.emplace_back(line_start, mData.size());
    }
    // Specify that this script contains line information
    mInfo = true;
}

// ------------------------------------------------------------------------------------------------
ScriptSrc::ScriptSrc(const String & path, bool delay, bool info) // NOLINT(modernize-pass-by-value)
    : mExec()
    , mPath(path)
    , mData()
    , mLine()
    , mInfo(info)
    , mDelay(delay)
{
    // Is the specified path empty?
    if (mPath.empty())
    {
        throw std::runtime_error("Invalid or empty script path");
    }
    // Should we load the file contents for debugging purposes?
    else if (mInfo)
    {
        Process();
    }
}

// ------------------------------------------------------------------------------------------------
String ScriptSrc::FetchLine(size_t line, bool trim) const
{
    // Do we have such line?
    if (line > mLine.size())
    {
        return String(); // Nope!
    }
    // Grab it's range in the file
    Line::const_reference l =  mLine.at(line);
    // Grab the code from that line
    String code = mData.substr(l.first, l.second - l.first);
    // Trim whitespace from the beginning of the code code
    if (trim)
    {
        code.erase(0, code.find_first_not_of(" \t\n\r\f\v"));
    }
    // Return the resulting string
    return code;
}

} // Namespace::  SqMod
