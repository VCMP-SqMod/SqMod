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
    {
        return _SC(""); // Default to an empty string!
    }
    // Allocate a buffer with the requested width
    Buffer b(w + 1); // + null terminator
    // Is the specified string valid?
    if (!t || *t == '\0')
    {
        // Insert only the fill character
        std::memset(b.Data(), f, w);
    }
    else
    {
        // Calculate the string length
        const Uint32 n = strlen(t);
        // Insert only the fill character first
        std::memset(b.Data(), f, w);
        // Overwrite with the specified string
        std::strncpy(b.Data(), t, n);
    }
    // End the resulted string
    b.At(w) = '\0';
    // Return the resulted string
    return b.Get< SQChar >();
}

CSStr LeftStr(CSStr t, SQChar f, Uint32 w, Uint32 o)
{
    // Is the specified width valid?
    if (!w)
    {
        return _SC(""); // Default to an empty string!
    }
    // Is the specified offset within width range?
    else if (o > w)
    {
        STHROWF("Offset is out of bounds");
    }
    // Allocate a buffer with the requested width
    Buffer b(w + 1); // + null terminator
    // Is the specified string valid?
    if (!t || *t == '\0')
    {
        // Insert only the fill character
        std::memset(b.Data(), f, w);
    }
    else
    {
        // Clculate the string length
        const Uint32 n = strlen(t);
        // Insert the fill character first
        std::memset(b.Data(), f, w);
        // Overwrite with the specified string
        std::strncpy(b.Data() + o, t, w - n);
    }
    // End the resulted string
    b.At(w) = '\0';
    // Return the resulted string
    return b.Get();
}

// ------------------------------------------------------------------------------------------------
CSStr RightStr(CSStr t, SQChar f, Uint32 w)
{
    // Is the specified width valid?
    if (!w)
    {
        return _SC(""); // Default to an empty string!
    }
    // Allocate a buffer with the requested width
    Buffer b(w + 1); // + null terminator
    // Is the specified string valid?
    if (!t || *t == '\0')
    {
        // Insert only the fill character
        std::memset(b.Data(), f, w);
    }
    else
    {
        // Calculate the string length
        const Uint32 n = strlen(t);
        // Insert the fill character first
        std::memset(b.Data(), f, w);
        // Overwrite with the specified string
        if (n >= w)
        {
            std::strncpy(b.Data(), t, w);
        }
        else
        {
            std::strncpy(b.Data() + (w - n), t, n);
        }
    }
    // End the resulted string
    b.At(w) = '\0';
    // Return the resulted string
    return b.Get< SQChar >();
}

