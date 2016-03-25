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
        return sq_throwerror(vm, e.Message().c_str());
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
        return sq_throwerror(vm, e.Message().c_str());
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
        return sq_throwerror(vm, e.Message().c_str());
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
        return sq_throwerror(vm, e.Message().c_str());
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
        // Insert only the fill character first
        std::memset(b.Data(), f, w);
        // Overwrite with the specified string
        std::strncpy(b.Data() + ((w/2) - (n/2)), s, n);
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
        return sq_throwerror(vm, e.Message().c_str());
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
 * Known character classifications
*/
enum struct CharClass
{
    IsSpace = 0,
    IsPrint,
    IsCntrl,
    IsUpper,
    IsLower,
    IsAlpha,
    IsDigit,
    IsPunct,
    IsXdigit,
    IsAlnum,
    IsGraph,
    IsBlank
};

/* ------------------------------------------------------------------------------------------------
 * Specialization for the character classifications.
*/
template < CharClass > struct CharClassSpec;

// ------------------------------------------------------------------------------------------------
template <> struct CharClassSpec< CharClass::IsSpace >
{
    static inline int Fn(int c) { return std::isspace(c); }
};

// ------------------------------------------------------------------------------------------------
template <> struct CharClassSpec< CharClass::IsPrint >
{
    static inline int Fn(int c) { return std::isprint(c); }
};

// ------------------------------------------------------------------------------------------------
template <> struct CharClassSpec< CharClass::IsCntrl >
{
    static inline int Fn(int c) { return std::iscntrl(c); }
};

// ------------------------------------------------------------------------------------------------
template <> struct CharClassSpec< CharClass::IsUpper >
{
    static inline int Fn(int c) { return std::isupper(c); }
};

// ------------------------------------------------------------------------------------------------
template <> struct CharClassSpec< CharClass::IsLower >
{
    static inline int Fn(int c) { return std::islower(c); }
};

// ------------------------------------------------------------------------------------------------
template <> struct CharClassSpec< CharClass::IsAlpha >
{
    static inline int Fn(int c) { return std::isalpha(c); }
};

// ------------------------------------------------------------------------------------------------
template <> struct CharClassSpec< CharClass::IsDigit >
{
    static inline int Fn(int c) { return std::isdigit(c); }
};

// ------------------------------------------------------------------------------------------------
template <> struct CharClassSpec< CharClass::IsPunct >
{
    static inline int Fn(int c) { return std::ispunct(c); }
};

// ------------------------------------------------------------------------------------------------
template <> struct CharClassSpec< CharClass::IsXdigit >
{
    static inline int Fn(int c) { return std::isxdigit(c); }
};

// ------------------------------------------------------------------------------------------------
template <> struct CharClassSpec< CharClass::IsAlnum >
{
    static inline int Fn(int c) { return std::isalnum(c); }
};

// ------------------------------------------------------------------------------------------------
template <> struct CharClassSpec< CharClass::IsGraph >
{
    static inline int Fn(int c) { return std::isgraph(c); }
};

// ------------------------------------------------------------------------------------------------
template <> struct CharClassSpec< CharClass::IsBlank >
{
    static inline int Fn(int c) { return std::isblank(c); }
};

