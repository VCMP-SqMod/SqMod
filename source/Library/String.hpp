#ifndef _LIBRARY_STRING_HPP_
#define _LIBRARY_STRING_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Utility used to generate a string with an arbitrary text surrounded by a specific character
*/
CSStr LeftStr(CSStr t, SQChar f, Uint32 w = 72);
CSStr LeftStr(CSStr t, SQChar f, Uint32 w = 72, Uint32 o = 0);
CSStr RightStr(CSStr t, SQChar f, Uint32 w = 72);
CSStr RightStr(CSStr t, SQChar f, Uint32 w = 72, Uint32 o = 0);
CSStr CenterStr(CSStr t, SQChar f, Uint32 w = 72);

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
