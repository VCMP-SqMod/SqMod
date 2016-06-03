#ifndef _SQSAMPLE_COMMON_HPP_
#define _SQSAMPLE_COMMON_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Utility.hpp"

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

/* --------------------------------------------------------------------------------------------
 * Sample Plug-in API
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
