#ifndef _SQMYSQL_RESULTSET_HPP_
#define _SQMYSQL_RESULTSET_HPP_

// ------------------------------------------------------------------------------------------------
#include "Handle/ResultSet.hpp"
#include "Field.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Allows management and interaction with a result set handle.
*/
class ResultSet
{
private:

    // --------------------------------------------------------------------------------------------
    ResRef  m_Handle; // Reference to the actual database result-set.

protected:

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void Validate(CCStr file, Int32 line) const;
#else
    void Validate() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateCreated(CCStr file, Int32 line) const;
#else
    void ValidateCreated() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and row, and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateStepped(CCStr file, Int32 line) const;
#else
    void ValidateStepped() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const ResRef & GetValid(CCStr file, Int32 line) const;
#else
    const ResRef & GetValid() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const ResRef & GetCreated(CCStr file, Int32 line) const;
#else
    const ResRef & GetCreated() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and row, and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const ResRef & GetStepped(CCStr file, Int32 line) const;
#else
    const ResRef & GetStepped() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the statement reference and field index, and throw an error if they're invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateField(Int32 idx, CCStr file, Int32 line) const;
#else
    void ValidateField(Int32 idx) const;
#endif // _DEBUG

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    ResultSet()
        : m_Handle()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Connection constructor.
    */
    ResultSet(const ConnRef & conn)
        : m_Handle(new ResHnd())
    {
        m_Handle->Create(conn);
    }

    /* --------------------------------------------------------------------------------------------
     * Statement constructor.
    */
    ResultSet(const StmtRef & stmt)
        : m_Handle(new ResHnd())
    {
        m_Handle->Create(stmt);
    }

    /* --------------------------------------------------------------------------------------------
     * Handle constructor.
    */
    ResultSet(const ResRef & hnd)
        : m_Handle(hnd)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    ResultSet(const ResultSet & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    ResultSet(ResultSet && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~ResultSet() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    ResultSet & operator = (const ResultSet & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    ResultSet & operator = (ResultSet && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const ResultSet & o) const
    {
        if (m_Handle.Get() == o.m_Handle.Get())
        {
            return 0;
        }
        else if (m_Handle.Get() > o.m_Handle.Get())
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        // Do we have a valid handle?
        if (m_Handle)
        {
            ToStrF("%u", m_Handle->mFieldCount);
        }
        // Default to a negative value
        return _SC("-1");
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See whether the managed handle is valid.
    */
    bool IsValid() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Returns an array with all the field names available in the managed result set.
    */
    Array FieldNames() const;

    /* --------------------------------------------------------------------------------------------
     * Returns the current position of the row cursor for the last Next().
    */
    Object RowIndex() const
    {
        return MakeULongObj(SQMOD_GET_CREATED(*this)->RowIndex());
    }

    /* --------------------------------------------------------------------------------------------
     * Returns the number of rows in the result set.
    */
    Object RowCount() const
    {
        return MakeULongObj(SQMOD_GET_CREATED(*this)->RowCount());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the next row from the query.
    */
    bool Next() const
    {
        return SQMOD_GET_CREATED(*this)->Next();
    }

    /* --------------------------------------------------------------------------------------------
     * Seeks to an arbitrary row in a query result set.
    */
    bool SetRowIndex(SQInteger index) const
    {
        return SQMOD_GET_CREATED(*this)->SetRowIndex(ConvTo< Uint64 >::From(index));
    }

    /* --------------------------------------------------------------------------------------------
     * Seeks to an arbitrary row in a query result set.
    */
    bool SetLongRowIndex(Object & index) const
    {
        return SQMOD_GET_CREATED(*this)->SetRowIndex(FetchULongObjVal(index));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the field with the specified name or index.
    */
    Field GetField(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a boolean value.
    */
    bool GetBoolean(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetBoolean();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a character.
    */
    SQChar GetChar(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetChar();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a native script integer.
    */
    SQInteger GetInteger(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetInteger();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a native script floating point.
    */
    SQFloat GetFloat(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetFloat();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a signed 8 bit integer value.
    */
    SQInteger GetInt8(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetInt8();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as an unsigned 8 bit integer value.
    */
    SQInteger GetUint8(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetUint8();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a signed 16 bit integer value.
    */
    SQInteger GetInt16(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetInt16();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as an unsigned 16 bit integer value.
    */
    SQInteger GetUint16(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetUint16();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a signed 32 bit integer value.
    */
    SQInteger GetInt32(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetInt32();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as an unsigned 32 bit integer value.
    */
    SQInteger GetUint32(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetUint32();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a signed 64 bit integer value.
    */
    Object GetInt64(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetInt64();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as an unsigned 64 bit integer value.
    */
    Object GetUint64(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetUint64();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a 32 bit floating point value.
    */
    SQFloat GetFloat32(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetFloat32();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a 64 bit floating point value.
    */
    SQFloat GetFloat64(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetFloat64();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a string value.
    */
    Object GetString(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetString();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a memory buffer.
    */
    Object GetBuffer(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetBuffer();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the specified field as a memory blob.
    */
    Object GetBlob(const Object & field) const
    {
        return Field(SQMOD_GET_STEPPED(*this), field).GetBlob();
    }
};

} // Namespace:: SqMod

#endif // _SQMYSQL_RESULTSET_HPP_
