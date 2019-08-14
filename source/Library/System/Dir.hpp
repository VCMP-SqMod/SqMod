#ifndef _LIBRARY_SYSDIR_HPP_
#define _LIBRARY_SYSDIR_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
#include <memory>

// ------------------------------------------------------------------------------------------------
#include <tinydir.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
typedef std::unique_ptr< tinydir_dir > TinyDir;
typedef std::unique_ptr< tinydir_file > TinyFile;

/* ------------------------------------------------------------------------------------------------
 * This class represents file-system directories in a platform-independent manner.
*/
class SysDir
{
    // --------------------------------------------------------------------------------------------
    TinyDir mHandle; /* Handle to the managed directory. */

public:

    /* --------------------------------------------------------------------------------------------
     * Make sure a valid handle is being managed before attempting to use it.
    */
    void Validate() const
    {
        if (!mHandle)
        {
            STHROWF("Invalid directory handle. Please open a directory first.");
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure a valid handle is being managed before attempting to use it.
    */
    void Validate(CSStr action) const
    {
        if (!mHandle)
        {
            STHROWF("Cannot %s. Invalid directory handle.", action);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Defaults to a null handle.
    */
    SysDir()
        : mHandle()
    {
        /*...*/
    }

    /* --------------------------------------------------------------------------------------------
     * Opens the directory at the specified path.
    */
    SysDir(StackStrF & path)
        : SysDir(false, path)
    {
        /*...*/
    }

    /* --------------------------------------------------------------------------------------------
     * Construct from an existing file handle.
    */
    explicit SysDir(tinydir_dir * handle)
        : mHandle(handle)
    {
        /*...*/
    }

    /* --------------------------------------------------------------------------------------------
     * Opens the directory at the specified path.
    */
    SysDir(bool sorted, StackStrF & path)
        : SysDir()
    {
        // Should we open this in sorted mode?
        if (sorted)
        {
            OpenSorted(path);
        }
        else
        {
            Open(path);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    SysDir(const SysDir &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    SysDir(SysDir && o)
        : mHandle(std::forward< TinyDir >(o.mHandle))
    {
        /*...*/
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~SysDir()
    {
        // Is there handle being managed?
        if (mHandle)
        {
            tinydir_close(mHandle.get()); // Close it!
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    SysDir & operator = (const SysDir &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    SysDir & operator = (SysDir && o)
    {
        // Avoid self assignment
        if (this != &o)
        {
            // Is there handle being managed?
            if (mHandle)
            {
                tinydir_close(mHandle.get()); // Close it!
            }
            // Take ownership of the new handle
            mHandle = std::move(o.mHandle);
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw managed handle.
    */
    tinydir_dir * Get() const
    {
        return mHandle.get();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw managed handle and make one if it doesn't exist already.
    */
    tinydir_dir * GetOrMake()
    {
        // Do we have a handle already?
        if (!mHandle)
        {
            mHandle = std::make_unique< tinydir_dir >(); // Make one
        }
        // Return it like we promised
        return mHandle.get();
    }

    /* --------------------------------------------------------------------------------------------
     * Take ownership of the managed handle.
    */
    tinydir_dir * Release()
    {
        return mHandle.release();
    }

    /* --------------------------------------------------------------------------------------------
     * Release the managed handle.
    */
    void Reset()
    {
        mHandle.reset();
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return mHandle ? mHandle->path : _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * Check for the presence of a handle.
    */
    bool IsValid() const
    {
        return !!mHandle;
    }

    /* --------------------------------------------------------------------------------------------
     * Open a handle to the directory at the specified path.
    */
    void Open(StackStrF & path)
    {
        // Get the string from the script
        if ((SQ_FAILED(path.Proc(true))))
        {
            STHROWF("Unable to extract the specified path.");
        }
        // Allocate handle memory, if necessary
        if (!mHandle)
        {
            mHandle = std::make_unique< tinydir_dir >();
        }
        // If there was a handle open, we close it
        // If we just allocated one, we initialize it (win, either way)
        tinydir_close(mHandle.get());
        // Attempt to open the specified directory
        if (tinydir_open(mHandle.get(), path.mPtr) == -1)
        {
            // Don't keep a bad handle
            mHandle.reset();
            // Now we can throw the exception
            STHROWLASTF("Failed to open directory: %s", path.mPtr);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Open a handle to the directory at the specified path.
    */
    void OpenSorted(StackStrF & path)
    {
        // Get the string from the script
        if ((SQ_FAILED(path.Proc(true))))
        {
            STHROWF("Unable to extract the specified path.");
        }
        // Allocate handle memory, if necessary
        if (!mHandle)
        {
            mHandle = std::make_unique< tinydir_dir >();
        }
        // If there was a handle open, we close it
        // If we just allocated one, we initialize it (win, either way)
        tinydir_close(mHandle.get());
        // Attempt to open the specified directory
        if (tinydir_open_sorted(mHandle.get(), path.mPtr) == -1)
        {
            // Don't keep a bad handle
            mHandle.reset();
            // Now we can throw the exception
            STHROWLASTF("Failed to open directory: %s", path.mPtr);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Open a handle to the directory at the specified path.
    */
    SysDir & OpenSubDir(SQInteger i)
    {
        Validate("open sub directory");
        // Discard current error number
        errno = 0;
        // Make sure the specified directory index is valid
        if (i < 0)
        {
            STHROWF("Directory index (" PRINT_INT_FMT " < 0) our of bounds.", i);
        }
        if (static_cast< size_t >(i) >= mHandle->n_files)
        {
            STHROWF("Directory index (" PRINT_INT_FMT " >= " PRINT_SZ_FMT ") our of bounds.", i, mHandle->n_files);
        }
        // Make sure there is a directory at the specified index
        else if (!mHandle->_files[i].is_dir)
        {
            STHROWF("The specified index (" PRINT_INT_FMT ") is not a directory.", i);
        }
        // Attempt to open the specified sub-directory
        if (tinydir_open_subdir_n(mHandle.get(), static_cast< size_t >(i)) == -1)
        {
            // Don't keep a bad handle
            mHandle.reset();
            // Now we can throw the exception
            STHROWLASTF("Failed to open sub directory (" PRINT_INT_FMT ").", i);
        }
        // Return self to allow chaining
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Advance to the next element in the opened directory.
    */
    void Next()
    {
        Validate("advance to next element");
        // See if there is a next element
        if (!mHandle->has_next)
        {
            STHROWF("Nothing left to advance to.");
        }
        // Discard current error number
        errno = 0;
        // Perform the requested action
        if (tinydir_next(mHandle.get()) == -1)
        {
            // This particular error number means the directory was closed
            if (errno == EIO) mHandle.reset();
            // Now the exception can be thrown
            STHROWLASTF("Failed to advance to the next element");
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Close the currently associated directory handle.
    */
    void Close()
    {
        Validate("close directory");
        // Perform the requested action
        tinydir_close(mHandle.get());
        // Release any associated memory
        mHandle.reset();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the opened path.
    */
    CSStr GetPath() const
    {
        Validate("obtain path");
        // Return the requested information
        return mHandle->path;
    }

    /* --------------------------------------------------------------------------------------------
     * See if there's a next element in the opened directory.
    */
    bool HasNext() const
    {
        Validate("check for next");
        // Return the requested information
        return static_cast< bool >(mHandle->has_next);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of files in the opened directory (only when opened in sorted mode).
    */
    SQInteger FileCount() const
    {
        Validate("obtain file count");
        // Return the requested information
        return static_cast< SQInteger >(mHandle->n_files);
    }

    /* --------------------------------------------------------------------------------------------
     * Open current file from the specified directory.
    */
    LightObj ReadFile() const;
    /* --------------------------------------------------------------------------------------------
     * Open current file from the specified directory.
    */
    LightObj ReadFileAt(SQInteger i) const;
};

/* ------------------------------------------------------------------------------------------------
 * This class represents file-system files in a platform-independent manner.
*/
class SysFile
{
    // --------------------------------------------------------------------------------------------
    TinyFile mHandle; /* Handle to the managed file. */

public:
    /* --------------------------------------------------------------------------------------------
     * Make sure a valid handle is being managed before attempting to use it.
    */
    void Validate() const
    {
        if (!mHandle)
        {
            STHROWF("Invalid file handle. Please open a file first.");
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Make sure a valid handle is being managed before attempting to use it.
    */
    void Validate(CSStr action) const
    {
        if (!mHandle)
        {
            STHROWF("Cannot %s. Invalid file handle.", action);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Defaults to a null handle.
    */
    SysFile()
        : mHandle()
    {
        /*...*/
    }

    /* --------------------------------------------------------------------------------------------
     * Opens the file at the specified path.
    */
    SysFile(StackStrF & path)
        : SysFile()
    {
        Open(path);
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    SysFile(const SysFile &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    SysFile(SysFile && o)
        : mHandle(std::forward< TinyFile >(o.mHandle))
    {
        /*...*/
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    SysFile & operator = (const SysFile &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    SysFile & operator = (SysFile && o)
    {
        // Avoid self assignment
        if (this != &o)
        {
            // Take ownership of the new handle
            mHandle = std::move(o.mHandle);
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw managed handle.
    */
    tinydir_file * Get() const
    {
        return mHandle.get();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw managed handle and make one if it doesn't exist already.
    */
    tinydir_file * GetOrMake()
    {
        // Do we have a handle already?
        if (!mHandle)
        {
            mHandle = std::make_unique< tinydir_file >(); // Make one
        }
        // Return it like we promised
        return mHandle.get();
    }

    /* --------------------------------------------------------------------------------------------
     * Take ownership of the managed handle.
    */
    tinydir_file * Release()
    {
        return mHandle.release();
    }

    /* --------------------------------------------------------------------------------------------
     * Release the managed handle.
    */
    void Reset()
    {
        mHandle.reset();
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return mHandle ? mHandle->path : _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * Check for the presence of a handle.
    */
    bool IsValid() const
    {
        return !!mHandle;
    }

    /* --------------------------------------------------------------------------------------------
     * Open a handle to the file at the specified path.
    */
    void Open(StackStrF & path)
    {
        // Get the string from the script
        if ((SQ_FAILED(path.Proc(true))))
        {
            STHROWF("Unable to extract the specified path.");
        }
        // Allocate the handle memory
        mHandle = std::make_unique< tinydir_file >();
        // Discard current error number
        errno = 0;
        // Attempt to open the specified file
        if (tinydir_file_open(mHandle.get(), path.mPtr) == -1)
        {
            // Don't keep a bad handle
            mHandle.reset();
            // Now we can throw the exception
            if (errno != 0)
            {
                STHROWF("Failed to open file: %s [%s]", path.mPtr, strerror(errno));
            }
            else
            {
                STHROWLASTF("Failed to open file: %s", path.mPtr);
            }
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Check if the opened element is a directory.
    */
    bool IsDir() const
    {
        Validate("check type");
        // Return the requested information
        return static_cast< bool >(mHandle->is_dir);
    }

    /* --------------------------------------------------------------------------------------------
     * Check if the opened element is a regular file.
    */
    bool IsReg() const
    {
        Validate("check type");
        // Return the requested information
        return static_cast< bool >(mHandle->is_reg);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the path of the opened file.
    */
    CSStr GetPath() const
    {
        Validate("retrieve path");
        // Return the requested information
        return mHandle->path;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the name of the opened file.
    */
    CSStr GetName() const
    {
        Validate("retrieve name");
        // Return the requested information
        return mHandle->name;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the extension of the opened file.
    */
    CSStr GetExtension() const
    {
        Validate("retrieve extension");
        // Return the requested information
        return mHandle->extension != nullptr ? mHandle->extension : _SC("");
    }
};



} // Namespace:: SqMod

#endif // _LIBRARY_SYSDIR_HPP_
