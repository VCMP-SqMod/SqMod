#ifndef _LIBRARY_STRING_HPP_
#define _LIBRARY_STRING_HPP_

// ------------------------------------------------------------------------------------------------
#include "SqBase.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Get a new string with only the alpha numeric characters from the specified string.
*/
CSStr StrJustAlphaNum(CSStr str);
Buffer StrJustAlphaNumB(CSStr str);

/* ------------------------------------------------------------------------------------------------
 * Convert the specified string to lowercase.
*/
CSStr StrToLowercase(CSStr str);
Buffer StrToLowercaseB(CSStr str);

/* ------------------------------------------------------------------------------------------------
 * Convert the specified string to uppercase.
*/
CSStr StrToUppercase(CSStr str);
Buffer StrToUppercaseB(CSStr str);

} // Namespace:: SqMod

#endif // _LIBRARY_STRING_HPP_
