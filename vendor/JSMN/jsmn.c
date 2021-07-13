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

#include "jsmn.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Allocates a fresh unused token from the token pool.
 */
static jsmntok *jsmn_alloc_token(jsmnparser *parser, jsmntok *tokens,
                                   const size_t num_tokens) {
  if (parser->toknext >= num_tokens) {
    return NULL;
  }

  jsmntok *tok;
  tok = &tokens[parser->toknext++];
  tok->start = tok->end = JSMN_NEG;
  tok->size = 0;
#ifdef JSMN_PARENT_LINKS
  tok->parent = JSMN_NEG;
#endif
#ifdef JSMN_NEXT_SIBLING
  tok->next_sibling = JSMN_NEG;
#endif
  return tok;
}

/**
 * Fills token type and boundaries.
 */
static void jsmn_fill_token(jsmntok *token, const jsmntype type,
                            const jsmnint start, const jsmnint end) {
  token->type = type;
  token->start = start;
  token->end = end;
  token->size = 0;
}

#ifdef JSMN_NEXT_SIBLING
/**
 * Set previous child's next_sibling to current token
 */
static void jsmn_next_sibling(jsmnparser *parser, jsmntok *tokens) {
  jsmnint sibling;

  /* Start with parent's first child */
  if (parser->toksuper != JSMN_NEG) {
    sibling = parser->toksuper + 1;
  } else {
    sibling = 0;
  }

  /* If the first child is the current token */
  if (sibling == parser->toknext - 1) {
    return;
  }

  /* Loop until we find previous sibling */
  while (tokens[sibling].next_sibling != JSMN_NEG) {
    sibling = tokens[sibling].next_sibling;
  }

  /* Set previous sibling's next_sibling to current token */
  tokens[sibling].next_sibling = parser->toknext - 1;
}
#endif

static jsmnbool is_whitespace(const char c) {
  if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
    return JSMN_TRUE;
  }
  return JSMN_FALSE;
}

static jsmnbool is_hexadecimal(const char c) {
  if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') ||
      (c >= 'a' && c <= 'f')) {
    return JSMN_TRUE;
  }
  return JSMN_FALSE;
}

/* TODO: Confusing function name */
static jsmnbool is_character(const char c) {
  if ((c >= 0x20 && c <= 0x21) || (c >= 0x23 && c <= 0x5B) || (c >= 0x5D)) {
    return JSMN_TRUE;
  }
  return JSMN_FALSE;
}

static jsmnbool is_special(const char c) {
  if (c == '{' || c == '}' || c == '[' || c == ']' || c == '"' || c == ':' ||
      c == ',') {
    return JSMN_TRUE;
  }
  return JSMN_FALSE;
}

/**
 * Fills next available token with JSON primitive.
 */
