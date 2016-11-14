#ifndef _SQSQLITE_PARAMETER_HPP_
#define _SQSQLITE_PARAMETER_HPP_

// ------------------------------------------------------------------------------------------------
#include "Handle/Statement.hpp"

// ------------------------------------------------------------------------------------------------
#include <cctype>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Used to manage and interact with parameters from a database statement.
*/
class Parameter
{
    // --------------------------------------------------------------------------------------------
    friend class Statement;

private:

    // --------------------------------------------------------------------------------------------
    Int32       m_Index; // The index of the managed parameter.
    StmtRef     m_Handle; // Reference to the managed statement.

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
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const StmtRef & GetValid(CCStr file, Int32 line) const;
#else
    const StmtRef & GetValid() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the managed statement handle and throw an error if invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    const StmtRef & GetCreated(CCStr file, Int32 line) const;
#else
    const StmtRef & GetCreated() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Validate the statement reference and parameter index, and throw an error if they're invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateParam(Int32 idx, CCStr file, Int32 line) const;
#else
    void ValidateParam(Int32 idx) const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Modify the index to the specified value.
    */
    void SetIndex(Int32 idx)
    {
        // Assign the index with a failsafe to invalid on error
        AutoAssign< Int32 > aa(m_Index, -1, idx);
        // Validate the obtained parameter index
        SQMOD_VALIDATE_PARAM(*this, idx);
        // Don't fall back to the invalid index anymore
        aa.Set(idx);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the index to the specified value.
    */
    void SetIndex(CSStr name)
    {
        SetIndex(sqlite3_bind_parameter_index(SQMOD_GET_CREATED(*this)->mPtr, name));
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the index to the specified value.
    */
    void SetIndex(const Object & param);

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor (null).
    */
    Parameter()
        : m_Index(0), m_Handle()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * No parameter constructor.
    */
    Parameter(const StmtRef & stmt)
        : m_Index(0), m_Handle(stmt)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Index constructor.
    */
    Parameter(const StmtRef & stmt, Int32 idx)
        : m_Index(idx), m_Handle(stmt)
    {
        SQMOD_VALIDATE_PARAM(*this, m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Name constructor.
    */
    Parameter(const StmtRef & stmt, CSStr name)
        : m_Index(stmt ? sqlite3_bind_parameter_index(stmt->mPtr, name) : 0), m_Handle(stmt)
    {
        SQMOD_VALIDATE_PARAM(*this, m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Dynamic constructor.
    */
    Parameter(const StmtRef & stmt, const Object & param)
        : m_Index(0), m_Handle(stmt)
    {
        if (!m_Handle)
        {
            STHROWF("Invalid SQLite statement reference");
        }
        // Extract the index
        SetIndex(param);
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Parameter(const Parameter & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Parameter(Parameter && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Parameter & operator = (const Parameter & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Parameter & operator = (Parameter && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two parameter indexes.
    */
    bool operator == (const Parameter & o) const
    {
        return (m_Index == o.m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two parameter indexes.
    */
    bool operator != (const Parameter & o) const
    {
        return (m_Index != o.m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean for use in boolean operations.
    */
    operator bool () const
    {
        return m_Index >= 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        CSStr val = nullptr;
        // Can we attempt to return the parameter name?
        if (m_Handle && m_Index)
        {
            val = sqlite3_bind_parameter_name(m_Handle->mPtr, m_Index);
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
     * See whether this statement is valid.
    */
    bool IsValid() const
    {
        return m_Handle; // An invalid statement means an invalid parameter
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to this statement handle.
    */
    Uint32 GetRefCount() const
    {
        return m_Handle.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated parameter index.
    */
    Int32 GetIndex() const
    {
        return m_Index;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated database statement.
    */
    Object GetStatement() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated database connection.
    */
    Object GetConnection() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the name of the referenced parameter.
    */
    CSStr GetName() const
    {
        return sqlite3_bind_parameter_name(SQMOD_GET_CREATED(*this)->mPtr, m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Release the reference to the associated database statement and parameter index.
    */
    void Release()
    {
        m_Handle.Reset();
        m_Index = 0;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a dynamic value at the referenced parameter index.
    */
    void SetValue(const Object & value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a boolean value at the referenced parameter index.
    */
    void SetBool(bool value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a character value at the referenced parameter index.
    */
    void SetChar(SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a native integer value at the referenced parameter index.
    */
    void SetInteger(SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a signed 8 bit integer value at the referenced parameter index.
    */
    void SetInt8(SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind an unsigned 8 bit integer value at the referenced parameter index.
    */
    void SetUint8(SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a signed 16 bit integer value at the referenced parameter index.
    */
    void SetInt16(SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind an unsigned 16 bit integer value at the referenced parameter index.
    */
    void SetUint16(SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a signed 32 bit integer value at the referenced parameter index.
    */
    void SetInt32(SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind an unsigned 32 bit integer value at the referenced parameter index.
    */
    void SetUint32(SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a signed 64 bit integer value at the referenced parameter index.
    */
    void SetInt64(const Object & value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind an unsigned 64 bit integer value at the referenced parameter index.
    */
    void SetUint64(const Object & value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a native floating point value at the referenced parameter index.
    */
    void SetFloat(SQFloat value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a 32 bit floating point value at the referenced parameter index.
    */
    void SetFloat32(SQFloat value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a 64 bit floating point value at the referenced parameter index.
    */
    void SetFloat64(SQFloat value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a string value at the referenced parameter index.
    */
    void SetString(CSStr value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a string value at the referenced parameter index.
    */
    void SetStringEx(CSStr value, Int32 length);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a zeroed blob value at the referenced parameter index.
    */
    void SetZeroBlob(SQInteger size);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a blob value at the referenced parameter index.
    */
    void SetBlob(const Object & value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a buffer value at the referenced parameter index.
    */
    void SetData(const Object & value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a date value at the referenced parameter index.
    */
    void SetDate(const Object & value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a date value at the referenced parameter index.
    */
    void SetDateEx(SQInteger year, SQInteger month, SQInteger day);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a time value at the referenced parameter index.
    */
    void SetTime(const Object & value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a time value at the referenced parameter index.
    */
    void SetTimeEx(SQInteger hour, SQInteger minute, SQInteger second);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a date-time value at the referenced parameter index.
    */
    void SetDatetime(const Object & value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a date-time value at the referenced parameter index.
    */
    void SetDatetimeEx(SQInteger year, SQInteger month, SQInteger day, SQInteger hour, SQInteger minute, SQInteger second);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the current timestamp at the referenced parameter index.
    */
    void SetNow();

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a null value at the referenced parameter index.
    */
    void SetNull();

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a string value at the referenced parameter index.
    */
    static SQInteger SetStringF(HSQUIRRELVM vm);

};

} // Namespace:: SqMod

#endif // _SQSQLITE_PARAMETER_HPP_
