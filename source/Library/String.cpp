// ------------------------------------------------------------------------------------------------
#include "Library/String.hpp"
#include "Base/Shared.hpp"
#include "Base/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqstdstring.h>

// ------------------------------------------------------------------------------------------------
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// ------------------------------------------------------------------------------------------------
#include <algorithm>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
CSStr LeftStr(CSStr t, SQChar f, Uint32 w)
{
    // Is the specified width valid?
    if (!w)
        return _SC("");
    // Allocate a buffer with the requested width
    Buffer b(w + 1); // + null terminator
    // Is the specified string valid?
    if (!t || *t == 0)
        // Insert only the fill character
        memset(b.Data(), f, w);
    else
    {
        // Calculate the string length
        const Uint32 n = strlen(t);
        // Insert only the fill character first
        memset(b.Data(), f, w);
        // Overwrite with the specified string
        strncpy(b.Data(), t, n);
    }
    // End the resulted string
    b.At(w) = 0;
    // Return the resulted string
    return b.Get< SQChar >();
}

CSStr LeftStr(CSStr t, SQChar f, Uint32 w, Uint32 o)
{
    // Is the specified width valid?
    if (!w)
        return _SC("");
    // Is the specified offset within width range?
    else if (o > w)
        STHROWF("Offset is out of bounds");
    // Allocate a buffer with the requested width
    Buffer b(w + 1); // + null terminator
    // Is the specified string valid?
    if (!t || *t == 0)
        // Insert only the fill character
        memset(b.Data(), f, w);
    else
    {
        // Clculate the string length
        const Uint32 n = strlen(t);
        // Insert the fill character first
        memset(b.Data(), f, w);
        // Overwrite with the specified string
        strncpy(b.Data() + o, t, w - n);
    }
    // End the resulted string
    b.At(w) = 0;
    // Return the resulted string
    return b.Get();
}

// ------------------------------------------------------------------------------------------------
CSStr RightStr(CSStr t, SQChar f, Uint32 w)
{
    // Is the specified width valid?
    if (!w)
        return _SC("");
    // Allocate a buffer with the requested width
    Buffer b(w + 1); // + null terminator
    // Is the specified string valid?
    if (!t || *t == 0)
        // Insert only the fill character
        memset(b.Data(), f, w);
    else
    {
        // Calculate the string length
        const Uint32 n = strlen(t);
        // Insert the fill character first
        memset(b.Data(), f, w);
        // Overwrite with the specified string
        if (n >= w)
            strncpy(b.Data(), t, w);
        else
            strncpy(b.Data() + (w - n), t, n);
    }
    // End the resulted string
    b.At(w) = 0;
    // Return the resulted string
    return b.Get< SQChar >();
}