static jsmnint jsmn_parse_primitive(jsmnparser *parser, const char *js,
                                      const size_t len, jsmntok *tokens,
                                      const size_t num_tokens) {
  /* If a PRIMITIVE wasn't expected */
  if (!(parser->expected & (JSMN_PRIMITIVE | JSMN_PRI_CONTINUE))) {
    return JSMN_ERROR_INVAL;
  }

  jsmnint pos;
  jsmntype type;
  jsmntype expected = JSMN_CLOSE;

  /**
   * Find beginning of the primitive
   * TODO: See if it is really necessary.
   * Shouldn't parser stay at the last valid state in case of an error?
   * In this case it should be right before primitive is parsed.
   */
  if (!(parser->expected & JSMN_PRI_CONTINUE)) {
    pos = parser->pos;
  } else {
    if (tokens != NULL) {
      pos = tokens[parser->toknext - 1].start;
    } else {
      pos = parser->pos;
      while (pos != JSMN_NEG && !is_whitespace(js[pos]) &&
             !is_special(js[pos]) && is_character(js[pos])) {
        pos--;
      }
      pos++;
    }
  }
  type = JSMN_PRIMITIVE;

#ifndef JSMN_PERMISSIVE_PRIMITIVE
  if (js[pos] == 't' || js[pos] == 'f' || js[pos] == 'n') {
    char *literal = NULL;
    jsmnint size = 0;
    if (js[pos] == 't') {
      literal = "true";
      size = 4;
    } else if (js[pos] == 'f') {
      literal = "false";
      size = 5;
    } else if (js[pos] == 'n') {
      literal = "null";
      size = 4;
    }
    jsmnint i;
    for (i = 1, pos++; i < size; i++, pos++) {
      if (pos == len || js[pos] == '\0') {
        return JSMN_ERROR_PART;
      } else if (js[pos] != literal[i]) {
        return JSMN_ERROR_INVAL;
      }
    }
    type |= JSMN_PRI_LITERAL;
    if (pos == len || js[pos] == '\0') {
      goto found;
    }
  } else {
    expected = JSMN_PRI_MINUS | JSMN_PRI_INTEGER;
    for (; pos < len && js[pos] != '\0'; pos++) {
      switch (js[pos]) {
      case '0':
        if (!(expected & JSMN_PRI_INTEGER)) {
          return JSMN_ERROR_INVAL;
        }
        if (type & JSMN_PRI_EXPONENT) {
          expected = JSMN_PRI_INTEGER | JSMN_CLOSE;
        } else if (type & JSMN_PRI_DECIMAL) {
          expected = JSMN_PRI_INTEGER | JSMN_PRI_EXPONENT | JSMN_CLOSE;
        } else if (parser->pos == pos ||
                   (parser->pos + 1 == pos && (type & JSMN_PRI_MINUS))) {
          expected = JSMN_PRI_DECIMAL | JSMN_PRI_EXPONENT | JSMN_CLOSE;
        } else {
          expected = JSMN_PRI_INTEGER | JSMN_PRI_DECIMAL | JSMN_PRI_EXPONENT |
                     JSMN_CLOSE;
        }
        break;
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        if (!(expected & JSMN_PRI_INTEGER)) {
          return JSMN_ERROR_INVAL;
        }
        if (type & JSMN_PRI_EXPONENT) {
          expected = JSMN_PRI_INTEGER | JSMN_CLOSE;
        } else if (type & JSMN_PRI_DECIMAL) {
          expected = JSMN_PRI_INTEGER | JSMN_PRI_EXPONENT | JSMN_CLOSE;
        } else {
          expected = JSMN_PRI_INTEGER | JSMN_PRI_DECIMAL | JSMN_PRI_EXPONENT |
                     JSMN_CLOSE;
        }
        break;
      case '-':
        if (!(expected & JSMN_PRI_MINUS)) {
          return JSMN_ERROR_INVAL;
        }
        expected = JSMN_PRI_INTEGER;
        if (parser->pos == pos) {
          type |= JSMN_PRI_MINUS;
        }
        break;
      case '+':
        if (!(expected & JSMN_PRI_SIGN)) {
          return JSMN_ERROR_INVAL;
        }
        expected = JSMN_PRI_INTEGER;
        break;
      case '.':
        if (!(expected & JSMN_PRI_DECIMAL)) {
          return JSMN_ERROR_INVAL;
        }
        type |= JSMN_PRI_DECIMAL;
        expected = JSMN_PRI_INTEGER;
        break;
      case 'e':
      case 'E':
        if (!(expected & JSMN_PRI_EXPONENT)) {
          return JSMN_ERROR_INVAL;
        }
        type |= JSMN_PRI_EXPONENT;
        expected = JSMN_PRI_SIGN | JSMN_PRI_INTEGER;
        break;
      default:
        if (!(expected & JSMN_CLOSE)) {
          return JSMN_ERROR_INVAL;
        }
        goto check_primitive_border;
      }
    }
    if (!(expected & JSMN_CLOSE)) {
      return JSMN_ERROR_INVAL;
    } else {
      goto found;
    }
  }
check_primitive_border:
  switch (js[pos]) {
  case ' ':
  case '\t':
  case '\n':
  case '\r':
  case ',':
  case '}':
  case ']':
    goto found;
  case '"':
  case ':':
  case '{':
  case '[':
    return JSMN_ERROR_INVAL;
  case '\0':
    goto found;
  default:
    return JSMN_ERROR_INVAL;
  }
#else
  for (; pos < len && js[pos] != '\0'; pos++) {
    switch (js[pos]) {
    case ' ':
    case '\t':
    case '\n':
    case '\r':
    case ',':
    case '}':
    case ']':
    case ':':

    case '{':
    case '[':
    case '"':
      goto found;
    default: /* to quiet a warning from gcc */
      break;
    }
    if (!is_character(js[pos])) {
      return JSMN_ERROR_INVAL;
    }
  }
#endif

found:
  expected = parser->expected;
  if (parser->toksuper != JSMN_NEG) {
    /* OBJECT KEY, strict query */
    if ((parser->expected & (JSMN_KEY | JSMN_INSD_OBJ)) ==
        (JSMN_KEY | JSMN_INSD_OBJ)) {
      parser->expected = JSMN_AFTR_OBJ_KEY;
      type |= JSMN_KEY | JSMN_INSD_OBJ;
      /* OBJECT VALUE, VALUE is implicit */
    } else if (parser->expected & JSMN_INSD_OBJ) {
      parser->expected = JSMN_AFTR_OBJ_VAL;
      type |= JSMN_VALUE | JSMN_INSD_OBJ;
#ifdef JSMN_PERMISSIVE
      /* OBJECT VALUE at the ROOT level */
    } else if (parser->toksuper == JSMN_NEG) {
      parser->expected = JSMN_ROOT_AFTR_O;
      type |= JSMN_VALUE;
#endif
      /* ARRAY VALUE, VALUE is implicit */
    } else {
      parser->expected = JSMN_AFTR_ARR_VAL;
      type |= JSMN_VALUE;
    }
  } else {
    parser->expected = JSMN_ROOT;
    type |= JSMN_VALUE;
  }
  if (pos == len || js[pos] == '\0') {
    parser->expected |= JSMN_PRI_CONTINUE;
  }

  if (tokens == NULL) {
    parser->pos = pos - 1;
    return JSMN_SUCCESS;
  }

  jsmntok *token;
  if (!(expected & JSMN_PRI_CONTINUE)) {
    token = jsmn_alloc_token(parser, tokens, num_tokens);
    if (token == NULL) {
      parser->expected = expected;
      return JSMN_ERROR_NOMEM;
    }
    jsmn_fill_token(token, type, parser->pos, pos);
  } else {
    token = &tokens[parser->toknext - 1];
    jsmn_fill_token(token, type, token->start, pos);
  }
  parser->pos = pos;
#ifdef JSMN_PARENT_LINKS
  token->parent = parser->toksuper;
#endif
#ifdef JSMN_NEXT_SIBLING
  jsmn_next_sibling(parser, tokens);
#endif

  if (parser->toksuper != JSMN_NEG) {
    if (!(expected & JSMN_PRI_CONTINUE)) {
      tokens[parser->toksuper].size++;
    }

    if (!(tokens[parser->toksuper].type & JSMN_CONTAINER)) {
#ifdef JSMN_PARENT_LINKS
      parser->toksuper = tokens[parser->toksuper].parent;
#else
      jsmnint i;
      for (i = parser->toksuper; i != JSMN_NEG; i--) {
        if (tokens[i].type & JSMN_CONTAINER && tokens[i].end == JSMN_NEG) {
          parser->toksuper = i;
          break;
        }
      }
#ifdef JSMN_PERMISSIVE
      if (i == JSMN_NEG) {
        parser->toksuper = i;
      }
#endif
#endif
    }
  }
  parser->pos--;

  return JSMN_SUCCESS;
}

