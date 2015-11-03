#ifndef _LIBRARY_SQLITE_SHARED_HPP_
#define _LIBRARY_SQLITE_SHARED_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"
#include "Library/LongInt.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqlite3.h>

// ------------------------------------------------------------------------------------------------
#include <queue>

// ------------------------------------------------------------------------------------------------
namespace SqMod {
namespace SQLite {

// ------------------------------------------------------------------------------------------------
class Connection;
class Statement;
class Result;

/* ------------------------------------------------------------------------------------------------
 * A managed handle to a shared SQLite connection.
*/
class ConnectionHandle
{
    // --------------------------------------------------------------------------------------------
    friend class Connection;

protected:

    /* --------------------------------------------------------------------------------------------
     * Helper class responsible for managing the handle to an SQLite connection resource.
    */
    struct Handle
    {
        friend class ConnectionHandle;

        /* ----------------------------------------------------------------------------------------
         * The type of container used to queue queries for group execution.
        */
        typedef std::queue< String > QueryList;

        /* ----------------------------------------------------------------------------------------
         * The amount of references to this handle instance.
        */
        mutable SQUint32    Ref;

        /* ----------------------------------------------------------------------------------------
         * The handle to the managed SQLite connection resource.
        */
        sqlite3*            Ptr;

        /* ----------------------------------------------------------------------------------------
         * The flags used to create the SQLite connection handle.
        */
        SQInt32             Flags;

        /* ----------------------------------------------------------------------------------------
         * The specified path to be used as the database file.
        */
        String              Path;

        /* ----------------------------------------------------------------------------------------
         * The specified virtual file system.
        */
        String              VFS;

        /* ----------------------------------------------------------------------------------------
         * The last status code of this connection handle.
        */
        SQInt32             Status;

        /* ----------------------------------------------------------------------------------------
         * A queue of queries to be executed in groups in order to reduce lag.
        */
        QueryList           Queue;

        /* ----------------------------------------------------------------------------------------
         * The global tag associated with this resource.
        */
        SqTag               Tag;

        /* ----------------------------------------------------------------------------------------
         * The global data associated with this resource.
        */
        SqObj               Data;

        /* ----------------------------------------------------------------------------------------
         * Whether the database exists in memory and not disk.
        */
        bool                Memory;

        /* ----------------------------------------------------------------------------------------
         * Whether tracing was activated on the database.
        */
        bool                Trace;

        /* ----------------------------------------------------------------------------------------
         * Whether profiling was activated on the database.
        */
        bool                Profile;

    private:

        /* ----------------------------------------------------------------------------------------
         * Handle constructor.
        */
        Handle(const String & path, SQInt32 flags, const String & vfs)
            : Ref(1), Ptr(NULL), Flags(flags), Path(path), VFS(vfs)
            , Status(SQLITE_OK), Queue(), Tag(), Data()
            , Memory(path == ":memory:"), Trace(false), Profile(false)
        {
            /* ... */
        }

        /* ----------------------------------------------------------------------------------------
         * Copy constructor (disabled)
        */
        Handle(const Handle &) = delete;

        /* ----------------------------------------------------------------------------------------
         * Move constructor (disabled)
        */
        Handle(Handle &&) = delete;

        /* ----------------------------------------------------------------------------------------
         * Handle destructor.
        */
        ~Handle()
        {
            // If a valid handle exists then attempt to release it
            if (Ptr != NULL && (Status = sqlite3_close(Ptr)) != SQLITE_OK)
            {
                LogErr("Unable to <close database connection> because : %s", sqlite3_errmsg(Ptr));
            }
        }

        /* ----------------------------------------------------------------------------------------
         * Copy assignment operator (disabled)
        */
        Handle & operator = (const Handle &) = delete;

        /* ----------------------------------------------------------------------------------------
         * Move assignment operator (disabled)
        */
        Handle & operator = (Handle &&) = delete;
    };

