#ifndef _SQSQLITE_STATEMENT_HPP
#define _SQSQLITE_STATEMENT_HPP

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"
#include "SqAPI.h"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class used to manage an SQLite statement.
*/
class Statement
{
private:

    // --------------------------------------------------------------------------------------------
    StmtHnd m_Handle; /* The handle to the managed database statement resource. */

    // --------------------------------------------------------------------------------------------
    static CSStr    s_BadParamI; /* Error message when failed to bind value to parameter index. */
    static CSStr    s_BadParamS; /* Error message when failed to bind value to parameter name. */

protected:

    /* --------------------------------------------------------------------------------------------
     * Validate the statement reference and throw an error if invalid.
    */
    bool Validate() const;

    /* --------------------------------------------------------------------------------------------
     * Validate the statement reference and index, and throw an error if they're invalid.
    */
    bool ValidateIndex(Int32 idx) const;

    /* --------------------------------------------------------------------------------------------
     * Validate the statement reference and row, and throw an error if they're invalid.
    */
    bool RowAvailable() const;

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Statement();

    /* --------------------------------------------------------------------------------------------
     * Construct a statement under the specified connection using the specified string.
    */
    Statement(const ConnHnd & connection, CSStr query);

    /* --------------------------------------------------------------------------------------------
     * Construct a statement under the specified connection using the specified string.
    */
    Statement(const Connection & connection, CSStr query);

    /* --------------------------------------------------------------------------------------------
     * Direct handle constructor.
    */
    Statement(const StmtHnd & h)
        : m_Handle(h)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Statement(const Statement & o)
        : m_Handle(o.m_Handle)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Statement()
    {
        /* Let the reference manager destroy the statement when necessary. */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Statement & operator = (const Statement & o)
    {
        m_Handle = o.m_Handle;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two connections.
    */
    bool operator == (const Statement & o) const
    {
        return (m_Handle.m_Hnd == o.m_Handle.m_Hnd);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two connections.
    */
    bool operator != (const Statement & o) const
    {
        return (m_Handle.m_Hnd != o.m_Handle.m_Hnd);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the raw connection handle.
    */
    operator sqlite3_stmt * ()
    {
        return (sqlite3_stmt *)m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the raw connection handle.
    */
    operator sqlite3_stmt * () const
    {
        return (sqlite3_stmt *)m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Statement & o) const
    {
        if (m_Handle.m_Hnd == o.m_Handle.m_Hnd)
            return 0;
        else if (m_Handle.m_Hnd > o.m_Handle.m_Hnd)
            return 1;
        else
            return -1;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        // Validate the handle
        if (m_Handle)
            return m_Handle->mQuery.c_str();
        // Request failed
        return _SC("");
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
        return (bool)m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated statement handle.
    */
    const StmtHnd & GetHandle() const
    {
        return m_Handle;
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to this statement handle.
    */
    Uint32 GetRefCount() const
    {
        return m_Handle.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated database connection.
    */
    Connection GetConnection() const;

    /* --------------------------------------------------------------------------------------------
     * Release the reference to the associated database statement.
    */
    void Release()
    {
        m_Handle.Drop();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last received status code.
    */
    Int32 GetStatus() const
    {
        // Validate the handle
        if (Validate())
            return m_Handle->mStatus;
        // Request failed
        return -1;
    }

    /* --------------------------------------------------------------------------------------------
     * Return the numeric result code for the most recent failed API call (if any).
    */
    Int32 GetErrorCode() const
    {
        // Validate the handle
        if (Validate())
            return m_Handle.ErrNo();
        // Request failed
        return -1;
    }

    /* --------------------------------------------------------------------------------------------
     * Return the extended numeric result code for the most recent failed API call (if any).
    */
    Int32 GetExtendedErrorCode() const
    {
        // Validate the handle
        if (Validate())
            return m_Handle.ExErrNo();
        // Request failed
        return -1;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the message of the last received error code.
    */
    CSStr GetErrStr() const
    {
        // Validate the handle
        if (Validate())
            return m_Handle.ErrStr();
        // Request failed
        return _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * Return the last error message associated with this database connection.
    */
    CSStr GetErrMsg() const
    {
        // Validate the handle
        if (Validate())
            return m_Handle.ErrMsg();
        // Request failed
        return _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the amount of requested columns.
    */
    Int32 GetColumns() const
    {
        // Validate the handle
        if (Validate())
            return m_Handle->mColumns;
        // Request failed
        return -1;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the query string used to create this statement.
    */
    CSStr GetQuery() const
    {
        // Validate the handle
        if (Validate())
            return m_Handle->mQuery.c_str();
        // Request failed
        return _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * See if the last step retrieved a valid row.
    */
    bool GetGood() const
    {
        // Validate the handle
        if (Validate())
            return m_Handle->mGood;
        // Request failed
        return false;
    }

    /* --------------------------------------------------------------------------------------------
     * See if there are any steps left.
    */
    bool GetDone() const
    {
        // Validate the handle
        if (Validate())
            return m_Handle->mDone;
        // Request failed
        return false;
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
    void IndexBindA(Int32 idx, Array & arr);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the a integer value at the the specified parameter index.
    */
    void IndexBindI(Int32 idx, Int32 value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the a long integer value at the the specified parameter index.
    */
    void IndexBindL(Int32 idx, Object & value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the a native integer value at the the specified parameter index.
    */
    void IndexBindV(Int32 idx, SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the a floating point value at the the specified parameter index.
    */
    void IndexBindF(Int32 idx, SQFloat value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the a string value at the the specified parameter index.
    */
    void IndexBindS(Int32 idx, CSStr value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the a boolean value at the the specified parameter index.
    */
    void IndexBindB(Int32 idx, bool value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the a null value at the the specified parameter index.
    */
    void IndexBindN(Int32 idx);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the values from an associative container.
    */
    void NameBindT(Table & tbl);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the a integer value at the specified parameter name.
    */
    void NameBindI(CSStr name, Int32 value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the a long integer value at the specified parameter name.
    */
    void NameBindL(CSStr name, Object & value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the a native integer value at the specified parameter name.
    */
    void NameBindV(CSStr name, SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the a floating point value at the specified parameter name.
    */
    void NameBindF(CSStr name, SQFloat value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the a string value at the specified parameter name.
    */
    void NameBindS(CSStr name, CSStr value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the a boolean value at the specified parameter name.
    */
    void NameBindB(CSStr name, bool value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the a null value at the specified parameter name.
    */
    void NameBindN(CSStr name);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the specified value at the specified parameter index.
    */
    void IndexBind(Int32 idx, Object & value);

    /* --------------------------------------------------------------------------------------------
     * Attempt to bind the specified value at the specified parameter name.
    */
    void NameBind(CSStr name, Object & value);

    /* --------------------------------------------------------------------------------------------
     * Fetch the value at the specifie column index.
    */
    Object FetchColumnIndex(Int32 idx) const;

    /* --------------------------------------------------------------------------------------------
     * Fetch the value at the specifie column name.
    */
    Object FetchColumnName(CSStr name) const;

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
     * Check whether a specific index is in range.
    */
    bool CheckIndex(Int32 idx) const;

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
    Column GetColumnByIndex(Int32 idx) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the column with the specified name.
    */
    Column GetColumnByName(CSStr name) const;
};

} // Namespace:: SqMod

#endif // _SQSQLITE_STATEMENT_HPP