/**
 * Fills next token with JSON string.
 */
static jsmnint jsmn_parse_string(jsmnparser *parser, const char *js,
                                   const size_t len, jsmntok *tokens,
                                   const size_t num_tokens) {
  /* If a STRING wasn't expected */
  if (!(parser->expected & JSMN_STRING)) {
    return JSMN_ERROR_INVAL;
  }

  if (len >= JSMN_NEG) {
    return JSMN_ERROR_LEN;
  }

  jsmnint pos;
  pos = parser->pos;

  /* Skip starting quote */
  pos++;

  char c;
  for (; pos < len && js[pos] != '\0'; pos++) {
    c = js[pos];

    /* Quote: end of string */
    if (c == '\"') {
      jsmntype expected = parser->expected;
      jsmntype type;
      if (parser->toksuper != JSMN_NEG) {
        /* OBJECT KEY, strict query */
        if ((parser->expected & (JSMN_INSD_OBJ | JSMN_KEY)) ==
            (JSMN_INSD_OBJ | JSMN_KEY)) {
          parser->expected = JSMN_AFTR_OBJ_KEY;
          type = JSMN_STRING | JSMN_KEY | JSMN_INSD_OBJ;
          /* OBJECT VALUE, VALUE is implicit */
        } else if (parser->expected & JSMN_INSD_OBJ) {
          parser->expected = JSMN_AFTR_OBJ_VAL;
          type = JSMN_STRING | JSMN_VALUE | JSMN_INSD_OBJ;
#ifdef JSMN_PERMISSIVE
          /* OBJECT VALUE at the ROOT level */
        } else if (parser->toksuper == JSMN_NEG) {
          parser->expected = JSMN_ROOT_AFTR_O;
          type = JSMN_STRING | JSMN_VALUE;
#endif
          /* ARRAY VALUE, VALUE is implicit */
        } else {
          parser->expected = JSMN_AFTR_ARR_VAL;
          type = JSMN_STRING | JSMN_VALUE;
        }
      } else {
        parser->expected = JSMN_ROOT;
        type = JSMN_STRING | JSMN_VALUE;
      }

      if (tokens == NULL) {
        parser->pos = pos;
        return JSMN_SUCCESS;
      }

      jsmntok *token;
      token = jsmn_alloc_token(parser, tokens, num_tokens);
      if (token == NULL) {
        parser->expected = expected;
        return JSMN_ERROR_NOMEM;
      }
      jsmn_fill_token(token, type, parser->pos + 1, pos);
      parser->pos = pos;
#ifdef JSMN_PARENT_LINKS
      token->parent = parser->toksuper;
#endif
#ifdef JSMN_NEXT_SIBLING
      jsmn_next_sibling(parser, tokens);
#endif

      if (parser->toksuper != JSMN_NEG) {
        tokens[parser->toksuper].size++;

        if (!(tokens[parser->toksuper].type & JSMN_CONTAINER)) {
#ifdef JSMN_PARENT_LINKS
          parser->toksuper = tokens[parser->toksuper].parent;
#else
          jsmnint i;
          for (i = parser->toksuper; i != JSMN_NEG; i--) {
            if (tokens[i].type & JSMN_CONTAINER && tokens[i].end == JSMN_NEG) {
              parser->toksuper = i;
              break;
            }
          }
#ifdef JSMN_PERMISSIVE
          if (i == JSMN_NEG) {
            parser->toksuper = i;
          }
#endif
#endif
        }
      }

      return JSMN_SUCCESS;
    }

    /* Backslash: Quoted symbol expected */
    if (c == '\\' && pos + 1 < len) {
      pos++;
      switch (js[pos]) {
      /* Allowed escaped symbols */
      case '\"':
      case '\\':
      case '/':
      case 'b':
      case 'f':
      case 'n':
      case 'r':
      case 't':
        break;
      /* Allows escaped symbol \uXXXX */
      case 'u':
        pos++;
        jsmnint i;
        for (i = pos + 4; pos < i; pos++) {
          if (pos == len || js[pos] == '\0') {
            return JSMN_ERROR_PART;
          }
          /* If it isn't a hex character we have an error */
          if (!is_hexadecimal(js[pos])) {
            return JSMN_ERROR_INVAL;
          }
        }
        pos--;
        break;
      /* Unexpected symbol */
      default:
        return JSMN_ERROR_INVAL;
      }
    }

    /* form feed, new line, carraige return, tab, and vertical tab not allowed
     */
    else if (c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v') {
      return JSMN_ERROR_INVAL;
    }
  }
  return JSMN_ERROR_PART;
}

