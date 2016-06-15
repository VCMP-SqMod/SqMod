#ifndef _SQSQLITE_HANDLE_CONNECTION_HPP_
#define _SQSQLITE_HANDLE_CONNECTION_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
#include <vector>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * The structure that holds the data associated with a certain connection.
*/
struct ConnHnd
{
public:

    // --------------------------------------------------------------------------------------------
    typedef sqlite3         Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*           Pointer; // Pointer to the managed type.
    typedef const Type*     ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&           Reference; // Reference to the managed type.
    typedef const Type&     ConstRef; // Constant reference to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef std::vector< String > QueryList; // Container used to queue queries.

public:

    // --------------------------------------------------------------------------------------------
    Pointer     mPtr; // The connection handle resource.

    // --------------------------------------------------------------------------------------------
    Int32       mStatus; // The last status code of this connection handle.

    // --------------------------------------------------------------------------------------------
    QueryList   mQueue; // A queue of queries to be executed in groups.

    // --------------------------------------------------------------------------------------------
    Int32       mFlags; // The flags used to create the database connection handle.
    String      mName; // The specified name to be used as the database file.
    String      mVFS; // The specified virtual file system.

    // --------------------------------------------------------------------------------------------
    bool        mMemory; // Whether the database exists in memory and not disk.
    bool        mTrace; // Whether tracing was activated on the database.
    bool        mProfile; // Whether profiling was activated on the database.

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    ConnHnd();

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    ConnHnd(const ConnHnd & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    ConnHnd(ConnHnd && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~ConnHnd();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    ConnHnd & operator = (const ConnHnd & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    ConnHnd & operator = (ConnHnd && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Create the database connection resource.
    */
    void Create(CSStr name, Int32 flags, CSStr vfs);

    /* --------------------------------------------------------------------------------------------
     * Execute a specific amount of queries from the queue.
    */
    Int32 Flush(Uint32 num, Object & env, Function & func);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the message of the last received error code.
    */
    CCStr ErrStr() const
    {
        return sqlite3_errstr(sqlite3_errcode(mPtr));
    }

    /* --------------------------------------------------------------------------------------------
     * Return the last error message associated with this database connection.
    */
    CCStr ErrMsg() const
    {
        return sqlite3_errmsg(mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Return the numeric result code for the most recent failed API call (if any).
    */
    Int32 ErrNo() const
    {
        return sqlite3_errcode(mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Return the extended numeric result code for the most recent failed API call (if any).
    */
    Int32 ExErrNo() const
    {
        return sqlite3_extended_errcode(mPtr);
    }
};

} // Namespace:: SqMod

#endif // _SQSQLITE_HANDLE_CONNECTION_HPP_
