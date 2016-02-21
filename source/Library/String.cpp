// ------------------------------------------------------------------------------------------------
#include "Library/String.hpp"
#include "Base/Shared.hpp"
#include "Base/Buffer.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqstdstring.h>

// ------------------------------------------------------------------------------------------------
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ------------------------------------------------------------------------------------------------
#include <algorithm>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
CSStr LeftStr(CSStr t, SQChar f, Uint32 w)
{
    // Allocate a buffer with the requested width
    Buffer b(w * sizeof(SQChar));
    Uint32 n = !t ? 0 : strlen(t);
    // Populate the entire buffer with the fill character
    memset(b.Data(), f, w * sizeof(SQChar));
    // Is the specified width valid?
    if (w >= b.Size< SQChar >())
    {
        SqThrow("Invalid width specified: %d > %d", w, b.Size< SQChar >());
        w = 0;
    }
    // Is the specified string valid?
    else if (n == 0)
    {
        SqThrow("Invalid string length: %d < 0", n);
    }
    // Is the specified string within width range?
    else if (n > w)
    {
        SqThrow("String is out of bounds: %d > %d", n, w);
    }
    // Insert the specified string
    else
    {
        strncpy(b.Data(), t, n);
    }
    // End the resulted string
    b.At< SQChar >(w) = 0;
    // Return the resulted string
    return b.Get< SQChar >();
}

