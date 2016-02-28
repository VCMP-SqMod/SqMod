// ------------------------------------------------------------------------------------------------
#include "Common.hpp"
#include "Module.hpp"

// ------------------------------------------------------------------------------------------------
#include <ctype.h>
#include <stdarg.h>

// ------------------------------------------------------------------------------------------------
#include <sqrat.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
static SQChar g_Buffer[4096]; // Common buffer to reduce memory allocations.

// ------------------------------------------------------------------------------------------------
SStr GetTempBuff()
{
    return g_Buffer;
}

// ------------------------------------------------------------------------------------------------
Uint32 GetTempBuffSize()
{
    return sizeof(g_Buffer);
}

// ------------------------------------------------------------------------------------------------
void SqThrowF(CSStr str, ...)
{
    // Initialize the argument list
    va_list args;
    va_start (args, str);
    // Write the requested contents
    if (snprintf(g_Buffer, sizeof(g_Buffer), str, args) < 0)
        strcpy(g_Buffer, "Unknown error has occurred");
    // Release the argument list
    va_end(args);
    // Throw the exception with the resulted message
    throw Sqrat::Exception(g_Buffer);
}

// ------------------------------------------------------------------------------------------------
CSStr FmtStr(CSStr str, ...)
{
    // Initialize the argument list
    va_list args;
    va_start (args, str);
    // Write the requested contents
    if (snprintf(g_Buffer, sizeof(g_Buffer), str, args) < 0)
        g_Buffer[0] = 0; /* make sure the string is terminated */
    // Release the argument list
    va_end(args);
    // Return the data from the buffer
    return g_Buffer;
}

// ------------------------------------------------------------------------------------------------
CSStr QFmtStr(CSStr str, ...)
{
    // Initialize the argument list
    va_list args;
    va_start (args, str);
    // Write the requested contents
    sqlite3_vsnprintf(sizeof(g_Buffer), g_Buffer, str, args);
    // Release the argument list
    va_end(args);
    // Return the data from the buffer
    return g_Buffer;
}

// ------------------------------------------------------------------------------------------------
bool IsQueryEmpty(CSStr str)
{
    // Is the pointer valid?
    if (!str)
        return true;
    // Currently processed character
    SQChar c = 0;
    // See if the query contains any alpha numeric characters
    while ((c = *str) != 0)
    {
        if (isalnum(c) != 0)
            return false;
        ++str;
    }
    // At this point we consider the query empty
    return true;
}

// ------------------------------------------------------------------------------------------------
StackGuard::StackGuard(HSQUIRRELVM vm)
    : m_Top(sq_gettop(vm)), m_VM(vm)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
StackGuard::~StackGuard()
{
    sq_pop(m_VM, sq_gettop(m_VM) - m_Top);
}

// ------------------------------------------------------------------------------------------------
ConnHnd::Handle::~Handle()
{
    // Is there anything to close?
    if (!mPtr)
        return; // Nothing to close
    // Are we dealing with a memory leak? Technically shouldn't reach this situation!
    else if (mRef != 0)
        // Should we deal with undefined behavior instead? How bad is one connection left open?
        _SqMod->LogErr("SQLite connection is still referenced (%s)", mName.c_str());
    else
    {
        // NOTE: Should we call sqlite3_interrupt(...) before closing?
        Object env;
        Function func;
        // Flush remaining queries in the queue and ignore the result
        Flush(mQueue.size(), env, func);
        // Attempt to close the database
        if ((sqlite3_close(mPtr)) != SQLITE_OK)
            _SqMod->LogErr("Unable to close SQLite connection [%s]", sqlite3_errmsg(mPtr));
    }
}

// ------------------------------------------------------------------------------------------------
void ConnHnd::Handle::Create(CSStr name, Int32 flags, CSStr vfs)
{
    // Make sure a previous connection doesn't exist
    if (mPtr)
        SqThrowF("Unable to connect to database. Database already connected");
    // Make sure the name is valid
    else if (!name || strlen(name) <= 0)
        SqThrowF("Unable to connect to database. The name is invalid");
    // Attempt to create the database connection
    else if ((mStatus = sqlite3_open_v2(name, &mPtr, flags, vfs)) != SQLITE_OK)
    {
        // Must be destroyed regardless of result
        sqlite3_close(mPtr);
        // Explicitly make sure it's null
        mPtr = NULL;
        // Now its safe to throw the error
        SqThrowF("Unable to connect to database [%s]", sqlite3_errstr(mStatus));
    }
    // Let's save the specified information
    mName.assign(name);
    mFlags = flags;
    mVFS.assign(vfs ? vfs : _SC(""));
    // Optional check if database is initially stored in memory
    mMemory = (mName.compare(_SC(":memory:")) == 0);
}

