#ifndef _BASE_STACK_HPP_
#define _BASE_STACK_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqrat.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Attempt to pop the value at the specified index on the stack as a native integer.
*/
SQInteger PopStackInteger(HSQUIRRELVM vm, SQInteger idx);

/* ------------------------------------------------------------------------------------------------
 * Attempt to pop the value at the specified index on the stack as a native float.
*/
SQFloat PopStackFloat(HSQUIRRELVM vm, SQInteger idx);

/* ------------------------------------------------------------------------------------------------
 * Attempt to pop the value at the specified index on the stack as a signed long integer.
*/
Int64 PopStackLong(HSQUIRRELVM vm, SQInteger idx);

/* ------------------------------------------------------------------------------------------------
 * Attempt to pop the value at the specified index on the stack as an unsigned long integer.
*/
Uint64 PopStackULong(HSQUIRRELVM vm, SQInteger idx);

/* ------------------------------------------------------------------------------------------------
 * Create a script string object from a buffer.
*/
Object BufferToStrObj(const Buffer & b);

/* ------------------------------------------------------------------------------------------------
 * Create a script string object from a portion of a buffer.
*/
Object BufferToStrObj(const Buffer & b, Uint32 size);

/* ------------------------------------------------------------------------------------------------
 * Implements RAII to restore the VM stack to it's initial size on function exit.
*/
struct StackGuard
{
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    StackGuard()
        : m_VM(DefaultVM::Get()), m_Top(sq_gettop(m_VM))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    StackGuard(HSQUIRRELVM vm)
        : m_VM(vm), m_Top(sq_gettop(vm))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    StackGuard(const StackGuard &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move constructor. (disabled)
    */
    StackGuard(StackGuard &&) = delete;

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~StackGuard()
    {
        sq_pop(m_VM, sq_gettop(m_VM) - m_Top);
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    StackGuard & operator = (const StackGuard &) = delete;

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator. (disabled)
    */
    StackGuard & operator = (StackGuard &&) = delete;

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
 * Create a script object from the specified value on the default VM.
*/
template < typename T > Object MakeObject(const T & v)
{
    // Remember the current stack size
    const StackGuard sg;
    // Transform the specified value into a script object
    PushVar< T >(DefaultVM::Get(), v);
    // Get the object from the stack and return it
    return Var< Object >(DefaultVM::Get(), -1).value;
}

/* ------------------------------------------------------------------------------------------------
 * Create a script object from the specified value on the specified VM.
*/
template < typename T > Object MakeObject(HSQUIRRELVM vm, const T & v)
{
    // Remember the current stack size
    const StackGuard sg;
    // Transform the specified value into a script object
    PushVar< T >(vm, v);
    // Get the object from the stack and return it
    return Var< Object >(vm, -1).value;
}

} // Namespace:: SqMod

#endif // _BASE_STACK_HPP_
