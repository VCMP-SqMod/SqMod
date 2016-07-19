#ifndef _SQMYSQL_FIELD_HPP_
#define _SQMYSQL_FIELD_HPP_

// ------------------------------------------------------------------------------------------------
#include "Handle/ResultSet.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Used to manage and interact with fields from result-sets.
*/
class Field
{
    // --------------------------------------------------------------------------------------------
    friend class ResultSet;

private:

    // --------------------------------------------------------------------------------------------
    Uint32  m_Index; // The actual index of the referenced field.
    ResRef  m_Handle; // Reference to the actual database result-set.

protected:

    /* --------------------------------------------------------------------------------------------
     * Validate the associated result-set handle and field index, and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void Validate(CCStr file, Int32 line) const;
#else
    void Validate() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the associated result-set handle and field index, and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateCreated(CCStr file, Int32 line) const;
#else
    void ValidateCreated() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the associated result-set handle and field index, and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const ResRef & GetValid(CCStr file, Int32 line) const;
#else
    const ResRef & GetValid() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the associated result-set handle and field index, and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const ResRef & GetCreated(CCStr file, Int32 line) const;
#else
    const ResRef & GetCreated() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the associated result-set handle and field index, and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateField(Uint32 idx, CCStr file, Int32 line) const;
#else
    void ValidateField(Uint32 idx) const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Modify the index to the specified value.
    */
    void SetIndex(Int32 idx)
    {
        SQMOD_VALIDATE_FIELD(*this, idx);
        // Assign the new index
        m_Index = idx;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the index to the specified value.
    */
    void SetIndex(CSStr name)
    {
        SetIndex(SQMOD_GET_CREATED(*this)->GetFieldIndex(name));
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the index to the specified value.
    */
    void SetIndex(const Object & field);

public:

    // --------------------------------------------------------------------------------------------
    static const Uint32 INVALID_INDEX;

    /* --------------------------------------------------------------------------------------------
     * Default constructor (null).
    */
    Field()
        : m_Index(INVALID_INDEX), m_Handle()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * No field constructor.
    */
    Field(const ResRef & rset)
        : m_Index(INVALID_INDEX), m_Handle(rset)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Index constructor.
    */
    Field(const ResRef & rset, Uint32 idx)
        : m_Index(idx), m_Handle(rset)
    {
        SQMOD_VALIDATE_FIELD(*this, m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Name constructor.
    */
    Field(const ResRef & rset, CSStr name)
        : m_Index(rset ? rset->GetFieldIndex(name) : -1), m_Handle(rset)
    {
        SQMOD_VALIDATE_FIELD(*this, m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Dynamic constructor.
    */
    Field(const ResRef & rset, const Object & field)
        : m_Index(INVALID_INDEX), m_Handle(rset)
    {
        if (!m_Handle)
        {
            STHROWF("Invalid MySQL result-set reference");
        }
        // Extract the index
        SetIndex(field);
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Field(const Field & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Field(Field && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Field & operator = (const Field & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Field & operator = (Field && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two result-set field indexes.
    */
    bool operator == (const Field & o) const
    {
        return (m_Index == o.m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two result-set field indexes.
    */
    bool operator != (const Field & o) const
    {
        return (m_Index != o.m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean for use in boolean operations.
    */
    operator bool () const
    {
        return (m_Handle && m_Handle->CheckFieldIndex(m_Index));
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Field & o) const
    {
        if (m_Index == o.m_Index)
        {
            return 0;
        }
        else if (m_Index > o.m_Index)
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
        CSStr val = nullptr;
        // Can we attempt to return the parameter name?
        if (m_Handle && m_Handle->CheckFieldIndex(m_Index))
        {
            val = m_Handle->mFields[m_Index].name;
        }
        else
        {
            val = ToStrF(_SC("%d"), m_Index);
        }
        // Return the value if valid
        return val ? val : _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See whether the field is valid.
    */
    bool IsValid() const
    {
        return m_Handle; // An invalid result-set means an invalid field
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to the associated result-set handle.
    */
    Uint32 GetRefCount() const
    {
        return m_Handle.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the referenced field index.
    */
    SQInteger GetIndex() const
    {
        return ConvTo< SQInteger >::From(m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the referenced database statement.
    */
    Object GetStatement() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the referenced database connection.
    */
    Object GetConnection() const;

    /* --------------------------------------------------------------------------------------------
     * Release the reference to the referenced database statement and field index.
    */
    void Release()
    {
        m_Handle.Reset();
        m_Index = INVALID_INDEX;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a boolean value.
    */
    bool GetBoolean() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a character.
    */
    SQChar GetChar() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a native script integer.
    */
    SQInteger GetInteger() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a native script floating point.
    */
    SQFloat GetFloat() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a signed 8 bit integer value.
    */
    SQInteger GetInt8() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as an unsigned 8 bit integer value.
    */
    SQInteger GetUint8() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a signed 16 bit integer value.
    */
    SQInteger GetInt16() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as an unsigned 16 bit integer value.
    */
    SQInteger GetUint16() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a signed 32 bit integer value.
    */
    SQInteger GetInt32() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as an unsigned 32 bit integer value.
    */
    SQInteger GetUint32() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a signed 64 bit integer value.
    */
    Object GetInt64() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as an unsigned 64 bit integer value.
    */
    Object GetUint64() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a 32 bit floating point value.
    */
    SQFloat GetFloat32() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a 64 bit floating point value.
    */
    SQFloat GetFloat64() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a string value.
    */
    Object GetString() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a memory buffer.
    */
    Object GetBuffer() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced field as a memory blob.
    */
    Object GetBlob() const;
};

} // Namespace:: SqMod

#endif // _SQMYSQL_FIELD_HPP_
