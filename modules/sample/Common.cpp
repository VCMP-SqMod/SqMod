// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
int SampleFunction()
{
    OutputMessage("Hello from the sample plug-in function!");
    return rand();
}

// ------------------------------------------------------------------------------------------------
void SampleType::SampleMethod() const
{
    OutputMessage("I have the values %d and %d", m_MyVal, mMyNum);
}

} // Namespace:: SqMod