CSStr LeftStr(CSStr t, SQChar f, Uint32 w, Uint32 o)
{
    // Allocate a buffer with the requested width
    Buffer b(w * sizeof(SQChar));
    Uint32 n = !t ? 0 : strlen(t);
    // Populate the entire buffer with the fill character
    memset(b.Data(), f, w * sizeof(SQChar));
    // Is the specified width valid?
    if (w >= b.Size< SQChar >())
    {
        SqThrow("Invalid width specified: %d > %d", w, b.Size< SQChar >());
        w = 0;
    }
    // Is the specified string valid?
    else if (n == 0)
    {
        SqThrow("Invalid string length: %d < 0", n);
    }
    // Is the specified string within width range?
    else if ((o+n) > w)
    {
        SqThrow("String is out of bounds: (%d+%d) > %d", o, n, w);
    }
    // Insert the specified string
    else
    {
        strncpy(b.Get< SQChar >() + o, t, n);
    }
    // End the resulted string
    b.At< SQChar >(w) = 0;
    // Return the resulted string
    return b.Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
CSStr RightStr(CSStr t, SQChar f, Uint32 w)
{
    // Allocate a buffer with the requested width
    Buffer b(w * sizeof(SQChar));
    Uint32 n = !t ? 0 : strlen(t);
    // Populate the entire buffer with the fill character
    memset(b.Data(), f, w * sizeof(SQChar));
    // Is the specified width valid?
    if (w >= b.Size< SQChar >())
    {
        SqThrow("Invalid width specified: %d > %d", w, b.Size< SQChar >());
        w = 0;
    }
    // Is the specified string valid?
    else if (n == 0)
    {
        SqThrow("Invalid string length: %d < 0", n);
    }
    // Is the specified string within width range?
    else if (n > w)
    {
        SqThrow("String is out of bounds: %d > %d", n, w);
    }
    // Insert the specified string
    else
    {
        strncpy(b.Get< SQChar >() + (w-n), t, n);
    }
    // End the resulted string
    b.At< SQChar >(w) = 0;
    // Return the resulted string
    return b.Get< SQChar >();
}

CSStr RightStr(CSStr t, SQChar f, Uint32 w, Uint32 o)
{
    // Allocate a buffer with the requested width
    Buffer b(w * sizeof(SQChar));
    Uint32 n = !t ? 0 : strlen(t);
    // Populate the entire buffer with the fill character
    memset(b.Data(), f, w * sizeof(SQChar));
    // Is the specified width valid?
    if (w >= b.Size< SQChar >())
    {
        SqThrow("Invalid width specified: %d > %d", w, b.Size< SQChar >());
        w = 0;
    }
    // Is the specified string valid?
    else if (n == 0)
    {
        SqThrow("Invalid string length: %d < 0", n);
    }
    // Is the specified string within width range?
    else if ((n+o) > w)
    {
        SqThrow("String is out of bounds: (%d+%d) > %d", n, o, w);
    }
    // Insert the specified string
    else
    {
        strncpy(b.Get< SQChar >() + ((w-n)-o), t, n);
    }
    // End the resulted string
    b.At< SQChar >(w) = 0;
    // Return the resulted string
    return b.Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
CSStr CenterStr(CSStr t, SQChar f, Uint32 w)
{
    // Allocate a buffer with the requested width
    Buffer b(w * sizeof(SQChar));
    Uint32 n = !t ? 0 : strlen(t);
    // Populate the entire buffer with the fill character
    memset(b.Data(), f, w * sizeof(SQChar));
    // Is the specified width valid?
    if (w >= b.Size< SQChar >())
    {
        SqThrow("Invalid width specified: %d > %d", w, b.Size< SQChar >());
        w = 0;
    }
    // Is the specified string valid?
    else if (n == 0)
    {
        SqThrow("Invalid string length: %d < 0", n);
    }
    // Is the specified string within width range?
    else if (n > w)
    {
        SqThrow("String is out of bounds: %d > %d", n, w);
    }
    // Insert the specified string
    else
    {
        strncpy(b.Get< SQChar >() + (w/2 - n/2), t, n);
    }
    // End the resulted string
    b.At< SQChar >(w) = 0;
    // Return the resulted string
    return b.Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
CSStr StrJustAlphaNum(CSStr str)
{
    Uint32 size = 0;
    // See if we actually have something to search for
    if(!str || (size = strlen(str)) <= 0)
    {
        return _SC("");
    }
    // Obtain a temporary buffer
    Buffer b(size);
    // Resulted string size
    Uint32 n = 0;
    // Currently processed character
    SQChar c = 0;
    // Process characters
    while ((c = *(str++)) != 0)
    {
        // Is this an alpha-numeric character?
        if (isalnum(c) != 0)
        {
            // Save it and move to the next one
            b.At< SQChar >(n++) = c;
        }
    }
    // End the resulted string
    b.At< SQChar >(n) = 0;
    // Return the string
    return b.Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
CSStr StrToLowercase(CSStr str)
{
    Uint32 size = 0;
    // See if we actually have something to search for
    if(!str || (size = strlen(str)) <= 0)
    {
        return _SC("");
    }
    // Obtain a temporary buffer
    Buffer b(size);
    // Resulted string size
    Uint32 n = 0;
    // Currently processed character
    SQChar c = 0;
    // Process characters
    while ((c = *(str++)) != 0)
    {
        // Convert it and move to the next one
        b.At< SQChar >(n++) = tolower(c);
    }
    // End the resulted string
    b.At< SQChar >(n) = 0;
    // Return the string
    return b.Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
CSStr StrToUppercase(CSStr str)
{
    Uint32 size = 0;
    // See if we actually have something to search for
    if(!str || (size = strlen(str)) <= 0)
    {
        return _SC("");
    }
    // Obtain a temporary buffer
    Buffer b(size);
    // Resulted string size
    Uint32 n = 0;
    // Currently processed character
    SQChar c = 0;
    // Process characters
    while ((c = *(str++)) != 0)
    {
        // Convert it and move to the next one
        b.At< SQChar >(n++) = toupper(c);
    }
    // End the resulted string
    b.At< SQChar >(n) = 0;
    // Return the string
    return b.Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
static CSStr FromArray(Array & arr)
{
    // Determine array size
    const Int32 length = (Int32)arr.Length();
    // Obtain a temporary buffer
    Buffer b(length * sizeof(Int32));
    // Get array elements as integers
    arr.GetArray< Int32 >(b.Get< Int32 >(), length);
    // Overwrite integers with characters
    for (Int32 n = 0; n < length; ++n)
    {
        b.At< SQChar >(n) = (SQChar)b.At< Int32 >(n);
    }
    // Terminate the resulted string
    b.At< SQChar >(length) = 0;
    // Return the string
    return b.Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
static SQInteger StdPrintF(HSQUIRRELVM vm)
{
    CStr msg = NULL;
    SQInteger length = 0;
    if(SQ_FAILED(sqstd_format(vm, 2, &length, &msg)))
        return -1;
    LogUsr("%s", msg);
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
