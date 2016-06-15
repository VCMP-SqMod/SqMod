#ifndef _SQSQLITE_COLUMN_HPP_
#define _SQSQLITE_COLUMN_HPP_

// ------------------------------------------------------------------------------------------------
#include "Handle/Statement.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Tsed to manage and interact with statement columns.
*/
class Column
{
    // --------------------------------------------------------------------------------------------
    friend class Statement;

private:

    // --------------------------------------------------------------------------------------------
    Int32       m_Index; // The index of the managed column.

    // --------------------------------------------------------------------------------------------
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
     * Validate the statement reference and row, and throw an error if they're invalid.
    */
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void ValidateRow(CCStr file, Int32 line) const;
#else
    void ValidateRow() const;
#endif // _DEBUG

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Column(const StmtRef & stmt, Int32 idx)
        : m_Index(idx), m_Handle(stmt)
    {
        /* ... */
    }

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
     * Perform an equality comparison between two connection handles.
    */
    bool operator == (const Column & o) const
    {
        return (m_Index == o.m_Index);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two connection handles.
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
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Column & o) const
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
        return FmtStr(_SC("%d"), m_Index);
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
        return m_Handle; // An invalid statement means an invalid column
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to this statement handle.
    */
    Uint32 GetRefCount() const
    {
        return m_Handle.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the associated column index.
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
     * Release the reference to the associated database statement and index.
    */
    void Release()
    {
        m_Handle.Reset();
        m_Index = -1;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the associated column cel as a dynamic type.
    */
    Object GetValue() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the associated column cel as a numeric type.
    */
    Object GetNumber() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the associated column cel as a native script integer.
    */
    SQInteger GetInteger() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the associated column cel as a native script floating point.
    */
    SQFloat GetFloat() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the associated column cel as a long integer.
    */
    Object GetLong() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the associated column cel as a string.
    */
    Object GetString() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the associated column cel as a boolean.
    */
    bool GetBoolean() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the associated column cel as a character.
    */
    SQChar GetChar() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the associated column cel as a memory buffer.
    */
    Object GetBuffer() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the associated column cel as a memory blob.
    */
    Object GetBlob() const;

    /* --------------------------------------------------------------------------------------------
     * Check whether the associated column is null.
    */
    bool IsNull() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the name of the associated column index.
    */
    CSStr GetName() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the column origin name if the library was compiled with such feature.
    */
    CSStr GetOriginName() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the type identifier of the associated column index.
    */
    Int32 GetType() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the size in bytes of the associated column index.
    */
    Int32 GetBytes() const;
};

} // Namespace:: SqMod

#endif // _SQSQLITE_COLUMN_HPP_
