#ifndef _SQSQLITE_COLUMN_HPP_
#define _SQSQLITE_COLUMN_HPP_

// ------------------------------------------------------------------------------------------------
#include "Handle/Statement.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Helper class used to manage statement columns.
*/
class Column
{
    // --------------------------------------------------------------------------------------------
    friend class Statement;

private:

    // --------------------------------------------------------------------------------------------
    Int32       m_Index; // The index of the managed column.

    // --------------------------------------------------------------------------------------------
    StmtHnd     m_Stmt; // The statement where the column exist.

    /* --------------------------------------------------------------------------------------------
     * Validate the statement reference and index, and throw an error if they're invalid.
    */
    void Validate() const;

    /* --------------------------------------------------------------------------------------------
     * Validate the statement reference, index and row, and throw an error if they're invalid.
    */
    void ValidateRow() const;

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    Column(const StmtHnd & stmt, Int32 idx)
        : m_Index(idx), m_Stmt(stmt)
    {
        /* ... */
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor (null).
    */
    Column()
        : m_Index(-1), m_Stmt()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Column(const Column & o)
        : m_Index(o.m_Index), m_Stmt(o.m_Stmt)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Column()
    {
        /* Let the reference manager destroy the statement when necessary. */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Column operator = (const Column & o)
    {
        m_Index = o.m_Index;
        m_Stmt = o.m_Stmt;
        return *this;
    }

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
        if (m_Stmt == o.m_Stmt)
        {
            return 0;
        }
        else if (m_Stmt.HndPtr() > o.m_Stmt.HndPtr())
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
        return (bool)m_Stmt; // An invalid statement means an invalid column
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to this statement handle.
    */
    Uint32 GetRefCount() const
    {
        return m_Stmt.Count();
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
        m_Stmt = StmtHnd();
        m_Index = -1;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the associated column cel as a 32bit integer.
    */
    Int32 GetNumber() const;

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
     * Retrieve the value inside the associated column cel as a memory blob.
    */
    Object GetBlob() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value inside the associated column cel as a character.
    */
    SQChar GetChar() const;

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
