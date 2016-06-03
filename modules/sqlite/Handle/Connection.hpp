#ifndef _SQSQLITE_HANDLE_CONNECTION_HPP_
#define _SQSQLITE_HANDLE_CONNECTION_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
#include <vector>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Manages a reference counted database connection handle.
*/
class ConnHnd
{
    // --------------------------------------------------------------------------------------------
    friend class Connection;
    friend class Statement;

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
    typedef unsigned int    Counter; // Reference counter type.

    /* --------------------------------------------------------------------------------------------
     * Validate the connection handle and throw an error if invalid.
    */
    void Validate() const;

protected:

    // --------------------------------------------------------------------------------------------
    typedef std::vector< String > QueryList; // Container used to queue queries.

    /* --------------------------------------------------------------------------------------------
     * The structure that holds the data associated with a certain connection.
    */
    struct Handle
    {
        // ----------------------------------------------------------------------------------------
        Pointer     mPtr; // The connection handle resource.
        Counter     mRef; // Reference count to the managed handle.

        // ----------------------------------------------------------------------------------------
        Int32       mStatus; // The last status code of this connection handle.

        // ----------------------------------------------------------------------------------------
        QueryList   mQueue; // A queue of queries to be executed in groups.

        // ----------------------------------------------------------------------------------------
        Int32       mFlags; // The flags used to create the database connection handle.
        String      mName; // The specified name to be used as the database file.
        String      mVFS; // The specified virtual file system.

        // ----------------------------------------------------------------------------------------
        bool        mMemory; // Whether the database exists in memory and not disk.
        bool        mTrace; // Whether tracing was activated on the database.
        bool        mProfile; // Whether profiling was activated on the database.

        /* ----------------------------------------------------------------------------------------
         * Base constructor.
        */
        Handle(Counter counter)
            : mPtr(nullptr)
            , mRef(counter)
            , mStatus(SQLITE_OK)
            , mQueue()
            , mFlags(0)
            , mName()
            , mVFS()
            , mMemory(false)
            , mTrace(false)
            , mProfile(false)
        {
            /* ... */
        }

        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~Handle();

        /* ----------------------------------------------------------------------------------------
         * Create the database connection resource.
        */
        void Create(CSStr name, Int32 flags, CSStr vfs);

        /* ----------------------------------------------------------------------------------------
         * Execute a specific amount of queries from the queue.
        */
        Int32 Flush(Uint32 num, Object & env, Function & func);
    };

private:

    // --------------------------------------------------------------------------------------------
    Handle * m_Hnd; // The managed handle instance.

    /* --------------------------------------------------------------------------------------------
     * Grab a strong reference to a connection handle.
    */
    void Grab()
    {
        if (m_Hnd)
        {
            ++(m_Hnd->mRef);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Drop a strong reference to a connection handle.
    */
    void Drop()
    {
        if (m_Hnd && --(m_Hnd->mRef) == 0)
        {
            delete m_Hnd; // Let the destructor take care of cleaning up (if necessary)
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    ConnHnd(CSStr name)
        : m_Hnd(name ? new Handle(1) : nullptr)
    {
        /* ... */
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor (null).
    */
    ConnHnd()
        : m_Hnd(nullptr)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    ConnHnd(const ConnHnd & o)
        : m_Hnd(o.m_Hnd)
    {
        Grab();
    }

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    ConnHnd(ConnHnd && o)
        : m_Hnd(o.m_Hnd)
    {
        o.m_Hnd = nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~ConnHnd()
    {
        Drop();
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    ConnHnd & operator = (const ConnHnd & o)
    {
        if (m_Hnd != o.m_Hnd)
        {
            Drop();
            m_Hnd = o.m_Hnd;
            Grab();
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    ConnHnd & operator = (ConnHnd && o)
    {
        if (m_Hnd != o.m_Hnd)
        {
            m_Hnd = o.m_Hnd;
            o.m_Hnd = nullptr;
        }

        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Status assignment operator.
    */
    ConnHnd & operator = (Int32 status)
    {
        if (m_Hnd)
        {
            m_Hnd->mStatus = status;
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two connection handles.
    */
    bool operator == (const ConnHnd & o) const
    {
        return (m_Hnd == o.m_Hnd);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two connection handles.
    */
    bool operator != (const ConnHnd & o) const
    {
        return (m_Hnd != o.m_Hnd);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison with an integer value status.
    */
    bool operator == (Int32 status) const
    {
        if (m_Hnd)
        {
            return (m_Hnd->mStatus == status);
        }
        return false;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison with an integer value status.
    */
    bool operator != (Int32 status) const
    {
        if (m_Hnd)
        {
            return (m_Hnd->mStatus != status);
        }
        return false;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean for use in boolean operations.
    */
    operator bool () const
    {
        return m_Hnd && m_Hnd->mPtr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance.
    */
    operator Pointer ()
    {
        return m_Hnd ? m_Hnd->mPtr : nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance.
    */
    operator Pointer () const
    {
        return m_Hnd ? m_Hnd->mPtr : nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance.
    */
    operator Reference ()
    {
        assert(m_Hnd && m_Hnd->mPtr);
        return *(m_Hnd->mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance.
    */
    operator ConstRef () const
    {
        assert(m_Hnd && m_Hnd->mPtr);
        return *(m_Hnd->mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Member operator for dereferencing the managed pointer.
    */
    Handle * operator -> () const
    {
        assert(m_Hnd);
        return m_Hnd;
    }

    /* --------------------------------------------------------------------------------------------
     * Indirection operator for obtaining a reference of the managed pointer.
    */
    Handle & operator * () const
    {
        assert(m_Hnd);
        return *m_Hnd;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw handle structure pointer.
    */
    Handle * HndPtr()
    {
        return m_Hnd;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the raw handle structure pointer.
    */
    Handle * HndPtr() const
    {
        return m_Hnd;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of active references to the managed instance.
    */
    Counter Count() const
    {
        return m_Hnd ? m_Hnd->mRef : 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last known status code.
    */
    Int32 Status() const
    {
        return m_Hnd ? m_Hnd->mStatus : SQLITE_NOMEM;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the message of the last received error code.
    */
    CCStr ErrStr() const
    {
        // SQLite does it's null pointer validations internally
        if (m_Hnd)
        {
            return sqlite3_errstr(sqlite3_errcode(m_Hnd->mPtr));
        }
        return _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * Return the last error message associated with this database connection.
    */
    CCStr ErrMsg() const
    {
        // SQLite does it's null pointer validations internally
        if (m_Hnd)
        {
            return sqlite3_errmsg(m_Hnd->mPtr);
        }
        return _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * Return the numeric result code for the most recent failed API call (if any).
    */
    Int32 ErrNo() const
    {
        // SQLite does it's null pointer validations internally
        if (m_Hnd)
        {
            return sqlite3_errcode(m_Hnd->mPtr);
        }
        return SQLITE_NOMEM;
    }

    /* --------------------------------------------------------------------------------------------
     * Return the extended numeric result code for the most recent failed API call (if any).
    */
    Int32 ExErrNo() const
    {
        // SQLite does it's null pointer validations internally
        if (m_Hnd)
        {
            return sqlite3_extended_errcode(m_Hnd->mPtr);
        }
        return SQLITE_NOMEM;
    }
};

} // Namespace:: SqMod

#endif // _SQSQLITE_HANDLE_CONNECTION_HPP_