/* ------------------------------------------------------------------------------------------------
 * Templated functions for working with character classification functions.
*/
template < class T > struct StrCType
{
    /* --------------------------------------------------------------------------------------------
     * Checks if all the characters in the specified string are of the specified class.
    */
    static bool AllCharsImpl(CSStr str)
    {
        // See if we actually have something to search for
        if(!str || *str == '\0')
        {
            return false; // Subject to change!
        }
        // Start iterating characters and find the first that doesn't match
        while (*str != '\0')
        {
            // Call the predicate with the current character
            if (T::Fn(*(str++)) == 0)
            {
                return false; // This character did not pass the test
            }
        }
        // All characters passed the test
        return true;
    }

    /* --------------------------------------------------------------------------------------------
     * Extracts and forwards the string argument to the actual implementation.
    */
    static SQInteger AllChars(HSQUIRRELVM vm)
    {
        // Attempt to retrieve the value from the stack as a string
        StackStrF val(vm, 2);
        // Have we failed to retrieve the string?
        if (SQ_FAILED(val.mRes))
        {
            return val.mRes; // Propagate the error!
        }
        // Perform the actual check and push the result on the stack
        sq_pushbool(vm, AllCharsImpl(val.mPtr));
        // We have a value to return on the stack
        return 1;
    }

    /* --------------------------------------------------------------------------------------------
     * Find the position of the first character that matches the specified class.
    */
    static SQInteger FirstCharImpl(CSStr str)
    {
        // See if we actually have something to search for
        if(!str || *str == '\0')
        {
            return -1; // Subject to change!
        }
        // Start iterating characters and find the first that matches
        for (Uint32 n = 0; *str != '\0'; ++n, ++str)
        {
            // Call the predicate with the current character
            if (T::Fn(*str) != 0)
            {
                // This character passed our test
                return static_cast< SQInteger >(n);
            }
        }
        // Unable to locate such character
        return -1;
    }

    /* --------------------------------------------------------------------------------------------
     * Extracts and forwards the string argument to the actual implementation.
    */
    static SQInteger FirstChar(HSQUIRRELVM vm)
    {
        // Attempt to retrieve the value from the stack as a string
        StackStrF val(vm, 2);
        // Have we failed to retrieve the string?
        if (SQ_FAILED(val.mRes))
        {
            return val.mRes; // Propagate the error!
        }
        // Perform the actual check and store the resulted value
        SQInteger ret = FirstCharImpl(val.mPtr);
        // Have we found anything?
        if (ret >= 0)
        {
            sq_pushinteger(vm, ret);
        }
        else
        {
            sq_pushnull(vm);
        }
        // We have a value to return on the stack
        return 1;
    }

    /* --------------------------------------------------------------------------------------------
     * Find the position of the first character that doesn't match the specified class.
    */
    static SQInteger FirstNotCharImpl(CSStr str)
    {
        // See if we actually have something to search for
        if(!str || *str == '\0')
        {
            return -1; // Subject to change!
        }
        // Start iterating characters and find the first that doesn't match
        for (Uint32 n = 0; *str != '\0'; ++n, ++str)
        {
            // Call the predicate with the current character
            if (T::Fn(*str) == 0)
            {
                // This character passed our test
                return static_cast< SQInteger >(n);
            }
        }
        // Unable to locate such character
        return -1;
    }

    /* --------------------------------------------------------------------------------------------
     * Extracts and forwards the string argument to the actual implementation.
    */
    static SQInteger FirstNotChar(HSQUIRRELVM vm)
    {
        // Attempt to retrieve the value from the stack as a string
        StackStrF val(vm, 2);
        // Have we failed to retrieve the string?
        if (SQ_FAILED(val.mRes))
        {
            return val.mRes; // Propagate the error!
        }
        // Perform the actual check and store the resulted value
        SQInteger ret = FirstNotCharImpl(val.mPtr);
        // Have we found anything?
        if (ret >= 0)
        {
            sq_pushinteger(vm, ret);
        }
        else
        {
            sq_pushnull(vm);
        }
        // We have a value to return on the stack
        return 1;
    }

    /* --------------------------------------------------------------------------------------------
     * Find the position of the last character that matches the specified class.
    */
    static SQInteger LastCharImpl(CSStr str)
    {
        // See if we actually have something to search for
        if(!str || *str == '\0')
        {
            return -1; // Subject to change!
        }
        // Find the end of the string
        CSStr end = std::strchr(str, '\0') - 1;
        // Start iterating characters and find the first that matches
        for (; end >= str; --end)
        {
            // Call the predicate with the current character
            if (T::Fn(*end) != 0)
            {
                // This character passed our test
                return static_cast< SQInteger >(end - str);
            }
        }
        // Unable to locate such character
        return -1;
    }

    /* --------------------------------------------------------------------------------------------
     * Extracts and forwards the string argument to the actual implementation.
    */
    static SQInteger LastChar(HSQUIRRELVM vm)
    {
        // Attempt to retrieve the value from the stack as a string
        StackStrF val(vm, 2);
        // Have we failed to retrieve the string?
        if (SQ_FAILED(val.mRes))
        {
            return val.mRes; // Propagate the error!
        }
        // Perform the actual check and store the resulted value
        SQInteger ret = LastCharImpl(val.mPtr);
        // Have we found anything?
        if (ret >= 0)
        {
            sq_pushinteger(vm, ret);
        }
        else
        {
            sq_pushnull(vm);
        }
        // We have a value to return on the stack
        return 1;
    }

    /* --------------------------------------------------------------------------------------------
     * Find the position of the last character that doesn't match the specified class.
    */
    static SQInteger LastNotCharImpl(CSStr str)
    {
        // See if we actually have something to search for
        if(!str || *str == '\0')
        {
            return -1; // Subject to change!
        }
        // Find the end of the string
        CSStr end = std::strchr(str, '\0') - 1;
        // Start iterating characters and find the first that matches
        for (; end >= str; --end)
        {
            // Call the predicate with the current character
            if (T::Fn(*end) == 0)
            {
                // This character passed our test
                return static_cast< SQInteger >(end - str);
            }
        }
        // Unable to locate such character
        return -1;
    }

    /* --------------------------------------------------------------------------------------------
     * Extracts and forwards the string argument to the actual implementation.
    */
    static SQInteger LastNotChar(HSQUIRRELVM vm)
    {
        // Attempt to retrieve the value from the stack as a string
        StackStrF val(vm, 2);
        // Have we failed to retrieve the string?
        if (SQ_FAILED(val.mRes))
        {
            return val.mRes; // Propagate the error!
        }
        // Perform the actual check and store the resulted value
        SQInteger ret = LastNotCharImpl(val.mPtr);
        // Have we found anything?
        if (ret >= 0)
        {
            sq_pushinteger(vm, ret);
        }
        else
        {
            sq_pushnull(vm);
        }
        // We have a value to return on the stack
        return 1;
    }

};

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
    .Func(_SC("Explode"), &StrExplode)
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
    .SquirrelFunc(_SC("AreAllSpace"), &StrCType< CharClassSpec< CharClass::IsSpace > >::AllChars)
    .SquirrelFunc(_SC("AreAllPrint"), &StrCType< CharClassSpec< CharClass::IsPrint > >::AllChars)
    .SquirrelFunc(_SC("AreAllCntrl"), &StrCType< CharClassSpec< CharClass::IsCntrl > >::AllChars)
    .SquirrelFunc(_SC("AreAllUpper"), &StrCType< CharClassSpec< CharClass::IsUpper > >::AllChars)
    .SquirrelFunc(_SC("AreAllLower"), &StrCType< CharClassSpec< CharClass::IsLower > >::AllChars)
    .SquirrelFunc(_SC("AreAllAlpha"), &StrCType< CharClassSpec< CharClass::IsAlpha > >::AllChars)
    .SquirrelFunc(_SC("AreAllDigit"), &StrCType< CharClassSpec< CharClass::IsDigit > >::AllChars)
    .SquirrelFunc(_SC("AreAllPunct"), &StrCType< CharClassSpec< CharClass::IsPunct > >::AllChars)
    .SquirrelFunc(_SC("AreAllXdigit"), &StrCType< CharClassSpec< CharClass::IsXdigit > >::AllChars)
    .SquirrelFunc(_SC("AreAllAlnum"), &StrCType< CharClassSpec< CharClass::IsAlnum > >::AllChars)
    .SquirrelFunc(_SC("AreAllGraph"), &StrCType< CharClassSpec< CharClass::IsGraph > >::AllChars)
    .SquirrelFunc(_SC("AreAllBlank"), &StrCType< CharClassSpec< CharClass::IsBlank > >::AllChars)
    .SquirrelFunc(_SC("FirstSpace"), &StrCType< CharClassSpec< CharClass::IsSpace > >::FirstChar)
    .SquirrelFunc(_SC("FirstPrint"), &StrCType< CharClassSpec< CharClass::IsPrint > >::FirstChar)
    .SquirrelFunc(_SC("FirstCntrl"), &StrCType< CharClassSpec< CharClass::IsCntrl > >::FirstChar)
    .SquirrelFunc(_SC("FirstUpper"), &StrCType< CharClassSpec< CharClass::IsUpper > >::FirstChar)
    .SquirrelFunc(_SC("FirstLower"), &StrCType< CharClassSpec< CharClass::IsLower > >::FirstChar)
    .SquirrelFunc(_SC("FirstAlpha"), &StrCType< CharClassSpec< CharClass::IsAlpha > >::FirstChar)
    .SquirrelFunc(_SC("FirstDigit"), &StrCType< CharClassSpec< CharClass::IsDigit > >::FirstChar)
    .SquirrelFunc(_SC("FirstPunct"), &StrCType< CharClassSpec< CharClass::IsPunct > >::FirstChar)
    .SquirrelFunc(_SC("FirstXdigit"), &StrCType< CharClassSpec< CharClass::IsXdigit > >::FirstChar)
    .SquirrelFunc(_SC("FirstAlnum"), &StrCType< CharClassSpec< CharClass::IsAlnum > >::FirstChar)
    .SquirrelFunc(_SC("FirstGraph"), &StrCType< CharClassSpec< CharClass::IsGraph > >::FirstChar)
    .SquirrelFunc(_SC("FirstBlank"), &StrCType< CharClassSpec< CharClass::IsBlank > >::FirstChar)
    .SquirrelFunc(_SC("FirstNotSpace"), &StrCType< CharClassSpec< CharClass::IsSpace > >::FirstNotChar)
    .SquirrelFunc(_SC("FirstNotPrint"), &StrCType< CharClassSpec< CharClass::IsPrint > >::FirstNotChar)
    .SquirrelFunc(_SC("FirstNotCntrl"), &StrCType< CharClassSpec< CharClass::IsCntrl > >::FirstNotChar)
    .SquirrelFunc(_SC("FirstNotUpper"), &StrCType< CharClassSpec< CharClass::IsUpper > >::FirstNotChar)
    .SquirrelFunc(_SC("FirstNotLower"), &StrCType< CharClassSpec< CharClass::IsLower > >::FirstNotChar)
    .SquirrelFunc(_SC("FirstNotAlpha"), &StrCType< CharClassSpec< CharClass::IsAlpha > >::FirstNotChar)
    .SquirrelFunc(_SC("FirstNotDigit"), &StrCType< CharClassSpec< CharClass::IsDigit > >::FirstNotChar)
    .SquirrelFunc(_SC("FirstNotPunct"), &StrCType< CharClassSpec< CharClass::IsPunct > >::FirstNotChar)
    .SquirrelFunc(_SC("FirstNotXdigit"), &StrCType< CharClassSpec< CharClass::IsXdigit > >::FirstNotChar)
    .SquirrelFunc(_SC("FirstNotAlnum"), &StrCType< CharClassSpec< CharClass::IsAlnum > >::FirstNotChar)
    .SquirrelFunc(_SC("FirstNotGraph"), &StrCType< CharClassSpec< CharClass::IsGraph > >::FirstNotChar)
    .SquirrelFunc(_SC("FirstNotBlank"), &StrCType< CharClassSpec< CharClass::IsBlank > >::FirstNotChar)
    .SquirrelFunc(_SC("LastSpace"), &StrCType< CharClassSpec< CharClass::IsSpace > >::LastChar)
    .SquirrelFunc(_SC("LastPrint"), &StrCType< CharClassSpec< CharClass::IsPrint > >::LastChar)
    .SquirrelFunc(_SC("LastCntrl"), &StrCType< CharClassSpec< CharClass::IsCntrl > >::LastChar)
    .SquirrelFunc(_SC("LastUpper"), &StrCType< CharClassSpec< CharClass::IsUpper > >::LastChar)
    .SquirrelFunc(_SC("LastLower"), &StrCType< CharClassSpec< CharClass::IsLower > >::LastChar)
    .SquirrelFunc(_SC("LastAlpha"), &StrCType< CharClassSpec< CharClass::IsAlpha > >::LastChar)
    .SquirrelFunc(_SC("LastDigit"), &StrCType< CharClassSpec< CharClass::IsDigit > >::LastChar)
    .SquirrelFunc(_SC("LastPunct"), &StrCType< CharClassSpec< CharClass::IsPunct > >::LastChar)
    .SquirrelFunc(_SC("LastXdigit"), &StrCType< CharClassSpec< CharClass::IsXdigit > >::LastChar)
    .SquirrelFunc(_SC("LastAlnum"), &StrCType< CharClassSpec< CharClass::IsAlnum > >::LastChar)
    .SquirrelFunc(_SC("LastGraph"), &StrCType< CharClassSpec< CharClass::IsGraph > >::LastChar)
    .SquirrelFunc(_SC("LastBlank"), &StrCType< CharClassSpec< CharClass::IsBlank > >::LastChar)
    .SquirrelFunc(_SC("LastNotSpace"), &StrCType< CharClassSpec< CharClass::IsSpace > >::LastNotChar)
    .SquirrelFunc(_SC("LastNotPrint"), &StrCType< CharClassSpec< CharClass::IsPrint > >::LastNotChar)
    .SquirrelFunc(_SC("LastNotCntrl"), &StrCType< CharClassSpec< CharClass::IsCntrl > >::LastNotChar)
    .SquirrelFunc(_SC("LastNotUpper"), &StrCType< CharClassSpec< CharClass::IsUpper > >::LastNotChar)
    .SquirrelFunc(_SC("LastNotLower"), &StrCType< CharClassSpec< CharClass::IsLower > >::LastNotChar)
    .SquirrelFunc(_SC("LastNotAlpha"), &StrCType< CharClassSpec< CharClass::IsAlpha > >::LastNotChar)
    .SquirrelFunc(_SC("LastNotDigit"), &StrCType< CharClassSpec< CharClass::IsDigit > >::LastNotChar)
    .SquirrelFunc(_SC("LastNotPunct"), &StrCType< CharClassSpec< CharClass::IsPunct > >::LastNotChar)
    .SquirrelFunc(_SC("LastNotXdigit"), &StrCType< CharClassSpec< CharClass::IsXdigit > >::LastNotChar)
    .SquirrelFunc(_SC("LastNotAlnum"), &StrCType< CharClassSpec< CharClass::IsAlnum > >::LastNotChar)
    .SquirrelFunc(_SC("LastNotGraph"), &StrCType< CharClassSpec< CharClass::IsGraph > >::LastNotChar)
    .SquirrelFunc(_SC("LastNotBlank"), &StrCType< CharClassSpec< CharClass::IsBlank > >::LastNotChar);

    RootTable(vm).Bind(_SC("SqStr"), strns);
    RootTable(vm).SquirrelFunc(_SC("printf"), &StdPrintF);
}

} // Namespace:: SqMod
