#ifndef _SQMYSQL_HANDLES_HPP_
#define _SQMYSQL_HANDLES_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
#include <mysql.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Forward declarations.
*/
class Connection;
class Statement;
class Column;
class Transaction;

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
    typedef MYSQL           Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*           Pointer; // Pointer to the managed type.
    typedef const Type*     ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&           Reference; // Reference to the managed type.
    typedef const Type&     ConstRef; // Constant reference to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef unsigned int    Counter; // Reference counter type.

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
        Uint32      mErrNo; // Last received error string.
        String      mErrStr; // Last received error message.

        // --------------------------------------------------------------------------------------------
        String      mHost; // Host address.
        String      mUser; // User name user.
        String      mPass; // User password.
        Uint16      mPort; // Server port.

        // --------------------------------------------------------------------------------------------
        String      mSchema; // Server schema.
        String      mSocket; // Unix socket.

        // --------------------------------------------------------------------------------------------
        String      mSSL_Key; // SSL key.
        String      mSSL_Cert; // SSL certificate.
        String      mSSL_CA; // SSL certificate authority.
        String      mSSL_CA_Path; // SSL certificate authority path.
        String      mSSL_Cipher; // SSL Cipher.

        // --------------------------------------------------------------------------------------------
        Options     mOptions; // Option container.

        // ----------------------------------------------------------------------------------------
        QueryList   mQueue; // A queue of queries to be executed in groups.

        // --------------------------------------------------------------------------------------------
        bool        mAutocommit; // Whether autocommit is enabled on this connection.
        bool        mInTransaction; // Whether the connection is in the middle of a transaction.

        /* ----------------------------------------------------------------------------------------
         * Base constructor.
        */
        Handle(Counter counter)
            : mPtr(nullptr)
            , mRef(counter)
            , mErrNo(0)
            , mErrStr(_SC(""))
            , mHost(_SC(""))
            , mUser(_SC(""))
            , mPass(_SC(""))
            , mPort(3306)
            , mSchema(_SC(""))
            , mSocket(_SC(""))
            , mSSL_Key(_SC(""))
            , mSSL_Cert(_SC(""))
            , mSSL_CA(_SC(""))
            , mSSL_CA_Path(_SC(""))
            , mSSL_Cipher(_SC(""))
            , mOptions(_SC(""))
            , mQueue()
            , mAutocommit(false)
            , mInTransaction(false)
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
        void Create(CSStr host, CSStr user, CSStr pass, CSStr schema, Uint16 port, CSStr socket);

        /* ----------------------------------------------------------------------------------------
         * Execute a specific amount of queries from the queue.
        */
        Int32 Flush(Uint32 num, Object & env, Function & func);
    };

private:

    // --------------------------------------------------------------------------------------------
    Handle* m_Hnd;

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
            m_Hnd->mErrNo = status;
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
            return (m_Hnd->mErrNo == status);
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
            return (m_Hnd->mErrNo != status);
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
};

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
    typedef MYSQL_STMT      Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*           Pointer; // Pointer to the managed type.
    typedef const Type*     ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&           Reference; // Reference to the managed type.
    typedef const Type&     ConstRef; // Constant reference to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef MYSQL_BIND      BindType; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef BindType*       BindPointer; // Pointer to the managed type.
    typedef const BindType* BindConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef BindType&       BindReference; // Reference to the managed type.
    typedef const BindType& BindConstRef; // Constant reference to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef unsigned int    Counter; // Reference counter type.

protected:

    // --------------------------------------------------------------------------------------------
    typedef std::map< String, int > Indexes; // Container used to identify column indexes.

    /* --------------------------------------------------------------------------------------------
     * The structure that holds the data associated with a certain statement.
    */
    struct Handle
    {
        // --------------------------------------------------------------------------------------------
        Pointer     mPtr; // The statement handle resource.
        Counter     mRef; // Reference count to the managed handle.

        // ----------------------------------------------------------------------------------------
        Int32       mErrNo; // The last status code of this connection handle.

        // ----------------------------------------------------------------------------------------
        MYSQL_BIND* mBinds;

        /* ----------------------------------------------------------------------------------------
         * Base constructor.
        */
        Handle(const ConnHnd & conn, Counter counter)
            : mPtr(nullptr)
            , mRef(counter)
            , mErrNo(0)
        {
            /* ... */
        }

        /* --------------------------------------------------------------------------------------------
         * Destructor.
        */
        ~Handle();

        /* --------------------------------------------------------------------------------------------
         * Create the database statement resource.
        */
        void Create(CSStr query);
    };

private:

    // --------------------------------------------------------------------------------------------
    Handle* m_Hnd;

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
            m_Hnd->mErrNo = status;
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
            return (m_Hnd->mErrNo == status);
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
            return (m_Hnd->mErrNo != status);
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
};

} // Namespace:: SqMod

#endif // _SQMYSQL_HANDLES_HPP_
