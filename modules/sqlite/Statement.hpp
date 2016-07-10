#ifndef _SQSQLITE_STATEMENT_HPP
#define _SQSQLITE_STATEMENT_HPP

// ------------------------------------------------------------------------------------------------
#include "Handle/Statement.hpp"
#include "Parameter.hpp"
#include "Column.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Used to manage and interact a database statement.
*/
class Statement
{
private:

    // --------------------------------------------------------------------------------------------
    StmtRef m_Handle; // Reference to the managed statement.

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
     * Validate the statement reference and column index, and throw an error if they're invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateColumn(Int32 idx, CCStr file, Int32 line) const;
#else
    void ValidateColumn(Int32 idx) const;
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
     * Validate the statement reference and row, and throw an error if they're invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateRow(CCStr file, Int32 line) const;
#else
    void ValidateRow() const;
#endif // _DEBUG

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Statement()
        : m_Handle()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Construct a statement under the specified connection using the specified string.
    */
    Statement(const ConnRef & connection, CSStr query)
        : m_Handle(new StmtHnd(connection))
    {
        SQMOD_GET_VALID(*this)->Create(query);
    }

    /* --------------------------------------------------------------------------------------------
     * Construct a statement under the specified connection using the specified string.
    */
    Statement(const Connection & connection, CSStr query);

