/*
 * MIT License
 *
 * Copyright (c) 2010 Serge Zaitsev
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef JSMN_H
#define JSMN_H

#include <stddef.h>

#include "jsmn_defines.h"

#ifdef JSMN_SHORT_TOKENS
typedef unsigned short jsmnint;
#else
typedef unsigned int jsmnint;
#endif
#define JSMN_NEG ((jsmnint)-1)

/**
 * JSON type identifier. Basic types are:
 */
typedef enum jsmntype {
  JSMN_UNDEFINED = 0x0000,
  JSMN_OBJECT = 0x0001, /*!< Object */
  JSMN_ARRAY = 0x0002,  /*!< Array */
  JSMN_STRING = 0x0004, /*!< String */
  JSMN_PRIMITIVE =
      0x0008, /*!< Other primitive: number, boolean (true/false) or null */

  JSMN_KEY = 0x0010,   /*!< is a key */
  JSMN_VALUE = 0x0020, /*!< is a value */

  /* Complex elements */
  JSMN_CONTAINER = JSMN_OBJECT | JSMN_ARRAY,
#ifndef JSMN_PERMISSIVE_KEY
  JSMN_KEY_TYPE = JSMN_STRING,
#else
  JSMN_KEY_TYPE = JSMN_STRING | JSMN_PRIMITIVE,
#endif
  JSMN_ANY_TYPE = JSMN_OBJECT | JSMN_ARRAY | JSMN_STRING | JSMN_PRIMITIVE,

  JSMN_OBJ_VAL = JSMN_OBJECT | JSMN_VALUE,
  JSMN_ARR_VAL = JSMN_ARRAY | JSMN_VALUE,
  JSMN_STR_KEY = JSMN_STRING | JSMN_KEY,
  JSMN_STR_VAL = JSMN_STRING | JSMN_VALUE,
  JSMN_PRI_VAL = JSMN_PRIMITIVE | JSMN_VALUE,
#ifdef JSMN_PERMISSIVE_KEY
  JSMN_OBJ_KEY = JSMN_OBJECT | JSMN_KEY,
  JSMN_ARR_KEY = JSMN_ARRAY | JSMN_KEY,
  JSMN_PRI_KEY = JSMN_PRIMITIVE | JSMN_KEY,
#endif

  /* Primitive extension */
  JSMN_PRI_LITERAL = 0x0040,  /*!< true, false, null */
  JSMN_PRI_INTEGER = 0x0080,  /*!< 0, 1 - 9 */
  JSMN_PRI_SIGN = 0x0100,     /*!< minus sign, '-' or plus sign, '+' */
  JSMN_PRI_DECIMAL = 0x0200,  /*!< deminal point '.' */
  JSMN_PRI_EXPONENT = 0x0400, /*!< exponent, 'e' or 'E' */

  JSMN_PRI_MINUS = JSMN_PRI_SIGN,

  /* Parsing validation, expectations, and state information */
  JSMN_PRI_CONTINUE = 0x0800, /*!< Allow a continuation of a PRIMITIVE */
  JSMN_CLOSE = 0x1000,        /*!< Close OBJECT '}' or ARRAY ']' */
  JSMN_COLON = 0x2000,        /*!< Colon ':' expected after KEY */
  JSMN_COMMA = 0x4000,        /*!< Comma ',' expected after VALUE */
  JSMN_INSD_OBJ = 0x8000,     /*!< Inside an OBJECT */

  /* Parsing rules */
  JSMN_ROOT_INIT = JSMN_ANY_TYPE | JSMN_VALUE,
#ifndef JSMN_PERMISSIVE
#ifndef JSMN_MULTIPLE_JSON
  JSMN_ROOT = JSMN_UNDEFINED,
#else
  JSMN_ROOT = JSMN_ANY_TYPE | JSMN_VALUE,
#endif
  JSMN_OPEN_OBJECT = JSMN_KEY_TYPE | JSMN_KEY | JSMN_CLOSE | JSMN_INSD_OBJ,
  JSMN_AFTR_OBJ_KEY = JSMN_VALUE | JSMN_INSD_OBJ | JSMN_COLON,
  JSMN_AFTR_OBJ_VAL = JSMN_KEY | JSMN_CLOSE | JSMN_INSD_OBJ | JSMN_COMMA,
  JSMN_OPEN_ARRAY = JSMN_ANY_TYPE | JSMN_VALUE | JSMN_CLOSE,
  JSMN_AFTR_ARR_VAL = JSMN_VALUE | JSMN_CLOSE | JSMN_COMMA,
  JSMN_AFTR_CLOSE = JSMN_CLOSE | JSMN_COMMA,
  JSMN_AFTR_COLON = JSMN_ANY_TYPE | JSMN_VALUE | JSMN_INSD_OBJ,
  JSMN_AFTR_COMMA_O = JSMN_KEY_TYPE | JSMN_KEY | JSMN_INSD_OBJ,
  JSMN_AFTR_COMMA_A = JSMN_ANY_TYPE | JSMN_VALUE,
#else
  JSMN_ROOT = JSMN_ANY_TYPE | JSMN_COLON | JSMN_COMMA,
  JSMN_ROOT_AFTR_O = JSMN_ANY_TYPE | JSMN_COMMA,
  JSMN_OPEN_OBJECT = JSMN_KEY_TYPE | JSMN_KEY | JSMN_CLOSE | JSMN_INSD_OBJ,
  JSMN_AFTR_OBJ_KEY = JSMN_VALUE | JSMN_INSD_OBJ | JSMN_COLON,
  JSMN_AFTR_OBJ_VAL = JSMN_ANY_TYPE | JSMN_CLOSE | JSMN_INSD_OBJ | JSMN_COMMA,
  JSMN_OPEN_ARRAY = JSMN_ANY_TYPE | JSMN_VALUE | JSMN_CLOSE,
  JSMN_AFTR_ARR_VAL = JSMN_ANY_TYPE | JSMN_CLOSE | JSMN_COMMA,
  JSMN_AFTR_CLOSE = JSMN_ANY_TYPE | JSMN_CLOSE | JSMN_COMMA,
  JSMN_AFTR_COLON = JSMN_ANY_TYPE | JSMN_VALUE | JSMN_INSD_OBJ,
  JSMN_AFTR_COLON_R = JSMN_ANY_TYPE | JSMN_VALUE,
  JSMN_AFTR_COMMA_O = JSMN_KEY_TYPE | JSMN_KEY | JSMN_INSD_OBJ,
  JSMN_AFTR_COMMA_A = JSMN_ANY_TYPE | JSMN_VALUE,
  JSMN_AFTR_COMMA_R = JSMN_ANY_TYPE,
#endif
} jsmntype;

