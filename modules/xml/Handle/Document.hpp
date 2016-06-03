#ifndef _SQXML_HANDLE_DOCUMENT_HPP_
#define _SQXML_HANDLE_DOCUMENT_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Manages a reference counted xml document instance.
*/
class DocumentRef
{
    // --------------------------------------------------------------------------------------------
    friend class Document;

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

} // Namespace:: SqMod

#endif // _SQXML_HANDLE_DOCUMENT_HPP_
