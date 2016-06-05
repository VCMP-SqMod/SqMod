#ifndef _SQMYSQL_STATEMENT_HPP_
#define _SQMYSQL_STATEMENT_HPP_

// ------------------------------------------------------------------------------------------------
#include "Handle/Statement.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Allows management and interaction with a statement handle.
*/
class Statement
{
private:

    // --------------------------------------------------------------------------------------------
    StmtHnd m_Handle; // Reference to the managed statement.

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
     * Copy constructor.
    */
    Statement(const Statement & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Statement(Statement && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Statement() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Statement & operator = (const Statement & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Statement & operator = (Statement && o) = default;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Statement & o) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const;

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
     * Retrieve the currently associated statement connection.
    */
    Connection GetConnection() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the currently associated statement connection.
    */
    void SetConnection(const Connection & conn);

    /* --------------------------------------------------------------------------------------------
     * Execute the statement.
    */
    Int32 Execute();

    /* --------------------------------------------------------------------------------------------
     * Execute the statement.
    */
    Uint32 Insert();

    /* --------------------------------------------------------------------------------------------
     * Execute the statement.
    */
    ResultSet Query();

    /* --------------------------------------------------------------------------------------------
     * Assign a signed 8bit integer to a parameter.
    */
    void SetInt8(Uint32 idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign an unsigned 8bit integer to a parameter.
    */
    void SetUint8(Uint32 idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a signed 16bit integer to a parameter.
    */
    void SetInt16(Uint32 idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign an unsigned 16bit integer to a parameter.
    */
    void SetUint16(Uint32 idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a signed 32bit integer to a parameter.
    */
    void SetInt32(Uint32 idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign an unsigned 32bit integer to a parameter.
    */
    void SetUint32(Uint32 idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a signed 64bit integer to a parameter.
    */
    void SetInt64(Uint32 idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign an unsigned 64bit integer to a parameter.
    */
    void SetUint64(Uint32 idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a signed long integer to a parameter.
    */
    void SetSLongInt(Uint32 idx, Object & val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign an unsigned long integer to a parameter.
    */
    void SetULongInt(Uint32 idx, Object & val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a native integer to a parameter.
    */
    void SetInteger(Uint32 idx, SQInteger val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a 32bit floating point to a parameter.
    */
    void SetFloat32(Uint32 idx, SQFloat val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a 64bit floating point to a parameter.
    */
    void SetFloat64(Uint32 idx, SQFloat val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a native float to a parameter.
    */
    void SetFloat(Uint32 idx, SQFloat val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a boolean to a parameter.
    */
    void SetBoolean(Uint32 idx, bool val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a date to a parameter.
    */
    void SetDate(Uint32 idx, Object & val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a time to a parameter.
    */
    void SetTime(Uint32 idx, Object & val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a date and time to a parameter.
    */
    void SetDatetime(Uint32 idx, Object & val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a string to a parameter.
    */
    void SetString(Uint32 idx, CSStr val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign an enumeration to a parameter.
    */
    void SetEnum(Uint32 idx, CSStr val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign an enumeration to a parameter.
    */
    void SetSet(Uint32 idx, CSStr val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a blob to a parameter.
    */
    void SetBlob(Uint32 idx, Object & val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a buffer to a paramete.
    */
    void SetData(Uint32 idx, Object & val) const;

    /* --------------------------------------------------------------------------------------------
     * Assign a null to a parameter.
    */
    void SetNull(Uint32 idx) const;

};
} // Namespace:: SqMod

#endif // _SQMYSQL_STATEMENT_HPP_