CSStr RightStr(CSStr t, SQChar f, Uint32 w, Uint32 o)
{
    // Is the specified width valid?
    if (!w)
    {
        return _SC(""); // Default to an empty string!
    }
    // Is the specified offset within width range?
    else if (o > w)
    {
        STHROWF("Offset is out of bounds");
    }
    // Allocate a buffer with the requested width
    Buffer b(w + 1); // + null terminator
    // Is the specified string valid?
    if (!t || *t == '\0')
    {
        // Insert only the fill character
        std::memset(b.Data(), f, w);
    }
    else
    {
        // Calculate the string length
        const Uint32 n = strlen(t);
        // Insert the fill character first
        std::memset(b.Data(), f, w);
        // Overwrite with the specified string
        if (n >= w || (n + o) >= w)
        {
            std::strncpy(b.Data(), t, w - o);
        }
        else
        {
            std::strncpy(b.Data() + ((w - n) - o), t, n);
        }
    }
    // End the resulted string
    b.At(w) = '\0';
    // Return the resulted string
    return b.Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
CSStr CenterStr(CSStr t, SQChar f, Uint32 w)
{
    // Is the specified width valid?
    if (!w)
    {
        return _SC(""); // Default to an empty string!
    }
    // Allocate a buffer with the requested width
    Buffer b(w + 1); // + null terminator
    // Is the specified string valid?
    if (!t || *t == '\0')
    {
        // Insert only the fill character
        std::memset(b.Data(), f, w);
    }
    else
    {
        // Calculate the string length
        const Uint32 n = strlen(t);
        // Insert only the fill character first
        std::memset(b.Data(), f, w);
        // Overwrite with the specified string
        std::strncpy(b.Data() + ((w/2) - (n/2)), t, n);
    }
    // End the resulted string
    b.At(w) = '\0';
    // Return the resulted string
    return b.Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
CSStr StrJustAlphaNum(CSStr str)
{
    // See if we actually have something to search for
    if(!str || *str == '\0')
    {
        return _SC(""); // Default to an empty string!
    }
    // Calculate the string length
    Uint32 size = std::strlen(str);
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
        if (std::isalnum(c) != 0)
        {
            // Save it and move to the next one
            b.At(n++) = c;
        }
    }
    // End the resulted string
    b.At(n) = '\0';
    // Return the string
    return b.Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
CSStr StrToLowercase(CSStr str)
{
    // See if we actually have something to search for
    if(!str || *str == '\0')
    {
        return _SC(""); // Default to an empty string!
    }
    // Calculate the string length
    Uint32 size = std::strlen(str);
    // Obtain a temporary buffer
    Buffer b(size + 1); // + null terminator
    // Resulted string size
    Uint32 n = 0;
    // Currently processed character
    SQChar c = 0;
    // Process characters
    while ((c = *(str++)) != '\0')
    {
        // Convert it and move to the next one
        b.At(n++) = std::tolower(c);
    }
    // End the resulted string
    b.At(n) = '\0';
    // Return the string
    return b.Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
CSStr StrToUppercase(CSStr str)
{
    // See if we actually have something to search for
    if(!str || *str == '\0')
    {
        return _SC(""); // Default to an empty string!
    }
    // Calculate the string length
    Uint32 size = std::strlen(str); // + null terminator
    // Obtain a temporary buffer
    Buffer b(size + 1); // + null terminator
    // Resulted string size
    Uint32 n = 0;
    // Currently processed character
    SQChar c = 0;
    // Process characters
    while ((c = *(str++)) != '\0')
    {
        // Convert it and move to the next one
        b.At(n++) = std::toupper(c);
    }
    // End the resulted string
    b.At(n) = '\0';
    // Return the string
    return b.Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
static bool OnlyDelimiter(CSStr str, SQChar chr)
{
    while (*str != '\0')
    {
        // Is this different from the delimiter?
        if (*(str++) != chr)
        {
            // Another character was found
            return false;
        }
    }
    // No other character was found
    return true;
}

// ------------------------------------------------------------------------------------------------
static Array StrExplode(CSStr str, SQChar chr, bool empty)
{
    // See if we actually have something to explode
    if(!str || *str == '\0')
    {
        // Default to an empty array
        return Array(DefaultVM::Get(), 0);
    }
    // Don't modify the specified string pointer
    CSStr itr = str, last = str;
    // The number of delimiter occurrences
    Uint32 num = 0;
    // Pre-count how many delimiters of this type exist
    while (*itr != '\0')
    {
        // Is this our delimiter?
        if (*(itr++) == chr)
        {
            // Are we allowed to include empty elements?
            if (empty || (itr - last) > 1)
            {
                // Increase the count
                ++num;
            }
            // Update the last delimiter position
            last = itr;
        }
    }
    // Were there no delimiters found and can we include empty elements?
    if (num == 0 && !empty && (str[1] == '\0' || OnlyDelimiter(str, chr)))
    {
        // Default to an empty array
        return Array(DefaultVM::Get(), 0);
    }
    // Have we found any delimiters?
    else if (num == 0)
    {
        // Create an array with just one element
        Array arr(DefaultVM::Get(), 1);
        // Test against strings with only delimiters
        if (str[1] == '\0' || OnlyDelimiter(str, chr))
        {
            arr.SetValue(0, _SC("")); // Add an empty string
        }
        else
        {
            arr.SetValue(0, str); // Add the whole string
        }
        // Return the resulted array
        return arr;
    }
    // Is there anything after the last delimiter?
    if (itr != last && *last != chr)
    {
        ++num; // Add it to the counter
    }
    // Pre-allocate an array with the number of found delimiters
    Array arr(DefaultVM::Get(), num);
    // Remember the initial stack size
    StackGuard sg;
    // Push the array object on the stack
    sq_pushobject(DefaultVM::Get(), arr.GetObject());
    // Don't modify the specified string pointer
    itr = str, last = str;
    // Reset the counter and use it as the element index
    num = 0;
    // Process the string again, this time slicing the actual elements
    while (*itr != '\0')
    {
        // Is this our delimiter?
        if (*itr++ == chr)
        {
            // Are we allowed to include empty elements?
            if (empty || (itr - last) > 1)
            {
                // Push the element index on the stack and advance to the next one
                sq_pushinteger(DefaultVM::Get(), num++);
                // Push the string portion on the stack
                sq_pushstring(DefaultVM::Get(), last, itr - last - 1);
                // Assign the string onto the
                sq_set(DefaultVM::Get(), -3);
            }
            // Update the last delimiter position
            last = itr;
        }
    }
    // Is there anything after the last delimiter?
    if (itr != last && *last != chr)
    {
        // Push the element index on the stack
        sq_pushinteger(DefaultVM::Get(), num);
        // Add the remaining string as an element
        sq_pushstring(DefaultVM::Get(), last, itr - last);
        // Assign the string onto the
        sq_set(DefaultVM::Get(), -3);
    }
    // Return the resulted array and let the stack guard handle the cleanup
    return arr;
}

// ------------------------------------------------------------------------------------------------
static CSStr StrImplode(Array & arr, SQChar chr)
{
    // Determine array size
    const Int32 length = static_cast< Int32 >(arr.Length());
    // Is there anything to implode?
    if (length <= 0)
    {
        return _SC(""); // Default to an empty string
    }
    // Obtain a temporary buffer
    Buffer b(length * 32);
    // Process array elements
    for (SQInteger i = 0; i < length; ++i)
    {
        // Retrieve the element value as string
        SharedPtr< String > str = arr.GetValue< String >(i);
        // Was there any value retrieved?
        if (!!str)
        {
            // Append the value to the buffer
            b.AppendS(str->data(), str->size());
        }
        // Append the delimiter
        b.Push(chr);
    }
    // Move the cursor back one element
    b.Retreat(1);
    // Set that as the null character
    b.Cursor() = '\0';
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
    {
        b.At(n) = static_cast< SQChar >(b.At< Int32 >(n));
    }
    // Terminate the resulted string
    b.At(length) = '\0';
    // Return the string
    return b.Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
static SQInteger StdPrintF(HSQUIRRELVM vm)
{
    CStr msg = nullptr;
    SQInteger length = 0;
    // Attempt to run the specified format and save the result
    const SQRESULT res = sqstd_format(vm, 2, &length, &msg);
    // Validate the result for errors and propagate them to the VM
    if(SQ_FAILED(res))
    {
        return res; // Return the error!
    }
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
    .Func(_SC("Uppercase"), &StrToUppercase)
    .Func(_SC("Explode"), &StrExplode)
    .Func(_SC("Implode"), &StrImplode);

    RootTable(vm).Bind(_SC("SqStr"), strns);
    RootTable(vm).SquirrelFunc(_SC("printf"), &StdPrintF);
}

} // Namespace:: SqMod
