#ifndef _SQMYSQL_HANDLE_STATEMENT_HPP_
#define _SQMYSQL_HANDLE_STATEMENT_HPP_

// ------------------------------------------------------------------------------------------------
#include "Handle/Connection.hpp"

// ------------------------------------------------------------------------------------------------
#include <Base/Buffer.hpp>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Manages a reference counted statement handle.
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
    typedef unsigned int    Counter; // Reference counter type.

    // --------------------------------------------------------------------------------------------
    typedef MYSQL_BIND      BindType; // Database bind type.
    typedef MYSQL_TIME      TimeType; // Database time type.
    typedef my_bool         BoolType; // Database boolean type.

    /* --------------------------------------------------------------------------------------------
     * Validate the statement handle and throw an error if invalid.
    */
    void Validate() const;

    /* --------------------------------------------------------------------------------------------
     * Validate the statement handle and parameter index and throw an error if invalid.
    */
    void ValidateIndex(Uint32 idx) const;

protected:

    // --------------------------------------------------------------------------------------------
    typedef std::map< String, int > Indexes; // Container used to identify column indexes.

    /* --------------------------------------------------------------------------------------------
     * The structure that holds the data associated with a certain bind point.
    */
    struct Bind
    {
        // ----------------------------------------------------------------------------------------
        BoolType        mIsNull; // Allows the database to specify if the parameter is null.
        BoolType        mError; // Allows the database if errors occured on this parameter.
        Buffer          mData; // Buffer to store non fundamental data for the parameter.
        BindType *      mBind; // The associated database bind point handle.
        TimeType        mTime; // Structure used to store time data from database.

        // ----------------------------------------------------------------------------------------
        union
        {
            Uint64      mUint64; // Store unsigned integer values for the parameter.
            Int64       mInt64; // Store signed integer values for the parameter.
            Int32       mInt32[2]; // Store 32 bit signed integer values for the parameter.
            Float64     mFloat64; // Store 32 bit floating point values for the parameter.
            Float32     mFloat32[2]; // Store 64 bit floating point values for the parameter.
        };

        /* ----------------------------------------------------------------------------------------
         * Default constructor.
        */
        Bind()
            : mIsNull(0), mError(0), mData(), mBind(nullptr), mTime(), mUint64(0)
        {
            /* ... */
        }

        /* ----------------------------------------------------------------------------------------
         * Copy constructor. (disabled)
        */
        Bind(const Bind & o) = delete;

        /* ----------------------------------------------------------------------------------------
         * Move constructor. (disabled)
        */
        Bind(Bind && o) = delete;

        /* ----------------------------------------------------------------------------------------
         * Copy assignment operator. (disabled)
        */
        Bind & operator = (const Bind & o) = delete;

        /* ----------------------------------------------------------------------------------------
         * Move assignment operator. (disabled)
        */
        Bind & operator = (Bind && o) = delete;

        /* ----------------------------------------------------------------------------------------
         * Retrieve the used buffer.
        */
        CStr GetBuffer()
        {
            return mData ? mData.Data() : reinterpret_cast< CStr >(&mUint64);
        }

        /* ----------------------------------------------------------------------------------------
         * Retrieve the buffer length.
        */
        Ulong GetLength() const
        {
            return mBind == nullptr ? 0 : mBind->buffer_length;
        }

        /* ----------------------------------------------------------------------------------------
         * Configure the input of a certain statement parameter.
        */
        void SetInput(enum_field_types type, BindType * bind, CCStr buffer = nullptr, Ulong length = 0UL);
    };

    /* --------------------------------------------------------------------------------------------
     * The structure that holds the data associated with a certain statement handle.
    */
    struct Handle
    {
        // ----------------------------------------------------------------------------------------
        Pointer         mPtr; // The managed statement handle.
        Counter         mRef; // Reference count to the managed handle.

        // ----------------------------------------------------------------------------------------
        Uint32          mErrNo; // Last received error string.
        String          mErrStr; // Last received error message.

        // ----------------------------------------------------------------------------------------
        Ulong           mParams; // Number of parameters in the statement.
        Bind *          mBinds; // List of parameter binds.
        BindType *      mMyBinds; // List of parameter binds.

        // ----------------------------------------------------------------------------------------
        ConnHnd         mConnection; // Reference to the associated connection.
        String          mQuery; // The query string.

        /* ----------------------------------------------------------------------------------------
         * Base constructor.
        */
        Handle(const ConnHnd & conn, CSStr query);

        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~Handle();

        /* ----------------------------------------------------------------------------------------
         * Grab the current error in the connection handle and throw it.
        */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
        void ThrowCurrent(CCStr act, CCStr file, Int32 line);
#else
        void ThrowCurrent(CCStr act);
#endif // _DEBUG

        /* ----------------------------------------------------------------------------------------
         * Create the database statement resource.
        */
        void Create(CSStr query);
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
    StmtHnd(const ConnHnd & db, CSStr query)
        : m_Hnd(new Handle(db, query))
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
    StmtHnd & operator = (Uint32 status)
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
    bool operator == (Uint32 status) const
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
    bool operator != (Uint32 status) const
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
        return (m_Hnd != nullptr) && (m_Hnd->mPtr != nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance.
    */
    operator Pointer ()
    {
        return (m_Hnd != nullptr) ? m_Hnd->mPtr : nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance.
    */
    operator Pointer () const
    {
        return (m_Hnd != nullptr) ? m_Hnd->mPtr : nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance.
    */
    operator Reference ()
    {
        assert((m_Hnd != nullptr) && (m_Hnd->mPtr != nullptr));
        return *(m_Hnd->mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance.
    */
    operator ConstRef () const
    {
        assert((m_Hnd != nullptr) && (m_Hnd->mPtr != nullptr));
        return *(m_Hnd->mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Member operator for dereferencing the managed pointer.
    */
    Handle * operator -> () const
    {
        assert(m_Hnd != nullptr);
        return m_Hnd;
    }

    /* --------------------------------------------------------------------------------------------
     * Indirection operator for obtaining a reference of the managed pointer.
    */
    Handle & operator * () const
    {
        assert(m_Hnd != nullptr);
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
        return (m_Hnd != nullptr) ? m_Hnd->mRef : 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the handle reference but only if valid.
    */
    Handle & GetHnd()
    {
        // Validate the managed handle
        Validate();
        // Return the requesed information
        return *m_Hnd;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the handle reference but only if valid.
    */
    const Handle & GetHnd() const
    {
        // Validate the managed handle
        Validate();
        // Return the requesed information
        return *m_Hnd;
    }
};

} // Namespace:: SqMod

#endif // _SQMYSQL_HANDLE_STATEMENT_HPP_
