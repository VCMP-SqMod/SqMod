#ifndef _SQJSON_COMMON_HPP_
#define _SQJSON_COMMON_HPP_

// ------------------------------------------------------------------------------------------------
#include "ModBase.hpp"

// ------------------------------------------------------------------------------------------------
#include <squirrel.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * SOFTWARE INFORMATION
*/
#define SQJSON_NAME "Squirrel JSON Module"
#define SQJSON_AUTHOR "Sandu Liviu Catalin (S.L.C)"
#define SQJSON_COPYRIGHT "Copyright (C) 2016 Sandu Liviu Catalin"
#define SQJSON_HOST_NAME "SqModJSONHost"
#define SQJSON_VERSION 001
#define SQJSON_VERSION_STR "0.0.1"
#define SQJSON_VERSION_MAJOR 0
#define SQJSON_VERSION_MINOR 0
#define SQJSON_VERSION_PATCH 1

/* ------------------------------------------------------------------------------------------------
 * Retrieve the temporary buffer.
*/
SStr GetTempBuff();

/* ------------------------------------------------------------------------------------------------
 * Retrieve the size of the temporary buffer.
*/
Uint32 GetTempBuffSize();

/* ------------------------------------------------------------------------------------------------
 * Throw a formatted exception.
*/
void SqThrowF(CSStr str, ...);

/* ------------------------------------------------------------------------------------------------
 * Generate a formatted string.
*/
CSStr FmtStr(CSStr str, ...);

/* ------------------------------------------------------------------------------------------------
 * Implements RAII to restore the VM stack to it's initial size on function exit.
*/
struct StackGuard
{
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    StackGuard();

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    StackGuard(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~StackGuard();

private:

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    StackGuard(const StackGuard &);

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    StackGuard(StackGuard &&);

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    StackGuard & operator = (const StackGuard &);

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    StackGuard & operator = (StackGuard &&);

private:

    // --------------------------------------------------------------------------------------------
    HSQUIRRELVM m_VM; // The VM where the stack should be restored.
    Int32       m_Top; // The top of the stack when this instance was created.
};

/* ------------------------------------------------------------------------------------------------
 * Helper structure for retrieving a value from the stack as a string or a formatted string.
*/
struct StackStrF
{
    // --------------------------------------------------------------------------------------------
    CSStr       mPtr; // Pointer to the C string that was retrieved.
    SQInteger   mLen; // The string length if it could be retrieved.
    SQRESULT    mRes; // The result of the retrieval attempts.
    HSQOBJECT   mObj; // Strong reference to the string object.
    HSQUIRRELVM mVM; // The associated virtual machine.

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    StackStrF(HSQUIRRELVM vm, SQInteger idx, bool fmt = true);

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    StackStrF(const StackStrF & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    StackStrF(StackStrF && o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~StackStrF();

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    StackStrF & operator = (const StackStrF & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    StackStrF & operator = (StackStrF && o) = delete;
};

} // Namespace:: SqMod

#endif // _SQJSON_COMMON_HPP_