/*!
 * JSMN Error Codes
 */
typedef enum jsmnerr {
  JSMN_SUCCESS = 0,
  JSMN_ERROR_NOMEM = -1, /*!< Not enough tokens were provided */
  JSMN_ERROR_LEN = -2,   /*!< Input data too long */
  JSMN_ERROR_INVAL = -3, /*!< Invalid character inside JSON string */
  JSMN_ERROR_PART =
      -4, /*!< The string is not a full JSON packet, more bytes expected */
  JSMN_ERROR_UNMATCHED_BRACKETS =
      -5, /*!< The JSON string has unmatched brackets */
} jsmnerr;

/*!
 * JSMN Boolean
 */
typedef enum jsmnbool {
  JSMN_FALSE = 0,
  JSMN_TRUE = 1,
} jsmnbool;

/**
 * JSON token description.
 */
typedef struct jsmntok {
  jsmntype type; /*!< type (object, array, string etc.) */
  jsmnint start; /*!< start position in JSON data string */
  jsmnint end;   /*!< end position in JSON data string */
  jsmnint size;  /*!< number of children */
#ifdef JSMN_PARENT_LINKS
  jsmnint parent; /*!< parent id */
#endif
#ifdef JSMN_NEXT_SIBLING
  jsmnint next_sibling; /*!< next sibling id */
#endif
} jsmntok;

/**
 * JSON parser
 *
 * Contains an array of token blocks available. Also stores
 * the string being parsed now and current position in that string.
 */
typedef struct jsmnparser {
  jsmnint pos;     /*!< offset in the JSON string */
  jsmnint toknext; /*!< next token to allocate */
  /*!< when tokens == NULL, keeps track of container types to a depth of
   * (sizeof(jsmnint) * 8) */
  jsmnint toksuper; /*!< superior token node, e.g. parent object or array */
  /*!< when tokens == NULL, toksuper represents container depth */
  jsmntype expected; /*!< Expected jsmn type(s) */
} jsmnparser;

/**
 * @brief Create JSON parser over an array of tokens
 *
 * @param[out] parser jsmn parser
 */
JSMN_API
void jsmn_init(jsmnparser *parser);

/**
 * @brief Run JSON parser
 *
 * It parses a JSON data string into and array of tokens, each
 * describing a single JSON object.
 *
 * @param[in,out] parser jsmn parser
 * @param[in] js JSON data string
 * @param[in] len JSON data string length
 * @param[in,out] tokens pointer to memory allocated for tokens or NULL
 * @param[in] num_tokens number of tokens allocated
 * @return jsmnint number of tokens found or ERRNO
 */
JSMN_API
jsmnint jsmn_parse(jsmnparser *parser, const char *js, const size_t len,
                     jsmntok *tokens, const size_t num_tokens);

#endif /* JSMN_H */
