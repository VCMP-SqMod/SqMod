#ifndef _SQIRC_COMMON_HPP_
#define _SQIRC_COMMON_HPP_

// ------------------------------------------------------------------------------------------------
#include "ModBase.hpp"

// ------------------------------------------------------------------------------------------------
extern "C" {
    struct SQVM;
    typedef struct SQVM* HSQUIRRELVM;
} /*extern "C"*/

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
    Int32       m_Top; /* The top of the stack when this instance was created. */
    HSQUIRRELVM m_VM; /* The VM where the stack should be restored. */
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
