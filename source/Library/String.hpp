#ifndef _LIBRARY_STRING_HPP_
#define _LIBRARY_STRING_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Utility used to generate a string with an arbitrary text surrounded by a specific character
*/
CSStr LeftStr(CSStr t, SQChar f, Uint32 w);
CSStr LeftStr(CSStr t, SQChar f, Uint32 w, Uint32 o);
CSStr RightStr(CSStr t, SQChar f, Uint32 w);
CSStr RightStr(CSStr t, SQChar f, Uint32 w, Uint32 o);
CSStr CenterStr(CSStr t, SQChar f, Uint32 w);

/* ------------------------------------------------------------------------------------------------
 * Get a new string with only the alpha numeric characters from the specified string.
*/
CSStr StrJustAlphaNum(CSStr str);

/* ------------------------------------------------------------------------------------------------
 * Convert the specified string to lowercase.
*/
CSStr StrToLowercase(CSStr str);

/* ------------------------------------------------------------------------------------------------
 * Convert the specified string to uppercase.
*/
CSStr StrToUppercase(CSStr str);

} // Namespace:: SqMod

#endif // _LIBRARY_STRING_HPP_