// ------------------------------------------------------------------------------------------------
Int32 ConnHnd::Handle::Flush(Uint32 num, Object & env, Function & func)
{
    // Do we even have a valid connection?
    if (!mPtr)
        return -1;
    // Is there anything to flush?
    else if (!num || mQueue.empty())
        return 0;
    // Can we even flush that many?
    else if (num > mQueue.size())
        num = mQueue.size();
    // Generate the function that should be called upon error
    Function callback = Function(env.GetVM(), env.GetObject(), func.GetFunc());
    // Obtain iterators to the range of queries that should be flushed
    QueryList::iterator itr = mQueue.begin();
    QueryList::iterator end = mQueue.begin() + num;
    // Attempt to begin the flush transaction
    if ((mStatus = sqlite3_exec(mPtr, "BEGIN", NULL, NULL, NULL)) != SQLITE_OK)
        SqThrowF("Unable to begin transaction [%s]", sqlite3_errmsg(mPtr));
    // Process all queries within range of selection
    for (; itr != end; ++itr)
    {
        // Should we manually terminate this query?
        /*
        if (*(*itr).rbegin() != ';')
            itr->push_back(';');
        */
        // Attempt to execute the currently processed query string
        if ((mStatus = sqlite3_exec(mPtr, itr->c_str(), NULL, NULL, NULL)) == SQLITE_OK)
            continue;
        // Do we have to execute any callback to resolve our issue?
        else if (!callback.IsNull())
        {
            // Ask the callback whether the query processing should end here
            SharedPtr< bool > ret = callback.Evaluate< bool, Int32, CSStr >(mStatus, itr->c_str());
            // Should we break here?
            if (!!ret && (*ret == false))
                break;
        }
    }
    // Erase all queries till end or till the point of failure (if any occurred)
    mQueue.erase(mQueue.begin(), itr);
    // Attempt to commit changes requested during transaction
    if ((mStatus = sqlite3_exec(mPtr, "COMMIT", NULL, NULL, NULL)) == SQLITE_OK)
        return sqlite3_changes(mPtr);
    // Attempt to roll back erroneous changes
    else if ((mStatus = sqlite3_exec(mPtr, "ROLLBACK", NULL, NULL, NULL)) != SQLITE_OK)
        SqThrowF("Unable to rollback transaction [%s]", sqlite3_errmsg(mPtr));
    // The transaction failed somehow but we managed to rollback
    else
        SqThrowF("Unable to commit transaction because [%s]", sqlite3_errmsg(mPtr));
    // Operation failed
    return -1;
}

// ------------------------------------------------------------------------------------------------
StmtHnd::Handle::~Handle()
{
    // Is there anything to finalize?
    if (!mPtr)
        return; // Nothing to finalize
    // Are we dealing with a memory leak? Technically shouldn't reach this situation!
    else if (mRef != 0)
        // Should we deal with undefined behavior instead? How bad is one statement left alive?
        _SqMod->LogErr("SQLite statement is still referenced (%s)", mQuery.c_str());
    else
    {
        // Attempt to finalize the statement
        if ((sqlite3_finalize(mPtr)) != SQLITE_OK)
            _SqMod->LogErr("Unable to finalize SQLite statement [%s]", mConn.ErrMsg());
    }
}

// ------------------------------------------------------------------------------------------------
void StmtHnd::Handle::Create(CSStr query)
{
    // Make sure a previous statement doesn't exist
    if (mPtr)
        SqThrowF("Unable to prepare statement. Statement already prepared");
    // Is the specified database connection is valid?
    else if (!mConn)
        SqThrowF("Unable to prepare statement. Invalid connection handle");
    // Save the query string and therefore multiple strlen(...) calls
    mQuery.assign(query ? query : _SC(""));
    // Is the specified query string we just saved, valid?
    if (mQuery.empty())
        SqThrowF("Unable to prepare statement. Invalid query string");
    // Attempt to prepare a statement with the specified query string
    else if ((mStatus = sqlite3_prepare_v2(mConn, mQuery.c_str(), (Int32)mQuery.size(),
                                            &mPtr, NULL)) != SQLITE_OK)
    {
        // Clear the query string since it failed
        mQuery.clear();
        // Explicitly make sure the handle is null
        mPtr = NULL;
        // Now it's safe to throw the error
        SqThrowF("Unable to prepare statement [%s]", mConn.ErrMsg());
    }
    else
        // Obtain the number of available columns
        mColumns = sqlite3_column_count(mPtr);
}

// ------------------------------------------------------------------------------------------------
Int32 StmtHnd::Handle::GetColumnIndex(CSStr name)
{
    // Validate the handle
    if (!mPtr)
        SqThrowF("Invalid SQLite statement");
    // Are the names cached?
    else if (mIndexes.empty())
    {
        for (Int32 i = 0; i < mColumns; ++i)
        {
            // Get the column name at the current index
            CSStr name = (CSStr)sqlite3_column_name(mPtr, i);
            // Validate the name
            if (!name)
                SqThrowF("Unable to retrieve column name for index (%d)", i);
            // Save it to guarantee the same lifetime as this instance
            else
                mIndexes[name] = i;
        }
    }
    // Attempt to find the specified column
    const Indexes::iterator itr = mIndexes.find(name);
    // Was there a column with the specified name?
    if (itr != mIndexes.end())
        return itr->second;
    // No such column exists (expecting the invoker to validate the result)
    return -1;
}

