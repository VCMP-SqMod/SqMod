#ifndef _SQMYSQL_RESULTSET_HPP_
#define _SQMYSQL_RESULTSET_HPP_

// ------------------------------------------------------------------------------------------------
#include "Handle/Result.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Allows management and interaction with a result set handle.
*/
class ResultSet
{
private:

    // --------------------------------------------------------------------------------------------
    ResHnd  m_Handle; // The managed result set.

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
     * Base constructor.
    */
    ResultSet(const ResHnd & hnd)
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
    Int32 Cmp(const ResultSet & o) const;

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
     * Retrieve a signed 8 bit integer from a field.
    */
    SQInteger GetInt8(Uint32 idx) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve an unsigned 8 bit integer from a field.
    */
    SQInteger GetUint8(Uint32 idx) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve a signed 16 bit integer from a field.
    */
    SQInteger GetInt16(Uint32 idx) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve an unsigned 16 bit integer from a field.
    */
    SQInteger GetUint16(Uint32 idx) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve a signed 32 bit integer from a field.
    */
    SQInteger GetInt32(Uint32 idx) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve an unsigned 32 bit integer from a field.
    */
    SQInteger GetUint32(Uint32 idx) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve a signed 64 bit integer from a field.
    */
    Int64 GetInt64(Uint32 idx) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve an unsigned 64 bit integer from a field.
    */
    Uint64 GetUint64(Uint32 idx) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve a 32 bit floating point from a field.
    */
    SQFloat GetFloat32(Uint32 idx) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve a 64 bit floating point from a field.
    */
    SQFloat GetFloat64(Uint32 idx) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve a boolean from a field.
    */
    bool GetBoolean(Uint32 idx) const;

};

} // Namespace:: SqMod

#endif // _SQMYSQL_RESULTSET_HPP_
