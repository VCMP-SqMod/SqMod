#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Get a new string with only the alpha numeric characters from the specified string.
*/
SQMOD_NODISCARD const SQChar * StrJustAlphaNum(const SQChar * str);
SQMOD_NODISCARD Buffer StrJustAlphaNumB(const SQChar * str);

/* ------------------------------------------------------------------------------------------------
 * Convert the specified string to lowercase.
*/
SQMOD_NODISCARD const SQChar * StrToLowercase(const SQChar * str);
SQMOD_NODISCARD Buffer StrToLowercaseB(const SQChar * str);

/* ------------------------------------------------------------------------------------------------
 * Convert the specified string to uppercase.
*/
SQMOD_NODISCARD const SQChar * StrToUppercase(const SQChar * str);
SQMOD_NODISCARD Buffer StrToUppercaseB(const SQChar * str);

} // Namespace:: SqMod
