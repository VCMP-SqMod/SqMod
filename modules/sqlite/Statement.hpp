#ifndef _SQSQLITE_STATEMENT_HPP
#define _SQSQLITE_STATEMENT_HPP

// ------------------------------------------------------------------------------------------------
#include "Handle/Statement.hpp"

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
    void ValidateParameter(Int32 idx, CCStr file, Int32 line) const;
#else
    void ValidateParameter(Int32 idx) const;
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
        GET_VALID_HND(*this)->Create(query);
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
        return GET_VALID_HND(*this)->mStatus;
    }

    /* --------------------------------------------------------------------------------------------
     * Return the numeric result code for the most recent failed API call (if any).
    */
    Int32 GetErrorCode() const
    {
        return GET_VALID_HND(*this)->ErrNo();
    }

    /* --------------------------------------------------------------------------------------------
     * Return the extended numeric result code for the most recent failed API call (if any).
    */
    Int32 GetExtendedErrorCode() const
    {
        return GET_VALID_HND(*this)->ExErrNo();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the message of the last received error code.
    */
    CSStr GetErrStr() const
    {
        return GET_VALID_HND(*this)->ErrStr();
    }

    /* --------------------------------------------------------------------------------------------
     * Return the last error message associated with this database connection.
    */
    CSStr GetErrMsg() const
    {
        return GET_VALID_HND(*this)->ErrMsg();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the amount of requested columns.
    */
    Int32 GetColumns() const
    {
        return GET_VALID_HND(*this)->mColumns;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the amount of specified parameters.
    */
    Int32 GetParameters() const
    {
        return GET_VALID_HND(*this)->mParameters;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the query string used to create this statement.
    */
    const String & GetQuery() const
    {
        return GET_VALID_HND(*this)->mQuery;
    }

    /* --------------------------------------------------------------------------------------------
     * See if the last step retrieved a valid row.
    */
    bool GetGood() const
    {
        return GET_CREATED_HND(*this)->mGood;
    }

    /* --------------------------------------------------------------------------------------------
     * See if there are any steps left.
    */
    bool GetDone() const
    {
        return GET_CREATED_HND(*this)->mDone;
    }

    /* --------------------------------------------------------------------------------------------
     * Reset the statement back to its initial position to be stepped again.
    */
    void Reset();

    /* --------------------------------------------------------------------------------------------
     * Clear any values binded to this statement.
    */
    void Clear();

    /* --------------------------------------------------------------------------------------------
     * Execute this statement and don't expect any rows to be returned.
    */
    Int32 Exec();

    /* --------------------------------------------------------------------------------------------
     * Step the statement and expect a row to be returned.
    */
    bool Step();

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the values from an array starting at the specified index.
    */
    Statement & IndexBindArray(Int32 idx, const Array & arr);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the a numeric value at the the specified parameter index.
    */
    Statement & IndexBindValue(Int32 idx, Int32 value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the a long integer value at the the specified parameter index.
    */
    Statement & IndexBindLong(Int32 idx, const Object & value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the a native integer value at the the specified parameter index.
    */
    Statement & IndexBindInteger(Int32 idx, SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the a floating point value at the the specified parameter index.
    */
    Statement & IndexBindFloat(Int32 idx, SQFloat value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the a string value at the the specified parameter index.
    */
    Statement & IndexBindString(Int32 idx, CSStr value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the a boolean value at the the specified parameter index.
    */
    Statement & IndexBindBool(Int32 idx, bool value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the a null value at the the specified parameter index.
    */
    Statement & IndexBindNull(Int32 idx);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the values from an associative container.
    */
    Statement & NameBindTable(const Table & tbl);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the a integer value at the specified parameter name.
    */
    Statement & NameBindValue(CSStr name, Int32 value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the a long integer value at the specified parameter name.
    */
    Statement & NameBindLong(CSStr name, const Object & value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the a native integer value at the specified parameter name.
    */
    Statement & NameBindInteger(CSStr name, SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the a floating point value at the specified parameter name.
    */
    Statement & NameBindFloat(CSStr name, SQFloat value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the a string value at the specified parameter name.
    */
    Statement & NameBindString(CSStr name, CSStr value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the a boolean value at the specified parameter name.
    */
    Statement & NameBindBool(CSStr name, bool value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the a null value at the specified parameter name.
    */
    Statement & NameBindNull(CSStr name);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the specified value at the specified parameter index.
    */
    Statement & IndexBind(Int32 idx, const Object & value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the specified value at the specified parameter name.
    */
    Statement & NameBind(CSStr name, const Object & value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the specified value at the specified parameter.
    */
    Statement & Bind(const Object & param, const Object & value);

    /* --------------------------------------------------------------------------------------------
     * Fetch the value at the specific column index.
    */
    Object FetchColumnIndex(Int32 idx) const;

    /* --------------------------------------------------------------------------------------------
     * Fetch the value at the specific column name.
    */
    Object FetchColumnName(CSStr name) const;

    /* --------------------------------------------------------------------------------------------
     * Fetch the value at the specific column.
    */
    Object FetchColumn(const Object & column) const;

    /* --------------------------------------------------------------------------------------------
     * Fetch the row as an array container.
    */
    Array FetchArray() const;

    /* --------------------------------------------------------------------------------------------
     * Fetch the row as an array container.
    */
    Array FetchArray(Int32 min) const;

    /* --------------------------------------------------------------------------------------------
     * Fetch the row as an array container.
    */
    Array FetchArray(Int32 min, Int32 max) const;

    /* --------------------------------------------------------------------------------------------
     * Fetch the row as an associative container.
    */
    Table FetchTable() const;

    /* --------------------------------------------------------------------------------------------
     * Fetch the row as an associative container.
    */
    Table FetchTable(Int32 min) const;

    /* --------------------------------------------------------------------------------------------
     * Fetch the row as an associative container.
    */
    Table FetchTable(Int32 min, Int32 max) const;

    /* --------------------------------------------------------------------------------------------
     * Check whether a specific column index is in range.
    */
    bool CheckColumn(Int32 idx) const;

    /* --------------------------------------------------------------------------------------------
     * Check whether a specific parameter index is in range.
    */
    bool CheckParameter(Int32 idx) const;

    /* --------------------------------------------------------------------------------------------
     * Check whether the specified column is null.
    */
    bool IsColumnNull(Int32 idx) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the column index associated with the specified name.
    */
    Int32 GetColumnIndex(CSStr name) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the column name associated with the specified index.
    */
    CSStr GetColumnName(Int32 idx) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the column origin name if the library was compiled with such feature.
    */
    CSStr GetColumnOriginName(Int32 idx) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the type identifier of the column associated with the specified index.
    */
    Int32 GetColumnType(Int32 idx) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the size in bytes of the column associated with the specified index.
    */
    Int32 GetColumnBytes(Int32 idx) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the column with the specified index.
    */
    Object GetColumnByIndex(Int32 idx) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the column with the specified name.
    */
    Object GetColumnByName(CSStr name) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the column with the specified name or index.
    */
    Object GetColumn(const Object & column) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the amount of columns available in the current row.
    */
    Int32 GetDataCount() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the parameter index associated with the specified name.
    */
    Int32 GetParameterIndex(CSStr name) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the parameter name associated with the specified index.
    */
    CSStr GetParameterName(Int32 idx) const;
};

} // Namespace:: SqMod

#endif // _SQSQLITE_STATEMENT_HPP
