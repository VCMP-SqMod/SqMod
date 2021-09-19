// ------------------------------------------------------------------------------------------------
#include "Library/UTF8.hpp"
#include "Core/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD static SQInteger SqStrLenUTF8(StackStrF & str)
{
    // Is the string empty or invalid?
    if (str.mLen <= 0)
    {
        return str.mLen; // Return same value
    }
    // Perform the request and return the result
    return static_cast< SQInteger >(utf8len(str.mPtr));
} 

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD static SQInteger SqStrNLenUTF8(SQInteger len, StackStrF & str)
{
    // Is the string empty or invalid?
    if (str.mLen <= 0)
    {
        return str.mLen; // Return same value
    }
    // Is the specified length within bounds?
    else if (std::abs(len) > str.mLen)
    {
        STHROWF("Specified range {} is out of bounds {}", len, str.mLen);
    }
    // Should we compute the length backwards?
    else if (len < 0)
    {
        len = str.mLen - std::abs(len); // Subtract from actual length
    }
    // Did we trim all of it?
    else if (len == 0)
    {
        return 0; // Nothing to search for
    }
    // Perform the request and return the result
    return static_cast< SQInteger >(utf8nlen(str.mPtr, static_cast< size_t >(len)));
}

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD static SQInteger SqStrChrUTF8(SQInteger chr, StackStrF & str)
{
    // Is the string empty or invalid?
    if (str.mLen <= 0)
    {
        return -1; // Let the user know that we could not search for this
    }
    // Look for the specified character in the string
    auto * p = reinterpret_cast< const SQChar * >(utf8chr(str.mPtr, static_cast< utf8_int32_t >(chr)));
    // Return the distance in bytes where the character was found
    return static_cast< SQInteger >(p - str.mPtr);
}

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD static SQInteger SqCaseCmpUTF8(StackStrF & a, StackStrF & b)
{
    // Is the left string invalid?
    if (a.mLen < 0)
    {
        STHROWF("Invalid string on the left {}", a.mLen);
    }
    // Is the right string invalid?
    else if (b.mLen < 0)
    {
        STHROWF("Invalid string on the right {}", b.mLen);
    }
    // Perform the request and return the result
    return utf8casecmp(a.mPtr, b.mPtr);
}

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD static SQInteger SqNCaseCmpUTF8(StackStrF & a, StackStrF & b)
{
    // Is the left string invalid?
    if (a.mLen < 0)
    {
        STHROWF("Invalid string on the left {}", a.mLen);
    }
    // Is the right string invalid?
    else if (b.mLen < 0)
    {
        STHROWF("Invalid string on the right {}", b.mLen);
    }
    // See if we can decide this based on length differences
    else if (a.mLen < b.mLen)
    {
        return -1;
    }
    else if (a.mLen > b.mLen)
    {
        return 1;
    }
    // Perform the request and return the result
    return utf8ncasecmp(a.mPtr, b.mPtr, static_cast< size_t >(a.mLen));
}

// ------------------------------------------------------------------------------------------------
SQMOD_NODISCARD static SQInteger SqCaseStrUTF8(StackStrF & hs, StackStrF & nd)
{
    // Is the haystack or needle string empty or invalid?
    if (hs.mLen <= 0 || nd.mLen <= 0)
    {
        return -1; // Let the user know that we could not search for this
    }
    // Look for the specified needle string in the haystack string
    auto * p = reinterpret_cast< const SQChar * >(utf8casestr(hs.mPtr, nd.mPtr));
    // Return the distance in bytes where the string was found
    return static_cast< SQInteger >(p - hs.mPtr);
}

// ================================================================================================
void Register_UTF8(HSQUIRRELVM vm)
{
    Table ns(vm);

    ns
    .FmtFunc(_SC("StrLen"), &SqStrLenUTF8)
    .FmtFunc(_SC("StrNLen"), &SqStrNLenUTF8)
    .FmtFunc(_SC("StrChr"), &SqStrChrUTF8)
    .FmtFunc(_SC("CaseCmp"), &SqCaseCmpUTF8)
    .FmtFunc(_SC("NCaseCmp"), &SqNCaseCmpUTF8)
    .FmtFunc(_SC("CaseStr"), &SqCaseStrUTF8)
    ;

    RootTable(vm).Bind(_SC("SqUTF8"), ns);
}

} // Namespace:: SqMod
