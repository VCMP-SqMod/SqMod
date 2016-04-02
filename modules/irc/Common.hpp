#ifndef _SQIRC_COMMON_HPP_
#define _SQIRC_COMMON_HPP_

// ------------------------------------------------------------------------------------------------
#include "ModBase.hpp"

// ------------------------------------------------------------------------------------------------
#include <squirrel.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * SOFTWARE INFORMATION
*/
#define SQIRC_NAME "Squirrel IRC Module"
#define SQIRC_AUTHOR "Sandu Liviu Catalin (S.L.C)"
#define SQIRC_COPYRIGHT "Copyright (C) 2016 Sandu Liviu Catalin"
#define SQIRC_HOST_NAME "SqModIRCHost"
#define SQIRC_VERSION 001
#define SQIRC_VERSION_STR "0.0.1"
#define SQIRC_VERSION_MAJOR 0
#define SQIRC_VERSION_MINOR 0
#define SQIRC_VERSION_PATCH 1

/* ------------------------------------------------------------------------------------------------
 * Forward declarations.
*/
class Session;

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

/* ------------------------------------------------------------------------------------------------
 * Extract the name from the specified origin.
*/
CSStr GetNick(CSStr origin);

/* ------------------------------------------------------------------------------------------------
 * Extract the host from the specified origin.
*/
CSStr GetHost(CSStr target);

} // Namespace:: SqMod

#endif // _SQIRC_COMMON_HPP_