    /* --------------------------------------------------------------------------------------------
     * Acquire a strong reference to the resource handle.
    */
    void Grab()
    {
        // If a valid handle exists then grab a reference to it
        if (Hnd != nullptr)
        {
            ++(Hnd->Ref);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Release the handle reference and therefore the handle it self if no more references exist.
    */
    void Drop()
    {
        // If a valid handle exists then delete it if this is the last released reference
        if (Hnd != nullptr && --(Hnd->Ref) == 0)
        {
            delete Hnd;
        }
        // Explicitly make sure the handle is null
        Hnd = nullptr;
    }

public:

    /* --------------------------------------------------------------------------------------------
     * The handle instance that manages the SQLite resource.
    */
    Handle * Hnd;

    /* --------------------------------------------------------------------------------------------
     * Default constructor (invalid).
    */
    ConnectionHandle();

    /* --------------------------------------------------------------------------------------------
     * Open a connection to a database using the specified path, flags and vfs.
    */
    ConnectionHandle(const SQChar * path, SQInt32 flags, const SQChar * vfs);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    ConnectionHandle(const ConnectionHandle & o);

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    ConnectionHandle(ConnectionHandle && o);

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~ConnectionHandle();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    ConnectionHandle & operator = (const ConnectionHandle & o);

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    ConnectionHandle & operator = (ConnectionHandle && o);

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean.
    */
    operator bool () const
    {
        return (Hnd != nullptr) && (Hnd->Ptr != NULL);
    }

    /* --------------------------------------------------------------------------------------------
     * Negation operator.
    */
    operator ! ()  const
    {
        return (Hnd == nullptr) || (Hnd->Ptr == NULL);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to raw SQLite connection handle.
    */
    operator sqlite3 * () const
    {
        return (Hnd != nullptr) ? Hnd->Ptr : NULL;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to integer status code.
    */
    operator SQInt32 () const
    {
        return (Hnd != nullptr) ? Hnd->Status : 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Member of pointer operator.
    */
    Handle * operator -> () const
    {
        return Hnd;
    }

    /* --------------------------------------------------------------------------------------------
     * Indirection operator.
    */
    Handle & operator * () const
    {
        return *Hnd;
    }

    /* --------------------------------------------------------------------------------------------
     * Equality operator.
    */
    bool operator == (const ConnectionHandle & o) const
    {
        return (Hnd == o.Hnd);
    }

    /* --------------------------------------------------------------------------------------------
     * Inequality operator.
    */
    bool operator != (const ConnectionHandle & o) const
    {
        return (Hnd != o.Hnd);
    }

    /* --------------------------------------------------------------------------------------------
     * Reset this handle and release the internal reference.
    */
    void Release()
    {
        Drop();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of active references to this handle.
    */
    SQUint32 Count() const
    {
        return (Hnd != nullptr) ? Hnd->Ref : 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the error message associated with the value in status.
    */
    const SQChar * ErrStr() const
    {
        return sqlite3_errstr(Hnd->Status);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the error message associated with the connection.
    */
    const SQChar * ErrMsg() const
    {
        return sqlite3_errmsg(Hnd->Ptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the error code associated with the connection.
    */
    SQInt32 ErrNo() const
    {
        return sqlite3_errcode(Hnd->Ptr);
    }
};

/* ------------------------------------------------------------------------------------------------
 * RAII encapsulation of a SQLite Transaction.
*/
class Transaction
{
public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Transaction(const Connection & db);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor (disabled).
    */
    Transaction(const Transaction & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor (disabled).
    */
    Transaction(Transaction && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Transaction();

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator (disabled).
    */
    Transaction & operator = (const Transaction & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator (disabled).
    */
    Transaction & operator = (Transaction && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Commit the transaction.
    */
    void Commit();

    /* --------------------------------------------------------------------------------------------
     * See whether the transaction was committed.
    */
    bool Commited() const
    {
        return m_Commited;
    }

protected:

    // --------------------------------------------------------------------------------------------

private:

    /* --------------------------------------------------------------------------------------------
     * The SQLite connection associated with this transaction.
    */
    ConnectionHandle    m_Connection;

    /* --------------------------------------------------------------------------------------------
     * Whether the changes were committed.
    */
    bool                m_Commited;
};

/* ------------------------------------------------------------------------------------------------
 * Places a "soft" limit on the amount of heap memory that may be allocated by SQLite.
*/
void SetSoftHeapLimit(SQInt32 limit);

/* ------------------------------------------------------------------------------------------------
* Attempts to free N bytes of heap memory by deallocating non-essential memory allocations
* held by the database library.
*/
SQInt32 ReleaseMemory(SQInt32 bytes);

/* ------------------------------------------------------------------------------------------------
 * Returns the number of bytes of memory currently outstanding (malloced but not freed).
*/
SLongInt GetMemoryUsage();

/* ------------------------------------------------------------------------------------------------
 * Returns the maximum value of used bytes since the high-water mark was last reset.
*/
SLongInt GetMemoryHighwaterMark(bool reset);

} // Namespace:: SQLite
} // Namespace:: SqMod

#endif // _LIBRARY_SQLITE_SHARED_HPP_