CSStr RightStr(CSStr t, SQChar f, Uint32 w, Uint32 o)
{
    // Is the specified width valid?
    if (!w)
        return _SC("");
    // Is the specified offset within width range?
    else if (o > w)
        STHROWF("Offset is out of bounds");
    // Allocate a buffer with the requested width
    Buffer b(w + 1); // + null terminator
    // Is the specified string valid?
    if (!t || *t == 0)
        // Insert only the fill character
        memset(b.Data(), f, w);
    else
    {
        // Calculate the string length
        const Uint32 n = strlen(t);
        // Insert the fill character first
        memset(b.Data(), f, w);
        // Overwrite with the specified string
        if (n >= w || (n + o) >= w)
            strncpy(b.Data(), t, w - o);
        else
            strncpy(b.Data() + ((w - n) - o), t, n);
    }
    // End the resulted string
    b.At(w) = 0;
    // Return the resulted string
    return b.Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
CSStr CenterStr(CSStr t, SQChar f, Uint32 w)
{
    // Is the specified width valid?
    if (!w)
        return _SC("");
    // Allocate a buffer with the requested width
    Buffer b(w + 1); // + null terminator
    // Is the specified string valid?
    if (!t || *t == 0)
        // Insert only the fill character
        memset(b.Data(), f, w);
    else
    {
        // Calculate the string length
        const Uint32 n = strlen(t);
        // Insert only the fill character first
        memset(b.Data(), f, w);
        // Overwrite with the specified string
        strncpy(b.Data() + ((w/2) - (n/2)), t, n);
    }
    // End the resulted string
    b.At(w) = 0;
    // Return the resulted string
    return b.Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
CSStr StrJustAlphaNum(CSStr str)
{
    // See if we actually have something to search for
    if(!str || *str == 0)
        return _SC("");
    // Calculate the string length
    Uint32 size = strlen(str);
    // Obtain a temporary buffer
    Buffer b(size + 1); // + null terminator
    // Resulted string size
    Uint32 n = 0;
    // Currently processed character
    SQChar c = 0;
    // Process characters
    while ((c = *(str++)) != '\0')
    {
        // Is this an alpha-numeric character?
        if (isalnum(c) != 0)
            // Save it and move to the next one
            b.At(n++) = c;
    }
    // End the resulted string
    b.At(n) = 0;
    // Return the string
    return b.Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
CSStr StrToLowercase(CSStr str)
{
    // See if we actually have something to search for
    if(!str || *str == 0)
        return _SC("");
    // Calculate the string length
    Uint32 size = strlen(str);
    // Obtain a temporary buffer
    Buffer b(size + 1); // + null terminator
    // Resulted string size
    Uint32 n = 0;
    // Currently processed character
    SQChar c = 0;
    // Process characters
    while ((c = *(str++)) != '\0')
        // Convert it and move to the next one
        b.At(n++) = tolower(c);
    // End the resulted string
    b.At(n) = 0;
    // Return the string
    return b.Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
CSStr StrToUppercase(CSStr str)
{
    // See if we actually have something to search for
    if(!str || *str == 0)
        return _SC("");
    // Calculate the string length
    Uint32 size = strlen(str); // + null terminator
    // Obtain a temporary buffer
    Buffer b(size + 1); // + null terminator
    // Resulted string size
    Uint32 n = 0;
    // Currently processed character
    SQChar c = 0;
    // Process characters
    while ((c = *(str++)) != 0)
        // Convert it and move to the next one
        b.At(n++) = toupper(c);
    // End the resulted string
    b.At(n) = 0;
    // Return the string
    return b.Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
static CSStr FromArray(Array & arr)
{
    // Determine array size
    const Int32 length = static_cast< Int32 >(arr.Length());
    // Obtain a temporary buffer
    Buffer b(length * sizeof(Int32));
    // Get array elements as integers
    arr.GetArray< Int32 >(b.Get< Int32 >(), length);
    // Overwrite integers with characters
    for (Int32 n = 0; n < length; ++n)
        b.At(n) = static_cast< SQChar >(b.At< Int32 >(n));
    // Terminate the resulted string
    b.At(length) = 0;
    // Return the string
    return b.Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
static SQInteger StdPrintF(HSQUIRRELVM vm)
{
    CStr msg = NULL;
    SQInteger length = 0;
    // Attempt to run the specified format and save the result
    const SQRESULT res = sqstd_format(vm, 2, &length, &msg);
    // Validate the result for errors and propagate them to the VM
    if(SQ_FAILED(res))
        return res;
    // Send the resulted string to console as a user message
    LogUsr("%s", msg);
    // This function doesn't return anything
    return 0;
}

// ================================================================================================
void Register_String(HSQUIRRELVM vm)
{
    Table strns(vm);

    strns.Func(_SC("FromArray"), &FromArray)
    .Overload< CSStr (*)(CSStr, SQChar, Uint32) >(_SC("Left"), &LeftStr)
    .Overload< CSStr (*)(CSStr, SQChar, Uint32, Uint32) >(_SC("Left"), &LeftStr)
    .Overload< CSStr (*)(CSStr, SQChar, Uint32) >(_SC("Right"), &RightStr)
    .Overload< CSStr (*)(CSStr, SQChar, Uint32, Uint32) >(_SC("Right"), &RightStr)
    .Func(_SC("Center"), &CenterStr)
    .Func(_SC("JustAlphaNum"), &StrJustAlphaNum)
    .Func(_SC("Lowercase"), &StrToLowercase)
    .Func(_SC("Uppercase"), &StrToUppercase);

    RootTable(vm).Bind(_SC("SqStr"), strns);
    RootTable(vm).SquirrelFunc(_SC("printf"), &StdPrintF);
}

} // Namespace:: SqMod