static jsmnint jsmn_parse_container_open(jsmnparser *parser, const char c,
                                           jsmntok *tokens,
                                           const size_t num_tokens) {
  /* If an OBJECT or ARRAY wasn't expected */
  if (!(parser->expected & JSMN_CONTAINER)) {
    return JSMN_ERROR_INVAL;
  }

  jsmntype type;
  if (c == '{') {
    parser->expected = JSMN_OPEN_OBJECT;
    type = JSMN_OBJECT | JSMN_VALUE;
  } else {
    parser->expected = JSMN_OPEN_ARRAY;
    type = JSMN_ARRAY | JSMN_VALUE;
  }

  if (tokens == NULL) {
    parser->toksuper++;
    if (parser->toksuper < (sizeof(jsmnint) * 8) &&
        parser->expected & JSMN_INSD_OBJ) {
      parser->toknext |= (1 << parser->toksuper);
    }
    return JSMN_SUCCESS;
  }

  if (parser->toksuper != JSMN_NEG &&
      tokens[parser->toksuper].type & JSMN_INSD_OBJ) {
    type |= JSMN_INSD_OBJ;
  }

  jsmntok *token;
  token = jsmn_alloc_token(parser, tokens, num_tokens);
  if (token == NULL) {
    return JSMN_ERROR_NOMEM;
  }
  jsmn_fill_token(token, type, parser->pos, JSMN_NEG);
#ifdef JSMN_PARENT_LINKS
  token->parent = parser->toksuper;
#endif
#ifdef JSMN_NEXT_SIBLING
  jsmn_next_sibling(parser, tokens);
#endif

  if (parser->toksuper != JSMN_NEG) {
    tokens[parser->toksuper].size++;
  }
  parser->toksuper = parser->toknext - 1;

  return JSMN_SUCCESS;
}

