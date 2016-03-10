// ------------------------------------------------------------------------------------------------
#include "Common.hpp"
#include "Module.hpp"

// ------------------------------------------------------------------------------------------------
#include <cfloat>
#include <climits>
#include <cstdlib>
#include <cstdarg>

// ------------------------------------------------------------------------------------------------
#include <sqrat.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
static SQChar g_Buffer[4096]; // Common buffer to reduce memory allocations.

// ------------------------------------------------------------------------------------------------
SStr GetTempBuff()
{
    return g_Buffer;
}

// ------------------------------------------------------------------------------------------------
Uint32 GetTempBuffSize()
{
    return sizeof(g_Buffer);
}

// ------------------------------------------------------------------------------------------------
void SqThrowF(CSStr str, ...)
{
    // Initialize the argument list
    va_list args;
    va_start (args, str);
    // Write the requested contents
    if (snprintf(g_Buffer, sizeof(g_Buffer), str, args) < 0)
        strcpy(g_Buffer, "Unknown error has occurred");
    // Release the argument list
    va_end(args);
    // Throw the exception with the resulted message
    throw Sqrat::Exception(g_Buffer);
}

// ------------------------------------------------------------------------------------------------
CSStr FmtStr(CSStr str, ...)
{
    // Initialize the argument list
    va_list args;
    va_start (args, str);
    // Write the requested contents
    if (snprintf(g_Buffer, sizeof(g_Buffer), str, args) < 0)
        g_Buffer[0] = 0; /* make sure the string is terminated */
    // Release the argument list
    va_end(args);
    // Return the data from the buffer
    return g_Buffer;
}

// ------------------------------------------------------------------------------------------------
StackGuard::StackGuard(HSQUIRRELVM vm)
    : m_Top(sq_gettop(vm)), m_VM(vm)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
StackGuard::~StackGuard()
{
    sq_pop(m_VM, sq_gettop(m_VM) - m_Top);
}

// ------------------------------------------------------------------------------------------------
DbRef::Pointer DbRef::Create()
{
    return reinterpret_cast< Pointer >(malloc(sizeof(Type)));
}

// ------------------------------------------------------------------------------------------------
void DbRef::Destroy(Pointer db)
{
    if (db)
        free(db);
}

// ------------------------------------------------------------------------------------------------
const char NumLimit< char >::Min = CHAR_MIN;
const signed char NumLimit< signed char >::Min = SCHAR_MIN;
const unsigned char NumLimit< unsigned char >::Min = 0;
const signed short NumLimit< signed short >::Min = SHRT_MIN;
const unsigned short NumLimit< unsigned short >::Min = 0;
const signed int NumLimit< signed int >::Min = INT_MIN;
const unsigned int NumLimit< unsigned int >::Min = 0;
const signed long NumLimit< signed long >::Min = LONG_MIN;
const unsigned long NumLimit< unsigned long >::Min = 0;
const signed long long NumLimit< signed long long >::Min = LLONG_MIN;
const unsigned long long NumLimit< unsigned long long >::Min = 0;
const float NumLimit< float >::Min = FLT_MIN;
const double NumLimit< double >::Min = DBL_MIN;
const long double NumLimit< long double >::Min = LDBL_MIN;

// ------------------------------------------------------------------------------------------------
const char NumLimit< char >::Max = CHAR_MAX;
const signed char NumLimit< signed char >::Max = SCHAR_MAX;
const unsigned char NumLimit< unsigned char >::Max = UCHAR_MAX;
const signed short NumLimit< signed short >::Max = SHRT_MAX;
const unsigned short NumLimit< unsigned short >::Max = USHRT_MAX;
const signed int NumLimit< signed int >::Max = INT_MAX;
const unsigned int NumLimit< unsigned int >::Max = UINT_MAX;
const signed long NumLimit< signed long >::Max = LONG_MAX;
const unsigned long NumLimit< unsigned long >::Max = ULONG_MAX;
const signed long long NumLimit< signed long long >::Max = LLONG_MAX;
const unsigned long long NumLimit< unsigned long long >::Max = ULLONG_MAX;
const float NumLimit< float >::Max = FLT_MAX;
const double NumLimit< double >::Max = DBL_MAX;
const long double NumLimit< long double >::Max = LDBL_MAX;

} // Namespace:: SqMod
