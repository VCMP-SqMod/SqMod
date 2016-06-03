#ifndef _SQSQLITE_HANDLE_STATEMENT_HPP_
#define _SQSQLITE_HANDLE_STATEMENT_HPP_

// ------------------------------------------------------------------------------------------------
#include "Handle/Connection.hpp"

// ------------------------------------------------------------------------------------------------
#include <map>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Manages a reference counted database statement handle.
*/
class StmtHnd
{
    // --------------------------------------------------------------------------------------------
    friend class Connection;
    friend class Statement;

public:

    // --------------------------------------------------------------------------------------------
    typedef sqlite3_stmt    Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*           Pointer; // Pointer to the managed type.
    typedef const Type*     ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&           Reference; // Reference to the managed type.
    typedef const Type&     ConstRef; // Constant reference to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef unsigned int    Counter; // Reference counter type.

    /* --------------------------------------------------------------------------------------------
     * Validate the statement handle and throw an error if invalid.
    */
    void Validate() const;

protected:

    // --------------------------------------------------------------------------------------------
    typedef std::map< String, int > Indexes; // Container used to identify column indexes.

    /* --------------------------------------------------------------------------------------------
     * The structure that holds the data associated with a certain statement.
    */
    struct Handle
    {
        // ----------------------------------------------------------------------------------------
        Pointer     mPtr; // The statement handle resource.
        Counter     mRef; // Reference count to the managed handle.

        // ----------------------------------------------------------------------------------------
        Int32       mStatus; // The last status code of this connection handle.

        // ----------------------------------------------------------------------------------------
        ConnHnd     mConn; // The handle to the associated database connection.

        // ----------------------------------------------------------------------------------------
        String      mQuery; // The query string used to create this statement.

        // ----------------------------------------------------------------------------------------
        Int32       mColumns; // The amount of columns available in this statement.
        Indexes     mIndexes; // An associative container with column names and their index.

        // ----------------------------------------------------------------------------------------
        bool        mGood; // True when a row has been fetched with step.
        bool        mDone; // True when the last step had no more rows to fetch.

        /* ----------------------------------------------------------------------------------------
         * Base constructor.
        */
        Handle(const ConnHnd & conn, Counter counter)
            : mPtr(nullptr)
            , mRef(counter)
            , mStatus(SQLITE_OK)
            , mConn(conn)
            , mQuery()
            , mColumns(0)
            , mIndexes()
            , mGood(false)
            , mDone(false)
        {
            /* ... */
        }

        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~Handle();

        /* ----------------------------------------------------------------------------------------
         * Create the database statement resource.
        */
        void Create(CSStr query);

        /* ----------------------------------------------------------------------------------------
         * Check whether a specific index is in range.
        */
        bool CheckIndex(Int32 idx) const
        {
            return (idx >= 0) && (idx <= mColumns);
        }

        /* ----------------------------------------------------------------------------------------
         * Retrieve the column index associated with the specified name.
        */
        Int32 GetColumnIndex(CSStr name);
    };

private:

    // --------------------------------------------------------------------------------------------
    Handle * m_Hnd; // The managed handle instance.

    /* --------------------------------------------------------------------------------------------
     * Grab a strong reference to a statement handle.
    */
    void Grab()
    {
        if (m_Hnd)
        {
            ++(m_Hnd->mRef);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Drop a strong reference to a statement handle.
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
    StmtHnd(const ConnHnd & db)
        : m_Hnd(new Handle(db, 1))
    {
        /* ... */
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor (null).
    */
    StmtHnd()
        : m_Hnd(nullptr)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    StmtHnd(const StmtHnd & o)
        : m_Hnd(o.m_Hnd)

    {
        Grab();
    }

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    StmtHnd(StmtHnd && o)
        : m_Hnd(o.m_Hnd)
    {
        o.m_Hnd = nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~StmtHnd()
    {
        Drop();
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    StmtHnd & operator = (const StmtHnd & o)
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
    StmtHnd & operator = (StmtHnd && o)
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
    StmtHnd & operator = (Int32 status)
    {
        if (m_Hnd)
        {
            m_Hnd->mStatus = status;
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two statement handles.
    */
    bool operator == (const StmtHnd & o) const
    {
        return (m_Hnd == o.m_Hnd);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two statement handles.
    */
    bool operator != (const StmtHnd & o) const
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
     * Perform an inequality comparison with an integer status value.
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
        if (m_Hnd)
        {
            return m_Hnd->mConn.ErrStr();
        }
        return _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * Return the last error message associated with this database connection.
    */
    CCStr ErrMsg() const
    {
        if (m_Hnd)
        {
            return m_Hnd->mConn.ErrMsg();
        }
        return _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * Return the numeric result code for the most recent failed API call (if any).
    */
    Int32 ErrNo() const
    {
        if (m_Hnd)
        {
            return m_Hnd->mConn.ErrNo();
        }
        return SQLITE_NOMEM;
    }

    /* --------------------------------------------------------------------------------------------
     * Return the extended numeric result code for the most recent failed API call (if any).
    */
    Int32 ExErrNo() const
    {
        if (m_Hnd)
        {
            return m_Hnd->mConn.ExErrNo();
        }
        return SQLITE_NOMEM;
    }
};

} // Namespace:: SqMod

#endif // _SQSQLITE_HANDLE_STATEMENT_HPP_