static jsmnint jsmn_parse_container_close(jsmnparser *parser, const char c,
                                            jsmntok *tokens) {
  /* If an OBJECT or ARRAY CLOSE wasn't expected */
  if (!(parser->expected & JSMN_CLOSE)) {
    return JSMN_ERROR_INVAL;
  }

  if (tokens == NULL) {
    if (parser->toksuper < (sizeof(jsmnint) * 8)) {
      jsmntype type;
      type = (c == '}' ? JSMN_OBJECT : JSMN_ARRAY);
      if ((((parser->toknext & (1 << parser->toksuper)) == 1) &&
           !(type & JSMN_OBJECT)) ||
          (((parser->toknext & (1 << parser->toksuper)) == 0) &&
           !(type & JSMN_ARRAY))) {
        return JSMN_ERROR_UNMATCHED_BRACKETS;
      }
      parser->toknext &= ~(1 << parser->toksuper);
    }
    parser->toksuper--;
  } else {
    jsmntype type;
    jsmntok *token;

    type = (c == '}' ? JSMN_OBJECT : JSMN_ARRAY);
#ifdef JSMN_PERMISSIVE
    if (parser->toksuper == JSMN_NEG) {
      return JSMN_ERROR_UNMATCHED_BRACKETS;
    }
#endif
    token = &tokens[parser->toksuper];
    if (!(token->type & type) || token->end != JSMN_NEG) {
      return JSMN_ERROR_UNMATCHED_BRACKETS;
    }
    token->end = parser->pos + 1;
#ifdef JSMN_PARENT_LINKS
    if (token->type & JSMN_INSD_OBJ) {
      if (tokens[token->parent].type & JSMN_CONTAINER) {
        parser->toksuper = token->parent;
      } else {
        parser->toksuper = tokens[token->parent].parent;
      }
    } else {
      parser->toksuper = token->parent;
    }
#else
    jsmnint i;
    for (i = parser->toksuper - 1; i != JSMN_NEG; i--) {
      if (tokens[i].type & JSMN_CONTAINER && tokens[i].end == JSMN_NEG) {
        parser->toksuper = i;
        break;
      }
    }
    if (i == JSMN_NEG) {
      parser->toksuper = i;
    }
#endif
  }

  if (parser->toksuper != JSMN_NEG) {
    parser->expected = JSMN_AFTR_CLOSE;
  } else {
    parser->expected = JSMN_ROOT;
  }

  return JSMN_SUCCESS;
}

static jsmnint jsmn_parse_colon(jsmnparser *parser, jsmntok *tokens) {
  /* If a COLON wasn't expected; strict check because it is a complex enum */
  if (!((parser->expected & JSMN_COLON) == JSMN_COLON)) {
    return JSMN_ERROR_INVAL;
  }

  if (parser->toksuper != JSMN_NEG) {
    parser->expected = JSMN_AFTR_COLON;
#ifdef JSMN_PERMISSIVE
  } else {
    parser->expected = JSMN_AFTR_COLON_R;
#endif
  }

  if (tokens == NULL) {
    return JSMN_SUCCESS;
  }

#ifdef JSMN_PERMISSIVE
  tokens[parser->toknext - 1].type &= ~JSMN_VALUE;
  tokens[parser->toknext - 1].type |= JSMN_KEY;
#endif

  parser->toksuper = parser->toknext - 1;

  return JSMN_SUCCESS;
}

