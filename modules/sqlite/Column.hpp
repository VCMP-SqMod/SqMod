#ifndef _SQSQLITE_COLUMN_HPP_
#define _SQSQLITE_COLUMN_HPP_

// ------------------------------------------------------------------------------------------------
#include "Handle/Statement.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Used to manage and interact with statement columns.
*/
class Column
{
    // --------------------------------------------------------------------------------------------
    friend class Statement;

private:

    // --------------------------------------------------------------------------------------------
    Int32       m_Index; // The index of the managed column.
    StmtRef     m_Handle; // The statement where the column exist.

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
     * Validate the statement reference and row, and throw an error if they're invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateRow(CCStr file, Int32 line) const;
#else
    void ValidateRow() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Modify the index to the specified value.
    */
    void SetIndex(Int32 idx)
    {
        // Assign the index with a failsafe to invalid on error
        AutoAssign< Int32 > aa(m_Index, -1, idx);
        // Validate the obtained column index
        SQMOD_VALIDATE_COLUMN(*this, idx);
        // Don't fall back to the invalid index anymore
        aa.Set(idx);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the index to the specified value.
    */
    void SetIndex(CSStr name)
    {
        SetIndex(SQMOD_GET_CREATED(*this)->GetColumnIndex(name));
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the index to the specified value.
    */
    void SetIndex(const Object & column);

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor (null).
    */
    Column()
        : m_Index(-1), m_Handle()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * No column constructor.
    */
    Column(const StmtRef & stmt)
        : m_Index(-1), m_Handle(stmt)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Index constructor.
    */
    Column(const StmtRef & stmt, Int32 idx)
        : m_Index(idx), m_Handle(stmt)
    {
        SQMOD_VALIDATE_COLUMN(*this, m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Name constructor.
    */
    Column(const StmtRef & stmt, CSStr name)
        : m_Index(stmt ? stmt->GetColumnIndex(name) : -1), m_Handle(stmt)
    {
        SQMOD_VALIDATE_COLUMN(*this, m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Dynamic constructor.
    */
    Column(const StmtRef & stmt, const Object & column)
        : m_Index(-1), m_Handle(stmt)
    {
        if (!m_Handle)
        {
            STHROWF("Invalid SQLite statement reference");
        }
        // Extract the index
        SetIndex(column);
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Column(const Column & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Column(Column && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Column & operator = (const Column & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Column & operator = (Column && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two table column indexes.
    */
    bool operator == (const Column & o) const
    {
        return (m_Index == o.m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two table column indexes.
    */
    bool operator != (const Column & o) const
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
            val = sqlite3_column_name(m_Handle->mPtr, m_Index);
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
     * See whether the column is valid.
    */
    bool IsValid() const
    {
        return m_Handle; // An invalid statement means an invalid column
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to the associated statement handle.
    */
    Uint32 GetRefCount() const
    {
        return m_Handle.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the referenced column index.
    */
    Int32 GetIndex() const
    {
        return m_Index;
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
     * Release the reference to the referenced database statement and column index.
    */
    void Release()
    {
        m_Handle.Reset();
        m_Index = -1;
    }

    /* --------------------------------------------------------------------------------------------
     * Check whether the referenced column is null.
    */
    bool IsNull() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the name of the referenced column index.
    */
    CSStr GetName() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the column origin name if the library was compiled with such feature.
    */
    CSStr GetOriginName() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the type identifier of the referenced column index.
    */
    Int32 GetType() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the size in bytes of the referenced column index.
    */
    Int32 GetBytes() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a dynamic type.
    */
    Object GetValue() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a numeric type.
    */
    Object GetNumber() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a native script integer.
    */
    SQInteger GetInteger() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a native script floating point.
    */
    SQFloat GetFloat() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a long integer.
    */
    Object GetLong() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a string.
    */
    Object GetString() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a boolean.
    */
    bool GetBoolean() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a character.
    */
    SQChar GetChar() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a memory buffer.
    */
    Object GetBuffer() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the referenced column as a memory blob.
    */
    Object GetBlob() const;
};

} // Namespace:: SqMod

#endif // _SQSQLITE_COLUMN_HPP_
