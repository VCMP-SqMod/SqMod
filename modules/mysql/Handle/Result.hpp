#ifndef _SQMYSQL_HANDLE_RESULT_HPP_
#define _SQMYSQL_HANDLE_RESULT_HPP_

// ------------------------------------------------------------------------------------------------
#include "Handle/Statement.hpp"

// ------------------------------------------------------------------------------------------------
#include <Base/Buffer.hpp>

// ------------------------------------------------------------------------------------------------
#include <unordered_map>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Manages a reference counted result set handle.
*/
class ResHnd
{
    // --------------------------------------------------------------------------------------------
    friend class Connection;
    friend class Statement;
    friend class ResultSet;

public:

    // --------------------------------------------------------------------------------------------
    typedef MYSQL_RES       Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*           Pointer; // Pointer to the managed type.
    typedef const Type*     ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&           Reference; // Reference to the managed type.
    typedef const Type&     ConstRef; // Constant reference to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef unsigned int    Counter; // Reference counter type.

    // --------------------------------------------------------------------------------------------
    typedef MYSQL_FIELD     FieldType; // Database field type.
    typedef MYSQL_BIND      BindType; // Database bind type.
    typedef MYSQL_TIME      TimeType; // Database time type.
    typedef MYSQL_ROW       RowType; // Database row type.
    typedef my_bool         BoolType; // Database boolean type.

    /* --------------------------------------------------------------------------------------------
     * Validate the result set handle and throw an error if invalid.
    */
    void Validate() const;

    /* --------------------------------------------------------------------------------------------
     * Validate the result set handle and specified index and throw an error if invalid.
    */
    void ValidateIndex(Uint32 idx) const;

protected:

    // --------------------------------------------------------------------------------------------
    typedef std::unordered_map< String, Uint32 > IndexMap;

    /* --------------------------------------------------------------------------------------------
     * The structure that holds the data associated with a certain bind point.
    */
    struct Bind
    {
        // ----------------------------------------------------------------------------------------
        BoolType        mIsNull; // Allows the database to specify if the field is null.
        BoolType        mError; // Allows the database if errors occured on this field.
        Buffer          mData; // Buffer to store non fundamental data for the field.
        BindType *      mBind; // The associated database bind point handle.
        TimeType        mTime; // Structure used to retrieve time data from database.

        // ----------------------------------------------------------------------------------------
        union
        {
            Uint64      mUint64; // Retrieve unsigned integer values from a field.
            Int64       mInt64; // Retrieve signed integer values from a field.
            Int32       mInt32[2]; // Retrieve 32 bit signed integer values from a field.
            Float64     mFloat64; // Retrieve 32 bit floating point values from a field.
            Float32     mFloat32[2]; // Retrieve 64 bit floating point values from the field.
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
         * Configure the output to match the requirements of a certain field.
        */
        void SetOutput(const FieldType & field, BindType * bind);
    };

    /* --------------------------------------------------------------------------------------------
     * The structure that holds the data associated with a certain result set handle.
    */
    struct Handle
    {
        // ----------------------------------------------------------------------------------------
        Pointer         mPtr; // The managed result set handle.
        Counter         mRef; // Reference count to the managed handle.

        // ----------------------------------------------------------------------------------------
        Uint32          mFieldCount; // Number of fields in the result set.
        Ulong *         mLengths; // Data length when the result set came from a connection.
        FieldType *     mFields; // Fields in the results set.
        Bind *          mBinds; // Bind wrappers.
        BindType *      mMyBinds; // Bind points.
        RowType         mRow; // Row data.

        // ----------------------------------------------------------------------------------------
        StmtHnd         mStatement; // Associated statement.
        IndexMap        mIndexes; // Field names and their associated index.

        /* ----------------------------------------------------------------------------------------
         * Connection constructor.
        */
        Handle(const ConnHnd & conn);

        /* ----------------------------------------------------------------------------------------
         * Statement constructor.
        */
        Handle(const StmtHnd & stmt);

        /* ----------------------------------------------------------------------------------------
         * Destructor.
        */
        ~Handle();
    };

private:

    // --------------------------------------------------------------------------------------------
    Handle * m_Hnd; // The managed handle instance.

    /* --------------------------------------------------------------------------------------------
     * Grab a strong reference to a result set handle.
    */
    void Grab()
    {
        if (m_Hnd)
        {
            ++(m_Hnd->mRef);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Drop a strong reference to a result set handle.
    */
    void Drop()
    {
        if (m_Hnd && --(m_Hnd->mRef) == 0)
        {
            delete m_Hnd; // Let the destructor take care of cleaning up (if necessary)
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Connection constructor.
    */
    ResHnd(const ConnHnd & conn)
        : m_Hnd(conn ? new Handle(conn) : nullptr)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Connection constructor.
    */
    ResHnd(const StmtHnd & stmt)
        : m_Hnd(stmt ? new Handle(stmt) : nullptr)
    {
        /* ... */
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor (null).
    */
    ResHnd()
        : m_Hnd(nullptr)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    ResHnd(const ResHnd & o)
        : m_Hnd(o.m_Hnd)
    {
        Grab();
    }

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    ResHnd(ResHnd && o)
        : m_Hnd(o.m_Hnd)
    {
        o.m_Hnd = nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~ResHnd()
    {
        Drop();
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    ResHnd & operator = (const ResHnd & o)
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
    ResHnd & operator = (ResHnd && o)
    {
        if (m_Hnd != o.m_Hnd)
        {
            m_Hnd = o.m_Hnd;
            o.m_Hnd = nullptr;
        }

        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two result set handles.
    */
    bool operator == (const ResHnd & o) const
    {
        return (m_Hnd == o.m_Hnd);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two result set handles.
    */
    bool operator != (const ResHnd & o) const
    {
        return (m_Hnd != o.m_Hnd);
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

#endif // _SQMYSQL_HANDLE_RESULT_HPP_
