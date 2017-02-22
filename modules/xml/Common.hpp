#ifndef _SQXML_COMMON_HPP_
#define _SQXML_COMMON_HPP_

// ------------------------------------------------------------------------------------------------
#include "Base/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#include <pugixml.hpp>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * SOFTWARE INFORMATION
*/
#define SQXML_NAME "Squirrel XML Module"
#define SQXML_AUTHOR "Sandu Liviu Catalin (S.L.C)"
#define SQXML_COPYRIGHT "Copyright (C) 2017 Sandu Liviu Catalin"
#define SQXML_HOST_NAME "SqModXMLHost"
#define SQXML_VERSION 001
#define SQXML_VERSION_STR "0.0.1"
#define SQXML_VERSION_MAJOR 0
#define SQXML_VERSION_MINOR 0
#define SQXML_VERSION_PATCH 1

// ------------------------------------------------------------------------------------------------
using namespace pugi;

// ------------------------------------------------------------------------------------------------
class Node;
class Text;
class Document;
class Attribute;
class XPathNode;
class XPathNodeSet;
class XPathVariable;
class XPathVariableSet;
class XPathVariableQuery;
class ParseResult;

} // Namespace:: SqMod

#endif // _SQXML_COMMON_HPP_