    /* --------------------------------------------------------------------------------------------
     * Direct handle constructor.
    */
    Statement(const StmtRef & s)
        : m_Handle(s)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Statement(const Statement & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Statement(Statement && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Statement & operator = (const Statement & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Statement & operator = (Statement && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two connections.
    */
    bool operator == (const Statement & o) const
    {
        return (m_Handle.Get() == o.m_Handle.Get());
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two connections.
    */
    bool operator != (const Statement & o) const
    {
        return (m_Handle.Get() != o.m_Handle.Get());
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the raw connection handle.
    */
    operator sqlite3_stmt * ()
    {
        return m_Handle ? m_Handle->mPtr : nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the raw connection handle.
    */
    operator sqlite3_stmt * () const
    {
        return m_Handle ? m_Handle->mPtr : nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Statement & o) const
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
    const String & ToString() const
    {
        return m_Handle ? m_Handle->mQuery : NullString();
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated statement handle.
    */
    const StmtRef & GetHandle() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the managed handle is valid.
    */
    bool IsValid() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the managed statement is valid.
    */
    bool IsPrepared() const
    {
        return m_Handle && (m_Handle->mPtr != nullptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to this statement handle.
    */
    Uint32 GetReferences() const
    {
        return m_Handle.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated database connection.
    */
    Object GetConnection() const;

    /* --------------------------------------------------------------------------------------------
     * Release the reference to the associated database statement.
    */
    void Release()
    {
        m_Handle.Reset();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last received status code.
    */
    Int32 GetStatus() const
    {
        return SQMOD_GET_VALID(*this)->mStatus;
    }

    /* --------------------------------------------------------------------------------------------
     * Return the numeric result code for the most recent failed API call (if any).
    */
    Int32 GetErrorCode() const
    {
        return SQMOD_GET_VALID(*this)->ErrNo();
    }

    /* --------------------------------------------------------------------------------------------
     * Return the extended numeric result code for the most recent failed API call (if any).
    */
    Int32 GetExtendedErrorCode() const
    {
        return SQMOD_GET_VALID(*this)->ExErrNo();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the message of the last received error code.
    */
    CSStr GetErrStr() const
    {
        return SQMOD_GET_VALID(*this)->ErrStr();
    }

    /* --------------------------------------------------------------------------------------------
     * Return the last error message associated with this database connection.
    */
    CSStr GetErrMsg() const
    {
        return SQMOD_GET_VALID(*this)->ErrMsg();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the amount of requested columns.
    */
    Int32 GetColumns() const
    {
        return SQMOD_GET_VALID(*this)->mColumns;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the amount of specified parameters.
    */
    Int32 GetParameters() const
    {
        return SQMOD_GET_VALID(*this)->mParameters;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the query string used to create this statement.
    */
    const String & GetQuery() const
    {
        return SQMOD_GET_VALID(*this)->mQuery;
    }

    /* --------------------------------------------------------------------------------------------
     * See if the last step retrieved a valid row.
    */
    bool GetGood() const
    {
        return SQMOD_GET_CREATED(*this)->mGood;
    }

    /* --------------------------------------------------------------------------------------------
     * See if there are any steps left.
    */
    bool GetDone() const
    {
        return SQMOD_GET_CREATED(*this)->mDone;
    }

    /* --------------------------------------------------------------------------------------------
     * Check whether a specific parameter index is within range.
    */
    bool CheckParameter(Int32 idx) const
    {
        return SQMOD_GET_VALID(*this)->CheckParameter(idx);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the parameter index associated with the specified name.
    */
    Int32 GetParameterIndex(CSStr name) const
    {
        return sqlite3_bind_parameter_index(SQMOD_GET_VALID(*this)->mPtr, name);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the parameter name associated with the specified index.
    */
    CSStr GetParameterName(Int32 idx) const
    {
        // Validate the specified index
        if (!idx)
        {
            STHROWF("Invalid parameter index (%d)", idx);
        }
        // Attempt to locate the name at the specified index
        CSStr name = sqlite3_bind_parameter_name(SQMOD_GET_VALID(*this)->mPtr, idx);
        // Validate the obtained string
        if (!name)
        {
            STHROWF("No such parameter exists (%d)", idx);
        }
        // Return the obtained string
        return name;
    }

    /* --------------------------------------------------------------------------------------------
     * Check whether a specific column index is within range.
    */
    bool CheckColumn(Int32 idx) const
    {
        return SQMOD_GET_VALID(*this)->CheckColumn(idx);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the amount of columns available in the current row.
    */
    Int32 GetDataCount() const
    {
        return sqlite3_data_count(SQMOD_GET_VALID(*this)->mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Check whether the specified column is null.
    */
    bool IsColumnNull(Int32 idx) const
    {
        return (sqlite3_column_type(SQMOD_GET_VALID(*this)->mPtr, idx) == SQLITE_NULL);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the column index associated with the specified name.
    */
    Int32 GetColumnIndex(CSStr name) const
    {
        return SQMOD_GET_VALID(*this)->GetColumnIndex(name);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the column name associated with the specified index.
    */
    CSStr GetColumnName(Int32 idx) const
    {
        return sqlite3_column_name(SQMOD_GET_VALID(*this)->mPtr, idx);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the column origin name if the library was compiled with such feature.
    */
    CSStr GetColumnOriginName(Int32 idx) const
    {
#ifdef SQLITE_ENABLE_COLUMN_METADATA
        return sqlite3_column_origin_name(SQMOD_GET_VALID(*this)->mPtr, idx);
#else
        // The compiler moans when extra warnings are enabled
        SQMOD_UNUSED_VAR(idx);
        // Stop the execution here!
        STHROWF("The module was compiled without this feature");
        // We have to return something
        return _SC("");
#endif
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the type identifier of the column associated with the specified index.
    */
    Int32 GetColumnType(Int32 idx) const
    {
        return sqlite3_column_type(SQMOD_GET_VALID(*this)->mPtr, idx);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the size in bytes of the column associated with the specified index.
    */
    Int32 GetColumnBytes(Int32 idx) const
    {
        return sqlite3_column_bytes(SQMOD_GET_VALID(*this)->mPtr, idx);
    }

    /* --------------------------------------------------------------------------------------------
     * Reset the statement back to its initial position to be stepped again.
    */
    Statement & Reset();

    /* --------------------------------------------------------------------------------------------
     * Clear any values binded to this statement.
    */
    Statement & Clear();

    /* --------------------------------------------------------------------------------------------
     * Execute this statement and don't expect any rows to be returned.
    */
    Int32 Exec();

    /* --------------------------------------------------------------------------------------------
     * Step the statement and expect a row to be returned.
    */
    bool Step();

    /* --------------------------------------------------------------------------------------------
     * Retrieve the parameter with the specified name or index.
    */
    Object GetParameter(const Object & param) const
    {
        return Object(new Parameter(m_Handle, param));
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a dynamic value at the specified parameter index.
    */
    Statement & SetValue(const Object & param, const Object & value)
    {
        Parameter(SQMOD_GET_CREATED(*this), param).SetValue(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a boolean value at the specified parameter index.
    */
    Statement & SetBool(const Object & param, bool value)
    {
        Parameter(SQMOD_GET_CREATED(*this), param).SetBool(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a character value at the specified parameter index.
    */
    Statement & SetChar(const Object & param, SQInteger value)
    {
        Parameter(SQMOD_GET_CREATED(*this), param).SetChar(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a native integer value at the specified parameter index.
    */
    Statement & SetInteger(const Object & param, SQInteger value)
    {
        Parameter(SQMOD_GET_CREATED(*this), param).SetInteger(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a signed 8 bit integer value at the specified parameter index.
    */
    Statement & SetInt8(const Object & param, SQInteger value)
    {
        Parameter(SQMOD_GET_CREATED(*this), param).SetInt8(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind an unsigned 8 bit integer value at the specified parameter index.
    */
    Statement & SetUint8(const Object & param, SQInteger value)
    {
        Parameter(SQMOD_GET_CREATED(*this), param).SetUint8(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a signed 16 bit integer value at the specified parameter index.
    */
    Statement & SetInt16(const Object & param, SQInteger value)
    {
        Parameter(SQMOD_GET_CREATED(*this), param).SetInt16(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind an unsigned 16 bit integer value at the specified parameter index.
    */
    Statement & SetUint16(const Object & param, SQInteger value)
    {
        Parameter(SQMOD_GET_CREATED(*this), param).SetUint16(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a signed 32 bit integer value at the specified parameter index.
    */
    Statement & SetInt32(const Object & param, SQInteger value)
    {
        Parameter(SQMOD_GET_CREATED(*this), param).SetInt32(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind an unsigned 32 bit integer value at the specified parameter index.
    */
    Statement & SetUint32(const Object & param, SQInteger value)
    {
        Parameter(SQMOD_GET_CREATED(*this), param).SetUint32(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a signed 64 bit integer value at the specified parameter index.
    */
    Statement & SetInt64(const Object & param, const Object & value)
    {
        Parameter(SQMOD_GET_CREATED(*this), param).SetInt64(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind an unsigned 64 bit integer value at the specified parameter index.
    */
    Statement & SetUint64(const Object & param, const Object & value)
    {
        Parameter(SQMOD_GET_CREATED(*this), param).SetUint64(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a native floating point value at the specified parameter index.
    */
    Statement & SetFloat(const Object & param, SQFloat value)
    {
        Parameter(SQMOD_GET_CREATED(*this), param).SetFloat(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a 32 bit floating point value at the specified parameter index.
    */
    Statement & SetFloat32(const Object & param, SQFloat value)
    {
        Parameter(SQMOD_GET_CREATED(*this), param).SetFloat32(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a 64 bit floating point value at the specified parameter index.
    */
    Statement & SetFloat64(const Object & param, SQFloat value)
    {
        Parameter(SQMOD_GET_CREATED(*this), param).SetFloat64(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a string value at the specified parameter index.
    */
    Statement & SetString(const Object & param, CSStr value)
    {
        Parameter(SQMOD_GET_CREATED(*this), param).SetString(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a zeroed blob value at the specified parameter index.
    */
    Statement & SetZeroBlob(const Object & param, SQInteger size)
    {
        Parameter(SQMOD_GET_CREATED(*this), param).SetZeroBlob(size);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a blob value at the specified parameter index.
    */
    Statement & SetBlob(const Object & param, const Object & value)
    {
        Parameter(SQMOD_GET_CREATED(*this), param).SetBlob(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a buffer value at the specified parameter index.
    */
    Statement & SetData(const Object & param, const Object & value)
    {
        Parameter(SQMOD_GET_CREATED(*this), param).SetData(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a date value at the specified parameter index.
    */
    Statement & SetDate(const Object & param, const Object & value)
    {
        Parameter(SQMOD_GET_CREATED(*this), param).SetDate(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a date value at the specified parameter index.
    */
    Statement & SetDateEx(const Object & param, SQInteger year, SQInteger month, SQInteger day)
    {
        Parameter(SQMOD_GET_CREATED(*this), param).SetDateEx(year, month, day);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a time value at the specified parameter index.
    */
    Statement & SetTime(const Object & param, const Object & value)
    {
        Parameter(SQMOD_GET_CREATED(*this), param).SetTime(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a time value at the specified parameter index.
    */
    Statement & SetTimeEx(const Object & param, SQInteger hour, SQInteger minute, SQInteger second)
    {
        Parameter(SQMOD_GET_CREATED(*this), param).SetTimeEx(hour, minute, second);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a date-time value at the specified parameter index.
    */
    Statement & SetDatetime(const Object & param, const Object & value)
    {
        Parameter(SQMOD_GET_CREATED(*this), param).SetDatetime(value);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a date-time value at the specified parameter index.
    */
    Statement & SetDatetimeEx(const Object & param, SQInteger year, SQInteger month, SQInteger day,
                                    SQInteger hour, SQInteger minute, SQInteger second)
    {
        Parameter(SQMOD_GET_CREATED(*this), param).SetDatetimeEx(year, month, day, hour, minute, second);
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the current timestamp at the specified parameter index.
    */
    Statement & SetNow(const Object & param)
    {
        Parameter(SQMOD_GET_CREATED(*this), param).SetNow();
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind a null value at the specified parameter index.
    */
    Statement & SetNull(const Object & param)
    {
        Parameter(SQMOD_GET_CREATED(*this), param).SetNull();
        // Allow chaining of operations
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the values from an array starting at the specified index.
    */
    Statement & SetArray(Int32 idx, const Array & arr);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the values from an associative container.
    */
    Statement & SetTable(const Table & tbl);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the column with the specified name or index.
    */
    Object GetColumn(const Object & column) const
    {
        return Object(new Column(m_Handle, column));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a dynamic type.
    */
    Object GetValue(const Object & column) const
    {
        return Column(SQMOD_GET_CREATED(*this), column).GetValue();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a numeric type.
    */
    Object GetNumber(const Object & column) const
    {
        return Column(SQMOD_GET_CREATED(*this), column).GetNumber();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a native script integer.
    */
    SQInteger GetInteger(const Object & column) const
    {
        return Column(SQMOD_GET_CREATED(*this), column).GetInteger();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a native script floating point.
    */
    SQFloat GetFloat(const Object & column) const
    {
        return Column(SQMOD_GET_CREATED(*this), column).GetFloat();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a long integer.
    */
    Object GetLong(const Object & column) const
    {
        return Column(SQMOD_GET_CREATED(*this), column).GetLong();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a string.
    */
    Object GetString(const Object & column) const
    {
        return Column(SQMOD_GET_CREATED(*this), column).GetString();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a boolean.
    */
    bool GetBoolean(const Object & column) const
    {
        return Column(SQMOD_GET_CREATED(*this), column).GetBoolean();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a character.
    */
    SQChar GetChar(const Object & column) const
    {
        return Column(SQMOD_GET_CREATED(*this), column).GetChar();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a memory buffer.
    */
    Object GetBuffer(const Object & column) const
    {
        return Column(SQMOD_GET_CREATED(*this), column).GetBuffer();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a memory blob.
    */
    Object GetBlob(const Object & column) const
    {
        return Column(SQMOD_GET_CREATED(*this), column).GetBlob();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the row as an array container.
    */
    Array GetArray() const
    {
        return GetArray(0, SQMOD_GET_CREATED(*this)->mColumns);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the row as an array container.
    */
    Array GetArray(Int32 min) const
    {
        return GetArray(min, SQMOD_GET_CREATED(*this)->mColumns);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the row as an array container.
    */
    Array GetArray(Int32 min, Int32 max) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the row as an associative container.
    */
    Table GetTable() const
    {
        return GetTable(0, SQMOD_GET_CREATED(*this)->mColumns);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the row as an associative container.
    */
    Table GetTable(Int32 min) const
    {
        return GetTable(min, SQMOD_GET_CREATED(*this)->mColumns);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the row as an associative container.
    */
    Table GetTable(Int32 min, Int32 max) const;
};

} // Namespace:: SqMod

#endif // _SQSQLITE_STATEMENT_HPP
