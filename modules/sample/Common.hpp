#ifndef _SQSAMPLE_COMMON_HPP_
#define _SQSAMPLE_COMMON_HPP_

// ------------------------------------------------------------------------------------------------
#include "ModBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * SOFTWARE INFORMATION
*/
#define SQSAMPLE_NAME "Squirrel Sample Module"
#define SQSAMPLE_AUTHOR "Sandu Liviu Catalin (S.L.C)"
#define SQSAMPLE_COPYRIGHT "Copyright (C) 2016 Sandu Liviu Catalin"
#define SQSAMPLE_HOST_NAME "SqModSampleHost"
#define SQSAMPLE_VERSION 001
#define SQSAMPLE_VERSION_STR "0.0.1"
#define SQSAMPLE_VERSION_MAJOR 0
#define SQSAMPLE_VERSION_MINOR 0
#define SQSAMPLE_VERSION_PATCH 1

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

/* --------------------------------------------------------------------------------------------
 * Sample Plugin API
*/

int SampleFunction();

class SampleType
{
private:

    int m_MyVal;

public:

    int mMyNum;

    SampleType()
        : m_MyVal(0), mMyNum(0)
    {

    }

    SampleType(int num)
        : m_MyVal(num * 2), mMyNum(num)
    {

    }

    int GetMyVal() const
    {
        return m_MyVal;
    }

    void SetMyVal(int val)
    {
        m_MyVal = val;
    }

    void SampleMethod() const;
};

} // Namespace:: SqMod

#endif // _SQSAMPLE_COMMON_HPP_
