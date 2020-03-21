#pragma once

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"

// ------------------------------------------------------------------------------------------------
#include "Library/Numeric/LongInt.hpp"

// ------------------------------------------------------------------------------------------------
#include <pugixml.hpp>
#include <utility>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * SOFTWARE INFORMATION
*/
#define SQXML_NAME "Squirrel XML Module"
#define SQXML_AUTHOR "Sandu Liviu Catalin (S.L.C)"
#define SQXML_COPYRIGHT "Copyright (C) 2018 Sandu Liviu Catalin"
#define SQXML_HOST_NAME "SqModXMLHost"
#define SQXML_VERSION 001
#define SQXML_VERSION_STR "0.0.1"
#define SQXML_VERSION_MAJOR 0
#define SQXML_VERSION_MINOR 0
#define SQXML_VERSION_PATCH 1

// ------------------------------------------------------------------------------------------------
using namespace pugi;

// ------------------------------------------------------------------------------------------------
class XmlNode;
class XmlText;
class XmlDocument;
class XmlAttribute;
class XPathNode;
class XPathNodeSet;
class XPathVariable;
class XPathVariableSet;
class XPathVariableQuery;
class XmlParseResult;

/* ------------------------------------------------------------------------------------------------
 * Manages a reference counted xml document instance.
*/
class DocumentRef
{
    // --------------------------------------------------------------------------------------------
    friend class XmlDocument;

public:

    // --------------------------------------------------------------------------------------------
    typedef xml_document    Type; /* The managed type. */

    // --------------------------------------------------------------------------------------------
    typedef Type*           Pointer; /* Pointer to the managed type. */
    typedef const Type*     ConstPtr; /* Constant pointer to the managed type. */

    // --------------------------------------------------------------------------------------------
    typedef Type&           Reference; /* Reference to the managed type. */
    typedef const Type&     ConstRef; /* Constant reference to the managed type. */

    // --------------------------------------------------------------------------------------------
    typedef unsigned int    Counter; /* Reference counter type. */

    /* --------------------------------------------------------------------------------------------
     * Validate the managed handle and throw exception if invalid.
    */
    void Validate() const;

private:

    // --------------------------------------------------------------------------------------------
    Pointer     m_Ptr; /* The document reader, writer and manager instance. */
    Counter*    m_Ref; /* Reference count to the managed instance. */

    /* --------------------------------------------------------------------------------------------
     * Grab a strong reference to a document instance.
    */
    void Grab()
    {
        if (m_Ptr)
        {
            ++(*m_Ref);
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Drop a strong reference to a document instance.
    */
    void Drop()
    {
        if (m_Ptr && --(*m_Ref) == 0)
        {
            delete m_Ptr;
            delete m_Ref;
            m_Ptr = NULL;
            m_Ref = NULL;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    DocumentRef(VoidP /* unused */)
        : m_Ptr(new Type())
        , m_Ref(new Counter(1))
    {
        /* ... */
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor (null).
    */
    DocumentRef()
        : m_Ptr(NULL), m_Ref(NULL)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    DocumentRef(const DocumentRef & o)
        : m_Ptr(o.m_Ptr), m_Ref(o.m_Ref)

    {
        Grab();
    }

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    DocumentRef(DocumentRef && o)
        : m_Ptr(o.m_Ptr), m_Ref(o.m_Ref)
    {
        o.m_Ptr = NULL;
        o.m_Ref = NULL;
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~DocumentRef()
    {
        Drop();
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    DocumentRef & operator = (const DocumentRef & o)
    {
        if (m_Ptr != o.m_Ptr)
        {
            Drop();
            m_Ptr = o.m_Ptr;
            m_Ref = o.m_Ref;
            Grab();
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    DocumentRef & operator = (DocumentRef && o)
    {
        if (m_Ptr != o.m_Ptr)
        {
            m_Ptr = o.m_Ptr;
            m_Ref = o.m_Ref;
            o.m_Ptr = NULL;
            o.m_Ref = NULL;
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two document instances.
    */
    bool operator == (const DocumentRef & o) const
    {
        return (m_Ptr == o.m_Ptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two document instances.
    */
    bool operator != (const DocumentRef & o) const
    {
        return (m_Ptr != o.m_Ptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean for use in boolean operations.
    */
    operator bool () const
    {
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance pointer.
    */
    operator Pointer ()
    {
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance pointer.
    */
    operator ConstPtr () const
    {
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance reference.
    */
    operator Reference ()
    {
        assert(m_Ptr);
        return *m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance reference.
    */
    operator ConstRef () const
    {
        assert(m_Ptr);
        return *m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Member operator for dereferencing the managed pointer.
    */
    Pointer operator -> () const
    {
        assert(m_Ptr);
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Indirection operator for obtaining a reference of the managed pointer.
    */
    Reference operator * () const
    {
        assert(m_Ptr);
        return *m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of active references to the managed instance.
    */
    Counter Count() const
    {
        return (m_Ptr && m_Ref) ? (*m_Ref) : 0;
    }
};

/* ------------------------------------------------------------------------------------------------
 * Allows the user to inspect the result of certain operations and act accordingly.
*/
class XmlParseResult
{
    // --------------------------------------------------------------------------------------------
    friend class XmlDocument;
    friend class XmlNode;

protected:

    // --------------------------------------------------------------------------------------------
    typedef xml_parse_result Result;

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    XmlParseResult(DocumentRef doc, const Result & result)
        : m_Doc(std::move(doc)), m_Result(result)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Validate the document reference and throw an error if invalid.
    */
    void Validate() const;

private:

    // ---------------------------------------------------------------------------------------------
    DocumentRef m_Doc; /* The main xml document instance. */
    Result      m_Result; /* The managed parse result. */

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    XmlParseResult()
        : m_Doc(), m_Result()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    XmlParseResult(const XmlParseResult & o)
        : m_Doc(o.m_Doc), m_Result(o.m_Result)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~XmlParseResult()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    XmlParseResult & operator = (const XmlParseResult & o)
    {
        m_Doc = o.m_Doc;
        m_Result = o.m_Result;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const XmlParseResult & o)
    {
        if (m_Result.status == o.m_Result.status)
        {
            return 0;
        }
        else if (m_Result.status > o.m_Result.status)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return m_Result.description();
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See whether this instance references a valid xml document.
    */
    bool IsValid() const
    {
        return m_Doc;
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to this document instance.
    */
    Uint32 GetRefCount() const
    {
        return m_Doc.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Cast to bool operator.
    */
    bool IsOk() const
    {
        return m_Result;
    }

    /* --------------------------------------------------------------------------------------------
     * Parsing status code.
    */
    Int32 GetStatus() const
    {
        return (Int32)m_Result.status;
    }

    /* --------------------------------------------------------------------------------------------
     * Last parsed offset. (in char_t units from start of input data)
    */
    SQInteger GetOffset() const
    {
        return (SQInteger)m_Result.offset;
    }

    /* --------------------------------------------------------------------------------------------
     * Source document encoding.
    */
    Int32 GetEncoding() const
    {
        return m_Result.encoding;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve error description as a string.
    */
    CSStr GetDescription() const
    {
        return m_Result.description();
    }

    /* --------------------------------------------------------------------------------------------
     * Check the parse result and throw the necessary errors.
    */
    void Check() const;
};

} // Namespace:: SqMod