// ------------------------------------------------------------------------------------------------
CSStr GetErrStr(Int32 status)
{
    return sqlite3_errstr(status);
}

// ------------------------------------------------------------------------------------------------
void SetSoftHeapLimit(Int32 limit)
{
    sqlite3_soft_heap_limit(limit);
}

// ------------------------------------------------------------------------------------------------
Int32 ReleaseMemory(Int32 bytes)
{
    return sqlite3_release_memory(bytes);
}

// ------------------------------------------------------------------------------------------------
Object GetMemoryUsage()
{
    // Obtain the initial stack size
    const StackGuard sg(_SqVM);
    // Push a long integer instance with the requested value on the stack
    _SqMod->PushSLongObject(_SqVM, sqlite3_memory_used());
    // Obtain the object from the stack and return it
    return Var< Object >(_SqVM, -1).value;
}

// ------------------------------------------------------------------------------------------------
Object GetMemoryHighwaterMark(bool reset)
{
    // Obtain the initial stack size
    const StackGuard sg(_SqVM);
    // Push a long integer instance with the requested value on the stack
    _SqMod->PushSLongObject(_SqVM, sqlite3_memory_highwater(reset));
    // Obtain the object from the stack and return it
    return Var< Object >(_SqVM, -1).value;
}

// ------------------------------------------------------------------------------------------------
CSStr EscapeString(CSStr str)
{
    // Is there even a string to escape?
    if (!str)
        return _SC(""); // Default to empty string
    // Attempt to escape the specified string
    sqlite3_snprintf(sizeof(g_Buffer), g_Buffer, "%q", str);
    // Return the resulted string
    return g_Buffer;
}

// ------------------------------------------------------------------------------------------------
CCStr EscapeStringEx(SQChar spec, CCStr str)
{
    // Utility that allows changing the format specifier temporarily
    static SQChar fs[] = _SC("%q");
    // Validate the specified format specifier
    if (spec != 'q' && spec != 'Q' && spec != 'w' && spec != 's')
    {
        SqThrowF("Unknown format specifier: %c", spec);
        // Default to empty string
        return _SC("");
    }
    // Is there even a string to escape?
    else if (!str)
        return _SC(""); // Default to empty string
    // Apply the format specifier
    fs[1] = spec;
    // Attempt to escape the specified string
    sqlite3_snprintf(sizeof(g_Buffer), g_Buffer, fs, str);
    // Restore the format specifier
    fs[1] = 'q';
    // Return the resulted string
    return g_Buffer;
}

// ------------------------------------------------------------------------------------------------
CCStr ArrayToQueryColumns(Array & arr)
{
    // Do we even have any elements to process?
    if (arr.Length() <= 0)
        return _SC(""); // Default to empty string
    // Allocate a vector with the required amount of column names
    std::vector< String > values(arr.Length());
    // Attempt to extract the array elements as strings
    arr.GetArray< String >(&values[0], values.size());
    // Used to know the position of the next column name
    Uint32 offset = 0;
    // Obtain the start of the array
    std::vector< String >::iterator itr = values.begin();
    // Process all elements within range
    for (; itr != values.end() && offset < sizeof(g_Buffer); ++itr)
    {
        // Is the name valid?
        if (itr->empty())
            SqThrowF("Invalid column name");
        // Attempt to append the column name to the buffer
        sqlite3_snprintf(sizeof(g_Buffer) - offset, g_Buffer + offset, "[%q], ", itr->c_str());
        // Add the column name size to the offset
        offset += itr->size();
        // Also include the comma and space in the offset
        offset += 2;
    }
    // Trim the last coma and space
    if (offset >= 2)
        g_Buffer[offset-2] = 0;
    else
        g_Buffer[0] = 0;
    // Return the resulted string
    return g_Buffer;
}

// ------------------------------------------------------------------------------------------------
CCStr TableToQueryColumns(Table & tbl)
{
    // Used to know the position of the next column name
    Uint32 offset = 0;
    // Used to obtain the column name temporarily
    String name;
    // Obtain the start of the table
    Table::iterator itr;
    // Process all elements within range
    while (tbl.Next(itr))
    {
        // Use the element key as the column name
        name.assign(itr.getName());
        // Is the name valid?
        if (name.empty())
            SqThrowF("Invalid or empty column name");
        // Attempt to append the column name to the buffer
        sqlite3_snprintf(sizeof(g_Buffer) - offset, g_Buffer + offset, "[%q], ", name.c_str());
        // Add the column name size to the offset
        offset += name.size();
        // Also include the comma and space in the offset
        offset += 2;
    }
    // Trim the last coma and space
    if (offset >= 2)
        g_Buffer[offset-2] = 0;
    else
        g_Buffer[0] = 0;
    // Return the resulted string
    return g_Buffer;
}

} // Namespace:: SqMod