static jsmnint jsmn_parse_comma(jsmnparser *parser, jsmntok *tokens) {
  /* If a COMMA wasn't expected; strict check because it is a complex enum */
  if (!((parser->expected & JSMN_COMMA) == JSMN_COMMA)) {
    return JSMN_ERROR_INVAL;
  }

  jsmntype type = JSMN_UNDEFINED;
  if (tokens == NULL) {
    if (parser->toksuper < (sizeof(jsmnint) * 8) &&
        parser->toknext & (1 << parser->toksuper)) {
      type = JSMN_INSD_OBJ;
    }
  } else {
    if (parser->toksuper != JSMN_NEG) {
      type = tokens[parser->toksuper].type;
    }
  }

  if (parser->toksuper != JSMN_NEG) {
    if (type & (JSMN_OBJECT | JSMN_INSD_OBJ)) {
      parser->expected = JSMN_AFTR_COMMA_O;
    } else {
      parser->expected = JSMN_AFTR_COMMA_A;
    }
#ifdef JSMN_PERMISSIVE
  } else {
    parser->expected = JSMN_AFTR_COMMA_R;
#endif
  }

  if (tokens == NULL) {
    return JSMN_SUCCESS;
  }

#ifdef JSMN_PERMISSIVE
  tokens[parser->toknext - 1].type |= JSMN_VALUE;
#endif

  return JSMN_SUCCESS;
}

/**
 * Parse JSON string and fill tokens.
 */
JSMN_API
jsmnint jsmn_parse(jsmnparser *parser, const char *js, const size_t len,
                     jsmntok *tokens, const size_t num_tokens) {
  jsmnint r;
  jsmnint count = parser->toknext;

  char c;
  for (; parser->pos < len && js[parser->pos] != '\0'; parser->pos++) {
#ifndef JSMN_MULTIPLE_JSON_FAIL
    if (parser->expected == JSMN_UNDEFINED) {
      break;
    }
#endif
    c = js[parser->pos];
    switch (c) {
    case '{':
    case '[':
      r = jsmn_parse_container_open(parser, c, tokens, num_tokens);
      if (r != JSMN_SUCCESS) {
        return r;
      }
      count++;
      break;
    case '}':
    case ']':
      r = jsmn_parse_container_close(parser, c, tokens);
      if (r != JSMN_SUCCESS) {
        return r;
      }
      break;
    case '\"':
      r = jsmn_parse_string(parser, js, len, tokens, num_tokens);
      if (r != JSMN_SUCCESS) {
        return r;
      }
      count++;
      break;
    case ':':
      r = jsmn_parse_colon(parser, tokens);
      if (r != JSMN_SUCCESS) {
        return r;
      }
      break;
    case ',':
      r = jsmn_parse_comma(parser, tokens);
      if (r != JSMN_SUCCESS) {
        return r;
      }
      break;
    /* Valid whitespace */
    case ' ':
    case '\t':
    case '\n':
    case '\r':
      break;
#ifndef JSMN_PERMISSIVE_PRIMITIVE
    /* rfc8259: PRIMITIVEs are numbers and booleans */
    case '-':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case 't':
    case 'f':
    case 'n':
#else
    /* In permissive mode every unquoted value is a PRIMITIVE */
    default:
#endif
      r = jsmn_parse_primitive(parser, js, len, tokens, num_tokens);
      if (r != JSMN_SUCCESS) {
        return r;
      }
      count++;
      break;

#ifndef JSMN_PERMISSIVE
    /* Unexpected char */
    default:
      return JSMN_ERROR_INVAL;
#endif
    }
  }

  if (parser->toksuper != JSMN_NEG) {
    return JSMN_ERROR_PART;
  }

  if (count == 0) {
    return JSMN_ERROR_INVAL;
  }

  while (parser->pos < len && is_whitespace(js[parser->pos])) {
    parser->pos++;
  }

  return count;
}

/**
 * Creates a new parser based over a given buffer with an array of tokens
 * available.
 */
JSMN_API
void jsmn_init(jsmnparser *parser) {
  parser->pos = 0;
  parser->toknext = 0;
  parser->toksuper = JSMN_NEG;
  parser->expected = JSMN_ROOT_INIT;
}

#ifdef __cplusplus
}
#endif
