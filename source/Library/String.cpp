// ------------------------------------------------------------------------------------------------
#include "Library/String.hpp"
#include "Base/Shared.hpp"
#include "Base/Buffer.hpp"

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
CSStr LeftStr(CSStr s, SQChar f, Uint32 w)
{
    // Is the specified width valid?
    if (!w)
    {
        return _SC(""); // Default to an empty string!
    }
    // Allocate a buffer with the requested width
    Buffer b(w + 1); // + null terminator
    // Is the specified string valid?
    if (!s || *s == '\0')
    {
        // Insert only the fill character
        std::memset(b.Data(), f, w);
    }
    else
    {
        // Calculate the string length
        const Uint32 n = std::strlen(s);
        // Insert only the fill character first
        std::memset(b.Data(), f, w);
        // Overwrite with the specified string
        std::strncpy(b.Data(), s, n);
    }
    // End the resulted string
    b.At(w) = '\0';
    // Return the resulted string
    return b.Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqLeftStr(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the fill character specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing fill character");
    }
    // Was the string width specified?
    else if (top <= 2)
    {
        return sq_throwerror(vm, "Missing string boundaries");
    }
    // Was the string value specified?
    else if (top <= 3)
    {
        return sq_throwerror(vm, "Missing string value");
    }
    // Attempt to generate the string value
    StackStrF val(vm, 4);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // The fill character and string boundaries
    SQChar fchar = 0;
    Uint32 width = 0;
    // Attempt to retrieve the remaining arguments from the stack
    try
    {
        fchar = Var< SQChar >(vm, 2).value;
        width = Var< Uint32 >(vm, 3).value;
    }
    catch (const Sqrat::Exception & e)
    {
        return sq_throwerror(vm, e.what());
    }
    catch (...)
    {
        return sq_throwerror(vm, "Unable to retrieve arguments");
    }
    // Forward the call to the actual implementation
    sq_pushstring(vm, LeftStr(val.mPtr, fchar, width), -1);
    // We have an argument on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
CSStr LeftStr(CSStr s, SQChar f, Uint32 w, Uint32 o)
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
    if (!s || *s == '\0')
    {
        // Insert only the fill character
        std::memset(b.Data(), f, w);
    }
    else
    {
        // Calculate the string length
        const Uint32 n = std::strlen(s);
        // Insert the fill character first
        std::memset(b.Data(), f, w);
        // Overwrite with the specified string
        if (n > (w - o))
        {
            std::strncpy(b.Data() + o, s, n);
        }
        else
        {
            std::memcpy(b.Data() + o, s, (n) * sizeof(SQChar));
        }
    }
    // End the resulted string
    b.At(w) = '\0';
    // Return the resulted string
    return b.Get();
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqLeftOffsetStr(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the fill character specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing fill character");
    }
    // Was the string width specified?
    else if (top <= 2)
    {
        return sq_throwerror(vm, "Missing string boundaries");
    }
    // Was the string offset specified?
    else if (top <= 3)
    {
        return sq_throwerror(vm, "Missing string offset");
    }
    // Was the string value specified?
    else if (top <= 4)
    {
        return sq_throwerror(vm, "Missing string value");
    }
    // Attempt to generate the string value
    StackStrF val(vm, 5);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // The fill character and string boundaries
    SQChar fchar = 0;
    Uint32 width = 0;
    Uint32 offset = 0;
    // Attempt to retrieve the remaining arguments from the stack
    try
    {
        fchar = Var< SQChar >(vm, 2).value;
        width = Var< Uint32 >(vm, 3).value;
        offset = Var< Uint32 >(vm, 4).value;
    }
    catch (const Sqrat::Exception & e)
    {
        return sq_throwerror(vm, e.what());
    }
    catch (...)
    {
        return sq_throwerror(vm, "Unable to retrieve arguments");
    }
    // Is the specified offset within width range?
    if (offset > width)
    {
        return sq_throwerror(vm, "Offset is out of bounds");
    }
    // Forward the call to the actual implementation
    sq_pushstring(vm, LeftStr(val.mPtr, fchar, width, offset), -1);
    // We have an argument on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
CSStr RightStr(CSStr s, SQChar f, Uint32 w)
{
    // Is the specified width valid?
    if (!w)
    {
        return _SC(""); // Default to an empty string!
    }
    // Allocate a buffer with the requested width
    Buffer b(w + 1); // + null terminator
    // Is the specified string valid?
    if (!s || *s == '\0')
    {
        // Insert only the fill character
        std::memset(b.Data(), f, w);
    }
    else
    {
        // Calculate the string length
        const Uint32 n = std::strlen(s);
        // Insert the fill character first
        std::memset(b.Data(), f, w);
        // Overwrite with the specified string
        if (n >= w)
        {
            std::strncpy(b.Data(), s, w);
        }
        else
        {
            std::strncpy(b.Data() + (w - n), s, n);
        }
    }
    // End the resulted string
    b.At(w) = '\0';
    // Return the resulted string
    return b.Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqRightStr(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the fill character specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing fill character");
    }
    // Was the string width specified?
    else if (top <= 2)
    {
        return sq_throwerror(vm, "Missing string boundaries");
    }
    // Was the string value specified?
    else if (top <= 3)
    {
        return sq_throwerror(vm, "Missing string value");
    }
    // Attempt to generate the string value
    StackStrF val(vm, 4);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // The fill character and string boundaries
    SQChar fchar = 0;
    Uint32 width = 0;
    // Attempt to retrieve the remaining arguments from the stack
    try
    {
        fchar = Var< SQChar >(vm, 2).value;
        width = Var< Uint32 >(vm, 3).value;
    }
    catch (const Sqrat::Exception & e)
    {
        return sq_throwerror(vm, e.what());
    }
    catch (...)
    {
        return sq_throwerror(vm, "Unable to retrieve arguments");
    }
    // Forward the call to the actual implementation
    sq_pushstring(vm, RightStr(val.mPtr, fchar, width), -1);
    // We have an argument on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
CSStr RightStr(CSStr s, SQChar f, Uint32 w, Uint32 o)
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
    if (!s || *s == '\0')
    {
        // Insert only the fill character
        std::memset(b.Data(), f, w);
    }
    else
    {
        // Calculate the string length
        const Uint32 n = std::strlen(s);
        // Insert the fill character first
        std::memset(b.Data(), f, w);
        // Overwrite with the specified string
        if (n >= w || (n + o) >= w)
        {
            std::strncpy(b.Data(), s, w - o);
        }
        else
        {
            std::strncpy(b.Data() + ((w - n) - o), s, n);
        }
    }
    // End the resulted string
    b.At(w) = '\0';
    // Return the resulted string
    return b.Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqRightOffsetStr(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the fill character specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing fill character");
    }
    // Was the string width specified?
    else if (top <= 2)
    {
        return sq_throwerror(vm, "Missing string boundaries");
    }
    // Was the string offset specified?
    else if (top <= 3)
    {
        return sq_throwerror(vm, "Missing string offset");
    }
    // Was the string value specified?
    else if (top <= 4)
    {
        return sq_throwerror(vm, "Missing string value");
    }
    // Attempt to generate the string value
    StackStrF val(vm, 5);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // The fill character and string boundaries
    SQChar fchar = 0;
    Uint32 width = 0;
    Uint32 offset = 0;
    // Attempt to retrieve the remaining arguments from the stack
    try
    {
        fchar = Var< SQChar >(vm, 2).value;
        width = Var< Uint32 >(vm, 3).value;
        offset = Var< Uint32 >(vm, 4).value;
    }
    catch (const Sqrat::Exception & e)
    {
        return sq_throwerror(vm, e.what());
    }
    catch (...)
    {
        return sq_throwerror(vm, "Unable to retrieve arguments");
    }
    // Is the specified offset within width range?
    if (offset > width)
    {
        return sq_throwerror(vm, "Offset is out of bounds");
    }
    // Forward the call to the actual implementation
    sq_pushstring(vm, RightStr(val.mPtr, fchar, width, offset), -1);
    // We have an argument on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
CSStr CenterStr(CSStr s, SQChar f, Uint32 w)
{
    // Is the specified width valid?
    if (!w)
    {
        return _SC(""); // Default to an empty string!
    }
    // Allocate a buffer with the requested width
    Buffer b(w + 1); // + null terminator
    // Is the specified string valid?
    if (!s || *s == '\0')
    {
        // Insert only the fill character
        std::memset(b.Data(), f, w);
    }
    else
    {
        // Calculate the string length
        const Uint32 n = std::strlen(s);
        // Calculate the insert position
        const Int32 p = ((w/2) - (n/2));
        // Insert only the fill character first
        std::memset(b.Data(), f, w);
        // Overwrite with the specified string
        std::strncpy(b.Data() + (p < 0 ? 0 : p), s, n);
    }
    // End the resulted string
    b.At(w) = '\0';
    // Return the resulted string
    return b.Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqCenterStr(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the fill character specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing fill character");
    }
    // Was the string width specified?
    else if (top <= 2)
    {
        return sq_throwerror(vm, "Missing string boundaries");
    }
    // Was the string value specified?
    else if (top <= 3)
    {
        return sq_throwerror(vm, "Missing string value");
    }
    // Attempt to generate the string value
    StackStrF val(vm, 4);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // The fill character and string boundaries
    SQChar fchar = 0;
    Uint32 width = 0;
    // Attempt to retrieve the remaining arguments from the stack
    try
    {
        fchar = Var< SQChar >(vm, 2).value;
        width = Var< Uint32 >(vm, 3).value;
    }
    catch (const Sqrat::Exception & e)
    {
        return sq_throwerror(vm, e.what());
    }
    catch (...)
    {
        return sq_throwerror(vm, "Unable to retrieve arguments");
    }
    // Forward the call to the actual implementation
    sq_pushstring(vm, CenterStr(val.mPtr, fchar, width), -1);
    // We have an argument on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
Buffer StrJustAlphaNumB(CSStr str)
{
    // See if we actually have something to search for
    if(!str || *str == '\0')
    {
        return Buffer(); // Default to an empty buffer!
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
    // Move the cursor to the end
    b.Move(n);
    // Return ownership of the buffer
    return std::move(b);
}

// ------------------------------------------------------------------------------------------------
CSStr StrJustAlphaNum(CSStr str)
{
    // See if we actually have something to search for
    if(!str || *str == '\0')
    {
        return _SC(""); // Default to an empty string!
    }
    // Attempt to convert and return the result
    return StrJustAlphaNumB(str).Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqJustAlphaNum(HSQUIRRELVM vm)
{
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // Forward the call to the actual implementation and store the buffer
    Buffer b(StrJustAlphaNumB(val.mPtr));
    // Is there anything in the buffer?
    if (!b)
    {
        // Default to an empty string
        sq_pushstring(vm, _SC(""), 0);
    }
    else
    {
        // Push the buffer data as a string
        sq_pushstring(vm, b.Data(), b.Position());
    }
    // We have a value to return on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
Buffer StrToLowercaseB(CSStr str)
{
    // See if we actually have something to search for
    if(!str || *str == '\0')
    {
        return Buffer(); // Default to an empty buffer!
    }
    // Calculate the string length
    Uint32 size = std::strlen(str);
    // Obtain a temporary buffer
    Buffer b(size + 1); // + null terminator
    // Resulted string size
    Uint32 n = 0;
    // Process characters
    while (*str != '\0')
    {
        // Convert it and move to the next one
        b.At(n++) = std::tolower(*(str++));
    }
    // End the resulted string
    b.At(n) = '\0';
    // Move the cursor to the end
    b.Move(n);
    // Return ownership of the buffer
    return std::move(b);
}

// ------------------------------------------------------------------------------------------------
CSStr StrToLowercase(CSStr str)
{
    // See if we actually have something to search for
    if(!str || *str == '\0')
    {
        return _SC(""); // Default to an empty string!
    }
    // Attempt to convert and return the result
    return StrToLowercaseB(str).Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqToLowercase(HSQUIRRELVM vm)
{
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // Forward the call to the actual implementation and store the buffer
    Buffer b(StrToLowercaseB(val.mPtr));
    // Is there anything in the buffer?
    if (!b)
    {
        // Default to an empty string
        sq_pushstring(vm, _SC(""), 0);
    }
    else
    {
        // Push the buffer data as a string
        sq_pushstring(vm, b.Data(), b.Position());
    }
    // We have a value to return on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
Buffer StrToUppercaseB(CSStr str)
{
    // See if we actually have something to search for
    if(!str || *str == '\0')
    {
        return Buffer(); // Default to an empty buffer!
    }
    // Calculate the string length
    Uint32 size = std::strlen(str); // + null terminator
    // Obtain a temporary buffer
    Buffer b(size + 1); // + null terminator
    // Resulted string size
    Uint32 n = 0;
    // Process characters
    while (*str != '\0')
    {
        // Convert it and move to the next one
        b.At(n++) = std::toupper(*(str++));
    }
    // End the resulted string
    b.At(n) = '\0';
    // Move the cursor to the end
    b.Move(n);
    // Return ownership of the buffer
    return std::move(b);
}

// ------------------------------------------------------------------------------------------------
CSStr StrToUppercase(CSStr str)
{
    // See if we actually have something to search for
    if(!str || *str == '\0')
    {
        return _SC(""); // Default to an empty string!
    }
    // Attempt to convert and return the result
    return StrToUppercaseB(str).Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
static SQInteger SqToUppercase(HSQUIRRELVM vm)
{
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // Forward the call to the actual implementation and store the buffer
    Buffer b(StrToUppercaseB(val.mPtr));
    // Is there anything in the buffer?
    if (!b)
    {
        // Default to an empty string
        sq_pushstring(vm, _SC(""), 0);
    }
    else
    {
        // Push the buffer data as a string
        sq_pushstring(vm, b.Data(), b.Position());
    }
    // We have a value to return on the stack
    return 1;
}

/* ------------------------------------------------------------------------------------------------
 * Checks if all the characters in the specified string are of the specified class or not.
*/
static SQInteger AllCharsImpl(HSQUIRRELVM vm, int(*fn)(int))
{
    static const SQInteger top = sq_gettop(vm);
    // Is there a value to process?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing string or value");
    }

    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // See if we actually have something to search for
    else if (!(val.mPtr) || *(val.mPtr) == '\0')
    {
        // Since there are no other different character then
        // we count this as all characters being of this type
        sq_pushbool(vm, true);
        // We're done here
        return 1;
    }

    // Start iterating characters and find the first that doesn't match
    for (CSStr itr = val.mPtr; *itr != '\0'; ++itr)
    {
        // Call the predicate with the current character
        if (fn(*itr) == 0)
        {
            // This character did not pass the test
            sq_pushbool(vm, false);
            // We're done here
            return 1;
        }
    }

    // All characters passed the test
    sq_pushbool(vm, true);
    // We're done here
    return 1;
}

/* ------------------------------------------------------------------------------------------------
 * Simple forwarder to minimize templated functions with large body and reduce executable size.
*/
template < int (*Fn)(int) > static SQInteger AllCharsProxy(HSQUIRRELVM vm)
{
    return AllCharsImpl(vm, Fn);
}

/* ------------------------------------------------------------------------------------------------
 * Find the position of the first character that matches the specified class.
*/
static SQInteger FirstCharImpl(HSQUIRRELVM vm, int(*fn)(int), bool neg)
{
    static const SQInteger top = sq_gettop(vm);
    // Is there a value to process?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing string or value");
    }

    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // See if we actually have something to search for
    else if (!(val.mPtr) || *(val.mPtr) == '\0')
    {
        // Since there are no other different character then
        // we count this as no characters being of this type
        sq_pushnull(vm);
        // We're done here
        return 1;
    }

    // Start iterating characters and find the first that matches
    for (CSStr itr = val.mPtr; *itr != '\0'; ++itr)
    {
        // Call the predicate with the current character
        if ((fn(*itr) == 0) == neg)
        {
            // This character passed our test, push it's position
            sq_pushinteger(vm, itr - val.mPtr);
            // We're done here
            return 1;
        }
    }

    // Unable to locate such character
    sq_pushnull(vm);
    // We're done here
    return 1;
}

/* ------------------------------------------------------------------------------------------------
 * Simple forwarder to minimize templated functions with large body and reduce executable size.
*/
template < int (*Fn)(int), bool Neg >static SQInteger FirstCharProxy(HSQUIRRELVM vm)
{
    return FirstCharImpl(vm, Fn, Neg);
}

/* ------------------------------------------------------------------------------------------------
 * Find the position of the last character that matches the specified class.
*/
static SQInteger LastCharImpl(HSQUIRRELVM vm, int(*fn)(int), bool neg)
{
    static const SQInteger top = sq_gettop(vm);
    // Is there a value to process?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing string or value");
    }

    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // See if we actually have something to search for
    else if (!(val.mPtr) || *(val.mPtr) == '\0')
    {
        // Since there are no other different character then
        // we count this as no characters being of this type
        sq_pushnull(vm);
        // We're done here
        return 1;
    }

    // Start iterating characters and find the first that matches
    for (CSStr itr = (val.mPtr + val.mLen) - 1; itr >= val.mPtr; --itr)
    {
        // Call the predicate with the current character
        if ((fn(*itr) == 0) == neg)
        {
            // This character passed our test, push it's position
            sq_pushinteger(vm, itr - val.mPtr);
            // We're done here
            return 1;
        }
    }

    // Unable to locate such character
    sq_pushnull(vm);
    // We're done here
    return 1;
}

/* ------------------------------------------------------------------------------------------------
 * Simple forwarder to minimize templated functions with large body and reduce executable size.
*/
template < int (*Fn)(int), bool Neg >static SQInteger LastCharProxy(HSQUIRRELVM vm)
{
    return LastCharImpl(vm, Fn, Neg);
}

/* ------------------------------------------------------------------------------------------------
 * Split the string into chunks wherever a character matches or not the specified class.
*/
static SQInteger SplitWhereCharImpl(HSQUIRRELVM vm, int(*fn)(int), bool neg)
{
    static const SQInteger top = sq_gettop(vm);
    // Is there a value to process?
    if (top <= 1)
    {
        return sq_throwerror(vm, "Missing string or value");
    }

    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }

    // Create a new array on the stack
    sq_newarray(vm, 0);

    // See if we actually have something to search for
    if (!(val.mPtr) || *(val.mPtr) == '\0')
    {
        return 1; // Return an empty array
    }

    // Remember the position of the last found match
    CSStr last = val.mPtr;

    // Start iterating characters and slice where a match is found
    for (CSStr itr = val.mPtr; *itr != '\0'; ++itr)
    {
        // Call the predicate with the current character
        if ((fn(*itr) == 0) == neg)
        {
            // Are there any characters before this match?
            if ((itr - last) > 1 && (last != '\0'))
            {
                // Push this chunk of string on the stack
                sq_pushstring(vm, last, itr - last);
                // Insert this element into the array on the stack
                const SQRESULT r = sq_arrayappend(vm, -2);
                // Did we fail to append the element?
                if (SQ_FAILED(r))
                {
                    return r; // We're done here
                }
            }
            // Push this character as an integer on the stack
            sq_pushinteger(vm, *itr);
            // Insert this element into the array on the stack
            const SQRESULT r = sq_arrayappend(vm, -2);
            // Did we fail to append the element?
            if (SQ_FAILED(r))
            {
                return r; // We're done here
            }
            // Update the position of the last found match
            last = (itr + 1);
        }
    }

    // Push the remaining chunk, if any
    if (*last != '\0')
    {
        // Push this chunk of string on the stack
        sq_pushstring(vm, last, -1);
        // Insert this element into the array on the stack
        const SQRESULT r = sq_arrayappend(vm, -2);
        // Did we fail to append the element?
        if (SQ_FAILED(r))
        {
            return r; // We're done here
        }
    }

    // We have a value to return on the stack
    return 1;
}

/* ------------------------------------------------------------------------------------------------
 * Simple forwarder to minimize templated functions with large body and reduce executable size.
*/
template < int (*Fn)(int), bool Neg > static SQInteger SplitWhereCharProxy(HSQUIRRELVM vm)
{
    return SplitWhereCharImpl(vm, Fn, Neg);
}

/* ------------------------------------------------------------------------------------------------
 * Checks if the specified character is of the specified class.
*/
template < int (*Fn)(int) > static bool IsCharOfType(int c)
{
    return (Fn(c) != 0);
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
static SQInteger SqStrExplode(HSQUIRRELVM vm)
{
    const Int32 top = sq_gettop(vm);
    // Was the delimiter character specified?
    if (top <= 1)
    {
        return sq_throwerror(vm, _SC("Missing delimiter character"));
    }
    // Was the boolean empty specified?
    else if (top <= 2)
    {
        return sq_throwerror(vm, _SC("Missing boolean empty"));
    }
    // Was the string value specified?
    else if (top <= 3)
    {
        return sq_throwerror(vm, _SC("Missing string value"));
    }
    // Attempt to generate the string value
    StackStrF val(vm, 4);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // The delimiter character and boolean empty
    SQChar delim = 0;
    bool empty = 0;
    // Attempt to retrieve the remaining arguments from the stack
    try
    {
        delim = Var< SQChar >(vm, 2).value;
        empty = Var< bool >(vm, 3).value;
    }
    catch (const Sqrat::Exception & e)
    {
        return sq_throwerror(vm, e.what());
    }
    catch (...)
    {
        return sq_throwerror(vm, _SC("Unable to retrieve arguments"));
    }
    // Grab the string value to a shorter name
    CSStr str = val.mPtr;
    // Create an empty array on the stack
    sq_newarray(vm, 0);
    // See if we actually have something to explode
    if(!str || *str == '\0')
    {
        // Specify that we have an argument on the stack
        return 1;
    }
    // Don't modify the specified string pointer
    CSStr itr = str, last = str;
    // The number of delimiter occurrences
    Uint32 num = 0;
    // Pre-count how many delimiters of this type exist
    while (*itr != '\0')
    {
        // Is this our delimiter?
        if (*(itr++) == delim)
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
    if (num == 0 && !empty && (str[1] == '\0' || OnlyDelimiter(str, delim)))
    {
        // Specify that we have an argument on the stack
        return 1;
    }
    // Have we found any delimiters?
    else if (num == 0)
    {
        // Test against strings with only delimiters
        if (str[1] == '\0' || OnlyDelimiter(str, delim))
        {
            sq_pushstring(vm, _SC(""), 0); // Add an empty string
        }
        else
        {
            sq_pushstring(vm, val.mPtr, val.mLen); // Add the whole string
        }
        // Append the string on the stack to the array
        const SQRESULT r = sq_arrayappend(vm, -2);
        // Check the result
        if (SQ_FAILED(r))
        {
            return r; // Propagate the error
        }
        // Specify that we have an argument on the stack
        return 1;
    }
    // Is there anything after the last delimiter?
    if (itr != last && *last != delim)
    {
        ++num; // Add it to the counter
    }
    SQRESULT r = SQ_OK;
    // Pre-allocate an array with the number of found delimiters
    r = sq_arrayresize(vm, -1, num);
    // Check the result
    if (SQ_FAILED(r))
    {
        return r; // Propagate the error
    }
    // Don't modify the specified string pointer
    itr = str, last = str;
    // Reset the counter and use it as the element index
    num = 0;
    // Process the string again, this time slicing the actual elements
    while (*itr != '\0')
    {
        // Is this our delimiter?
        if (*itr++ == delim)
        {
            // Are we allowed to include empty elements?
            if (empty || (itr - last) > 1)
            {
                // Push the element index on the stack and advance to the next one
                sq_pushinteger(vm, num++);
                // Push the string portion on the stack
                sq_pushstring(vm, last, itr - last - 1);
                // Assign the string onto the
                r = sq_set(vm, -3);
                // Check the result
                if (SQ_FAILED(r))
                {
                    return r; // Propagate the error
                }
            }
            // Update the last delimiter position
            last = itr;
        }
    }
    // Is there anything after the last delimiter?
    if (itr != last && *last != delim)
    {
        // Push the element index on the stack
        sq_pushinteger(vm, num);
        // Add the remaining string as an element
        sq_pushstring(vm, last, itr - last);
        // Assign the string onto the
        r = sq_set(vm, -3);
        // Check the result
        if (SQ_FAILED(r))
        {
            return r; // Propagate the error
        }
    }
    // Specify that we have an argument on the stack
    return 1;
}

// ------------------------------------------------------------------------------------------------
static CSStr StrImplode(SQChar chr, Array & arr)
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
    const Int32 length = ConvTo< Int32 >::From(arr.Length());
    // Obtain a temporary buffer
    Buffer b(length * sizeof(Int32));
    // Get array elements as integers
    arr.GetArray< Int32 >(b.Get< Int32 >(), length);
    // Overwrite integers with characters
    for (Int32 n = 0; n < length; ++n)
    {
        b.At(n) = ConvTo< SQChar >::From(b.At< Int32 >(n * sizeof(Int32)));
    }
    // Terminate the resulted string
    b.At(length) = '\0';
    // Return the string
    return b.Get< SQChar >();
}

// ------------------------------------------------------------------------------------------------
static SQInteger StdPrintF(HSQUIRRELVM vm)
{
    // Attempt to retrieve the value from the stack as a string
    StackStrF val(vm, 2);
    // Have we failed to retrieve the string?
    if (SQ_FAILED(val.mRes))
    {
        return val.mRes; // Propagate the error!
    }
    // Send the resulted string to console as a user message
    LogUsr("%s", val.mPtr);
    // This function doesn't return anything
    return 0;
}

// ================================================================================================
void Register_String(HSQUIRRELVM vm)
{
    Table strns(vm);

    strns.Func(_SC("FromArray"), &FromArray)
    .SquirrelFunc(_SC("Explode"), &SqStrExplode)
    .Func(_SC("Implode"), &StrImplode)
    .SquirrelFunc(_SC("Center"), &SqCenterStr)
    .SquirrelFunc(_SC("Left"), &SqLeftStr)
    .SquirrelFunc(_SC("Right"), &SqRightStr)
    .SquirrelFunc(_SC("LeftOffset"), &SqLeftOffsetStr)
    .SquirrelFunc(_SC("RightOffset"), &SqRightOffsetStr)
    .SquirrelFunc(_SC("ToLower"), &SqToLowercase)
    .SquirrelFunc(_SC("ToUpper"), &SqToUppercase)
    .SquirrelFunc(_SC("Lowercase"), &SqToLowercase)
    .SquirrelFunc(_SC("Uppercase"), &SqToUppercase)
    .SquirrelFunc(_SC("JustAlnum"), &SqJustAlphaNum)
    .SquirrelFunc(_SC("AreAllSpace"), &AllCharsProxy< std::isspace >)
    .SquirrelFunc(_SC("AreAllPrint"), &AllCharsProxy< std::isprint >)
    .SquirrelFunc(_SC("AreAllCntrl"), &AllCharsProxy< std::iscntrl >)
    .SquirrelFunc(_SC("AreAllUpper"), &AllCharsProxy< std::isupper >)
    .SquirrelFunc(_SC("AreAllLower"), &AllCharsProxy< std::islower >)
    .SquirrelFunc(_SC("AreAllAlpha"), &AllCharsProxy< std::isalpha >)
    .SquirrelFunc(_SC("AreAllDigit"), &AllCharsProxy< std::isdigit >)
    .SquirrelFunc(_SC("AreAllPunct"), &AllCharsProxy< std::ispunct >)
    .SquirrelFunc(_SC("AreAllXdigit"), &AllCharsProxy< std::isxdigit >)
    .SquirrelFunc(_SC("AreAllAlnum"), &AllCharsProxy< std::isalnum >)
    .SquirrelFunc(_SC("AreAllGraph"), &AllCharsProxy< std::isgraph >)
    .SquirrelFunc(_SC("AreAllBlank"), &AllCharsProxy< std::isblank >)
    .SquirrelFunc(_SC("FirstSpace"), &FirstCharProxy< std::isspace, false >)
    .SquirrelFunc(_SC("FirstPrint"), &FirstCharProxy< std::isprint, false >)
    .SquirrelFunc(_SC("FirstCntrl"), &FirstCharProxy< std::iscntrl, false >)
    .SquirrelFunc(_SC("FirstUpper"), &FirstCharProxy< std::isupper, false >)
    .SquirrelFunc(_SC("FirstLower"), &FirstCharProxy< std::islower, false >)
    .SquirrelFunc(_SC("FirstAlpha"), &FirstCharProxy< std::isalpha, false >)
    .SquirrelFunc(_SC("FirstDigit"), &FirstCharProxy< std::isdigit, false >)
    .SquirrelFunc(_SC("FirstPunct"), &FirstCharProxy< std::ispunct, false >)
    .SquirrelFunc(_SC("FirstXdigit"), &FirstCharProxy< std::isxdigit, false >)
    .SquirrelFunc(_SC("FirstAlnum"), &FirstCharProxy< std::isalnum, false >)
    .SquirrelFunc(_SC("FirstGraph"), &FirstCharProxy< std::isgraph, false >)
    .SquirrelFunc(_SC("FirstBlank"), &FirstCharProxy< std::isblank, false >)
    .SquirrelFunc(_SC("FirstNotSpace"), &FirstCharProxy< std::isspace, true >)
    .SquirrelFunc(_SC("FirstNotPrint"), &FirstCharProxy< std::isprint, true >)
    .SquirrelFunc(_SC("FirstNotCntrl"), &FirstCharProxy< std::iscntrl, true >)
    .SquirrelFunc(_SC("FirstNotUpper"), &FirstCharProxy< std::isupper, true >)
    .SquirrelFunc(_SC("FirstNotLower"), &FirstCharProxy< std::islower, true >)
    .SquirrelFunc(_SC("FirstNotAlpha"), &FirstCharProxy< std::isalpha, true >)
    .SquirrelFunc(_SC("FirstNotDigit"), &FirstCharProxy< std::isdigit, true >)
    .SquirrelFunc(_SC("FirstNotPunct"), &FirstCharProxy< std::ispunct, true >)
    .SquirrelFunc(_SC("FirstNotXdigit"), &FirstCharProxy< std::isxdigit, true >)
    .SquirrelFunc(_SC("FirstNotAlnum"), &FirstCharProxy< std::isalnum, true >)
    .SquirrelFunc(_SC("FirstNotGraph"), &FirstCharProxy< std::isgraph, true >)
    .SquirrelFunc(_SC("FirstNotBlank"), &FirstCharProxy< std::isblank, true >)
    .SquirrelFunc(_SC("LastSpace"), &LastCharProxy< std::isspace, false >)
    .SquirrelFunc(_SC("LastPrint"), &LastCharProxy< std::isprint, false >)
    .SquirrelFunc(_SC("LastCntrl"), &LastCharProxy< std::iscntrl, false >)
    .SquirrelFunc(_SC("LastUpper"), &LastCharProxy< std::isupper, false >)
    .SquirrelFunc(_SC("LastLower"), &LastCharProxy< std::islower, false >)
    .SquirrelFunc(_SC("LastAlpha"), &LastCharProxy< std::isalpha, false >)
    .SquirrelFunc(_SC("LastDigit"), &LastCharProxy< std::isdigit, false >)
    .SquirrelFunc(_SC("LastPunct"), &LastCharProxy< std::ispunct, false >)
    .SquirrelFunc(_SC("LastXdigit"), &LastCharProxy< std::isxdigit, false >)
    .SquirrelFunc(_SC("LastAlnum"), &LastCharProxy< std::isalnum, false >)
    .SquirrelFunc(_SC("LastGraph"), &LastCharProxy< std::isgraph, false >)
    .SquirrelFunc(_SC("LastBlank"), &LastCharProxy< std::isblank, false >)
    .SquirrelFunc(_SC("LastNotSpace"), &LastCharProxy< std::isspace, true >)
    .SquirrelFunc(_SC("LastNotPrint"), &LastCharProxy< std::isprint, true >)
    .SquirrelFunc(_SC("LastNotCntrl"), &LastCharProxy< std::iscntrl, true >)
    .SquirrelFunc(_SC("LastNotUpper"), &LastCharProxy< std::isupper, true >)
    .SquirrelFunc(_SC("LastNotLower"), &LastCharProxy< std::islower, true >)
    .SquirrelFunc(_SC("LastNotAlpha"), &LastCharProxy< std::isalpha, true >)
    .SquirrelFunc(_SC("LastNotDigit"), &LastCharProxy< std::isdigit, true >)
    .SquirrelFunc(_SC("LastNotPunct"), &LastCharProxy< std::ispunct, true >)
    .SquirrelFunc(_SC("LastNotXdigit"), &LastCharProxy< std::isxdigit, true >)
    .SquirrelFunc(_SC("LastNotAlnum"), &LastCharProxy< std::isalnum, true >)
    .SquirrelFunc(_SC("LastNotGraph"), &LastCharProxy< std::isgraph, true >)
    .SquirrelFunc(_SC("LastNotBlank"), &LastCharProxy< std::isblank, true >)
    .SquirrelFunc(_SC("SplitWhereSpace"), &SplitWhereCharProxy< std::isspace, false >)
    .SquirrelFunc(_SC("SplitWherePrint"), &SplitWhereCharProxy< std::isprint, false >)
    .SquirrelFunc(_SC("SplitWhereCntrl"), &SplitWhereCharProxy< std::iscntrl, false >)
    .SquirrelFunc(_SC("SplitWhereUpper"), &SplitWhereCharProxy< std::isupper, false >)
    .SquirrelFunc(_SC("SplitWhereLower"), &SplitWhereCharProxy< std::islower, false >)
    .SquirrelFunc(_SC("SplitWhereAlpha"), &SplitWhereCharProxy< std::isalpha, false >)
    .SquirrelFunc(_SC("SplitWhereDigit"), &SplitWhereCharProxy< std::isdigit, false >)
    .SquirrelFunc(_SC("SplitWherePunct"), &SplitWhereCharProxy< std::ispunct, false >)
    .SquirrelFunc(_SC("SplitWhereXdigit"), &SplitWhereCharProxy< std::isxdigit, false >)
    .SquirrelFunc(_SC("SplitWhereAlnum"), &SplitWhereCharProxy< std::isalnum, false >)
    .SquirrelFunc(_SC("SplitWhereGraph"), &SplitWhereCharProxy< std::isgraph, false >)
    .SquirrelFunc(_SC("SplitWhereBlank"), &SplitWhereCharProxy< std::isblank, false >)
    .SquirrelFunc(_SC("SplitWhereNotSpace"), &SplitWhereCharProxy< std::isspace, true >)
    .SquirrelFunc(_SC("SplitWhereNotPrint"), &SplitWhereCharProxy< std::isprint, true >)
    .SquirrelFunc(_SC("SplitWhereNotCntrl"), &SplitWhereCharProxy< std::iscntrl, true >)
    .SquirrelFunc(_SC("SplitWhereNotUpper"), &SplitWhereCharProxy< std::isupper, true >)
    .SquirrelFunc(_SC("SplitWhereNotLower"), &SplitWhereCharProxy< std::islower, true >)
    .SquirrelFunc(_SC("SplitWhereNotAlpha"), &SplitWhereCharProxy< std::isalpha, true >)
    .SquirrelFunc(_SC("SplitWhereNotDigit"), &SplitWhereCharProxy< std::isdigit, true >)
    .SquirrelFunc(_SC("SplitWhereNotPunct"), &SplitWhereCharProxy< std::ispunct, true >)
    .SquirrelFunc(_SC("SplitWhereNotXdigit"), &SplitWhereCharProxy< std::isxdigit, true >)
    .SquirrelFunc(_SC("SplitWhereNotAlnum"), &SplitWhereCharProxy< std::isalnum, true >)
    .SquirrelFunc(_SC("SplitWhereNotGraph"), &SplitWhereCharProxy< std::isgraph, true >)
    .SquirrelFunc(_SC("SplitWhereNotBlank"), &SplitWhereCharProxy< std::isblank, true >);

    RootTable(vm).Bind(_SC("SqStr"), strns);
    RootTable(vm).SquirrelFunc(_SC("printf"), &StdPrintF);

    RootTable(vm)
    .Func(_SC("IsSpace"), &IsCharOfType< std::isspace >)
    .Func(_SC("IsPrint"), &IsCharOfType< std::isprint >)
    .Func(_SC("IsCntrl"), &IsCharOfType< std::iscntrl >)
    .Func(_SC("IsUpper"), &IsCharOfType< std::isupper >)
    .Func(_SC("IsLower"), &IsCharOfType< std::islower >)
    .Func(_SC("IsAlpha"), &IsCharOfType< std::isalpha >)
    .Func(_SC("IsDigit"), &IsCharOfType< std::isdigit >)
    .Func(_SC("IsPunct"), &IsCharOfType< std::ispunct >)
    .Func(_SC("IsXdigit"), &IsCharOfType< std::isxdigit >)
    .Func(_SC("IsAlnum"), &IsCharOfType< std::isalnum >)
    .Func(_SC("IsGraph"), &IsCharOfType< std::isgraph >)
    .Func(_SC("IsBlank"), &IsCharOfType< std::isblank >)
    .Func(_SC("ToLower"), &tolower)
    .Func(_SC("ToUpper"), &toupper);
}

} // Namespace:: SqMod
