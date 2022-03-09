#pragma once

// ------------------------------------------------------------------------------------------------
#include "Core/Utility.hpp"

// ------------------------------------------------------------------------------------------------
#include <pugixml.hpp>
#include <utility>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
using namespace pugi;

// ------------------------------------------------------------------------------------------------
class XmlNode;
class XmlText;
class XmlDocument;
class XmlAttribute;
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
    void Validate() const
    {
        if (!m_Ptr)
        {
            STHROWF("Invalid XML document reference");
        }
    }

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
            m_Ptr = nullptr;
            m_Ref = nullptr;
        }
    }

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor (null).
    */
    DocumentRef()
        : m_Ptr(nullptr), m_Ref(nullptr)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    explicit DocumentRef(std::nullptr_t SQ_UNUSED_ARG(p)) //NOLINT (yes, I am using this constructor)
            : m_Ptr(new Type())
            , m_Ref(new Counter(1))
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
    DocumentRef(DocumentRef && o) noexcept
        : m_Ptr(o.m_Ptr), m_Ref(o.m_Ref)
    {
        o.m_Ptr = nullptr;
        o.m_Ref = nullptr;
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
    DocumentRef & operator = (const DocumentRef & o) //NOLINT (yes, I am checking for self assignment!)
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
    DocumentRef & operator = (DocumentRef && o) noexcept
    {
        if (m_Ptr != o.m_Ptr)
        {
            m_Ptr = o.m_Ptr;
            m_Ref = o.m_Ref;
            o.m_Ptr = nullptr;
            o.m_Ref = nullptr;
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an equality comparison between two document instances.
    */
    SQMOD_NODISCARD bool operator == (const DocumentRef & o) const
    {
        return (m_Ptr == o.m_Ptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Perform an inequality comparison between two document instances.
    */
    SQMOD_NODISCARD bool operator != (const DocumentRef & o) const
    {
        return (m_Ptr != o.m_Ptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to boolean for use in boolean operations.
    */
    SQMOD_NODISCARD operator bool () const //NOLINT (intentionally implicit)
    {
        return m_Ptr != nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance pointer.
    */
    SQMOD_NODISCARD operator Pointer () //NOLINT (intentionally implicit)
    {
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance pointer.
    */
    SQMOD_NODISCARD operator ConstPtr () const //NOLINT (intentionally implicit)
    {
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance reference.
    */
    SQMOD_NODISCARD operator Reference () //NOLINT (intentionally implicit)
    {
        assert(m_Ptr);
        return *m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance reference.
    */
    SQMOD_NODISCARD operator ConstRef () const //NOLINT (intentionally implicit)
    {
        assert(m_Ptr);
        return *m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Member operator for dereferencing the managed pointer.
    */
    SQMOD_NODISCARD Pointer operator -> () const
    {
        assert(m_Ptr);
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Indirection operator for obtaining a reference of the managed pointer.
    */
    SQMOD_NODISCARD Reference operator * () const
    {
        assert(m_Ptr);
        return *m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of active references to the managed instance.
    */
    SQMOD_NODISCARD Counter Count() const
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
    void Validate() const
    {
        // Is the documen handle valid?
        if (!m_Doc)
        {
            STHROWF("Invalid XML document reference");
        }
    }

private:

    // ---------------------------------------------------------------------------------------------
    DocumentRef m_Doc{}; /* The main xml document instance. */
    Result      m_Result{}; /* The managed parse result. */

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    XmlParseResult() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    XmlParseResult(const XmlParseResult & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    XmlParseResult & operator = (const XmlParseResult & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    SQMOD_NODISCARD int32_t Cmp(const XmlParseResult & o) const
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
    SQMOD_NODISCARD const SQChar * ToString() const
    {
        return m_Result.description();
    }

    /* --------------------------------------------------------------------------------------------
     * See whether this instance references a valid xml document.
    */
    SQMOD_NODISCARD bool IsValid() const
    {
        return m_Doc;
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to this document instance.
    */
    SQMOD_NODISCARD uint32_t GetRefCount() const
    {
        return m_Doc.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Cast to bool operator.
    */
    SQMOD_NODISCARD bool IsOk() const
    {
        return m_Result;
    }

    /* --------------------------------------------------------------------------------------------
     * Parsing status code.
    */
    SQMOD_NODISCARD int32_t GetStatus() const
    {
        return (int32_t)m_Result.status;
    }

    /* --------------------------------------------------------------------------------------------
     * Last parsed offset. (in char_t units from start of input data)
    */
    SQMOD_NODISCARD SQInteger GetOffset() const
    {
        return (SQInteger)m_Result.offset;
    }

    /* --------------------------------------------------------------------------------------------
     * Source document encoding.
    */
    SQMOD_NODISCARD int32_t GetEncoding() const
    {
        return m_Result.encoding;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve error description as a string.
    */
    SQMOD_NODISCARD const SQChar * GetDescription() const
    {
        return m_Result.description();
    }

    /* --------------------------------------------------------------------------------------------
     * Check the parse result and throw the necessary errors.
    */
    void Check() const
    {
        if (m_Result.status != status_ok)
        {
            STHROWF("XML parse error [%s]", m_Result.description());
        }
    }
};

/* ------------------------------------------------------------------------------------------------
 * Class that can read/write and alter the contents of XML files.
*/
class XmlDocument
{
protected:

    // --------------------------------------------------------------------------------------------
    typedef xml_document Type;

    /* --------------------------------------------------------------------------------------------
     * See if the document is allowed to overwrite its contents and throw an error if not.
    */
    void CanLoad() const
    {
        // Is the document even valid?
        m_Doc.Validate();
        // Are there any other references?
        if (m_Doc.Count() > 1)
        {
            // To load new values now, would mean to cause undefined behavior in existing references
            STHROWF("Loading is disabled while document is referenced");
        }
    }

private:

    // ---------------------------------------------------------------------------------------------
    DocumentRef  m_Doc; // The main xml document instance.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    XmlDocument()
        : m_Doc(nullptr)
    {
        /*...*/
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    XmlDocument(const XmlDocument & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    XmlDocument & operator = (const XmlDocument & o) = delete;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    SQMOD_NODISCARD int32_t Cmp(const XmlDocument & o) const
    {
        if (m_Doc && !o.m_Doc)
        {
            return 1;
        }
        else if (!m_Doc && o.m_Doc)
        {
            return -1;
        }
        else if (!m_Doc && !o.m_Doc)
        {
            return 0;
        }

        if (*m_Doc == *o.m_Doc)
        {
            return 0;
        }
        else if (*m_Doc > *o.m_Doc)
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
    SQMOD_NODISCARD const SQChar * ToString() const
    {
        // Do we manage a valid document?
        if (m_Doc)
        {
            return m_Doc->name();
        }
        // Default to an empty name
        return _SC("");
    }

    /* --------------------------------------------------------------------------------------------
     * See whether this instance references a valid xml document.
    */
    SQMOD_NODISCARD bool IsValid() const
    {
        return m_Doc;
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to this document instance.
    */
    SQMOD_NODISCARD uint32_t GetRefCount() const
    {
        return m_Doc.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Removes all nodes, leaving the empty document.
    */
    void Reset0()
    {
        // Validate the document handle
        m_Doc.Validate();
        // Perform the requested operation
        m_Doc->reset();
    }

    /* --------------------------------------------------------------------------------------------
     * Removes all nodes, then copies the entire contents of the specified document.
    */
    void Reset1(const XmlDocument & doc)
    {
        // Validate the document handles
        m_Doc.Validate();
        doc.m_Doc.Validate();
        // Perform the requested operation
        m_Doc->reset(*(doc.m_Doc));
    }

    /* --------------------------------------------------------------------------------------------
     * Load document from zero-terminated string. (LoadString collides with the windows api)
    */
    XmlParseResult LoadData1(StackStrF & source)
    {
        // Make sure that we are allowed to load in data
        CanLoad();
        // Perform the requested operation and return the result
        return XmlParseResult(m_Doc, m_Doc->load_string(source.mPtr));
    }

    /* --------------------------------------------------------------------------------------------
     * Load document from zero-terminated string. (LoadString collides with the windows api)
    */
    XmlParseResult LoadData2(StackStrF & source, uint32_t options)
    {
        // Make sure that we are allowed to load in data
        CanLoad();
        // Perform the requested operation and return the result
        return XmlParseResult(m_Doc, m_Doc->load_string(source.mPtr, options));
    }

    /* --------------------------------------------------------------------------------------------
     * Load document from file on disk.
    */
    XmlParseResult LoadFile1(StackStrF & filepath)
    {
        // Make sure that we are allowed to load in data
        CanLoad();
        // Perform the requested operation and return the result
        return XmlParseResult(m_Doc, m_Doc->load_file(filepath.mPtr));
    }

    /* --------------------------------------------------------------------------------------------
     * Load document from file on disk.
    */
    XmlParseResult LoadFile2(StackStrF & filepath, uint32_t options)
    {
        // Make sure that we are allowed to load in data
        CanLoad();
        // Perform the requested operation and return the result
        return XmlParseResult(m_Doc, m_Doc->load_file(filepath.mPtr, options));
    }

    /* --------------------------------------------------------------------------------------------
     * Load document from file on disk.
    */
    XmlParseResult LoadFile3(StackStrF & filepath, uint32_t options, int32_t encoding)
    {
        // Make sure that we are allowed to load in data
        CanLoad();
        // Perform the requested operation and return the result
        return XmlParseResult(m_Doc, m_Doc->load_file(filepath.mPtr, options,
                                                      static_cast< xml_encoding >(encoding)));
    }

    /* --------------------------------------------------------------------------------------------
     * Save XML to file on disk.
    */
    void SaveFile1(StackStrF & filepath)
    {
        // Validate the document handle
        m_Doc.Validate();
        // Perform the requested operation
        m_Doc->save_file(filepath.mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Save XML to file on disk.
    */
    void SaveFile2(StackStrF & filepath, StackStrF & indent)
    {
        // Validate the document handle
        m_Doc.Validate();
        // Perform the requested operation
        m_Doc->save_file(filepath.mPtr, indent.mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Save XML to file on disk.
    */
    void SaveFile3(StackStrF & filepath, StackStrF & indent, uint32_t format)
    {
        // Validate the document handle
        m_Doc.Validate();
        // Perform the requested operation
        m_Doc->save_file(filepath.mPtr, indent.mPtr, format);
    }

    /* --------------------------------------------------------------------------------------------
     * Save XML to file on disk.
    */
    void SaveFile4(StackStrF & filepath, StackStrF & indent, uint32_t format, int32_t encoding)
    {
        // Validate the document handle
        m_Doc.Validate();
        // Perform the requested operation
        m_Doc->save_file(filepath.mPtr, indent.mPtr, format, static_cast< xml_encoding >(encoding));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the document root node.
    */
    SQMOD_NODISCARD XmlNode GetNode() const;
};

/* ------------------------------------------------------------------------------------------------
 * A light-weight handle for manipulating nodes in DOM tree.
*/
class XmlNode
{
    // --------------------------------------------------------------------------------------------
    friend class XmlDocument;
    friend class XmlText;

protected:

    // --------------------------------------------------------------------------------------------
    typedef xml_node Type;

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    XmlNode(DocumentRef doc, const Type & node)
        : m_Doc(std::move(doc)), m_Node(node)
    {
        /* ... */
    }

private:

    // ---------------------------------------------------------------------------------------------
    DocumentRef  m_Doc{}; // The main xml document instance.
    Type         m_Node{}; // The managed document node.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    XmlNode() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    XmlNode(const XmlNode & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    XmlNode & operator = (const XmlNode & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    SQMOD_NODISCARD int32_t Cmp(const XmlNode & o)
    {
        if (m_Node == o.m_Node)
        {
            return 0;
        }
        else if (m_Node > o.m_Node)
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
    SQMOD_NODISCARD const SQChar * ToString() const
    {
        return m_Node.value();
    }

    /* --------------------------------------------------------------------------------------------
     * See whether this instance references a valid xml document.
    */
    SQMOD_NODISCARD bool IsValid() const
    {
        return m_Doc;
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to this document instance.
    */
    SQMOD_NODISCARD uint32_t GetRefCount() const
    {
        return m_Doc.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the node is empty.
    */
    SQMOD_NODISCARD bool IsEmpty() const
    {
        return m_Node.empty();
    }

    /* --------------------------------------------------------------------------------------------
     * Get hash value (unique for handles to the same object).
    */
    SQMOD_NODISCARD SQInteger GetHashValue() const
    {
        return static_cast< SQInteger >(m_Node.hash_value());
    }

    /* --------------------------------------------------------------------------------------------
     * Get node offset in parsed file/string (in char_t units) for debugging purposes.
    */
    SQMOD_NODISCARD SQInteger GetOffsetDebug() const
    {
        return static_cast< SQInteger >(m_Node.offset_debug());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve node type.
    */
    SQMOD_NODISCARD int32_t GetType() const
    {
        return static_cast< int32_t >(m_Node.type());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve node name.
    */
    SQMOD_NODISCARD const SQChar * GetName() const
    {
        return m_Node.name();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify node name.
    */
    XmlNode & SetName(StackStrF & name)
    {
        if (!m_Node.set_name(name.mPtr))
        {
            STHROWF("Unable to set XML node name");
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the node name.
    */
    bool ApplyName(StackStrF & name)
    {
        return m_Node.set_name(name.mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve node value.
    */
    SQMOD_NODISCARD const SQChar * GetValue() const
    {
        return m_Node.value();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify node value.
    */
    XmlNode & SetValue(StackStrF & name)
    {
        if (!m_Node.set_value(name.mPtr))
        {
            STHROWF("Unable to set XML node value");
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the node value.
    */
    bool ApplyValue(StackStrF & value)
    {
        return m_Node.set_value(value.mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Parses buffer as an XML document fragment and appends all nodes as children of this node.
    */
    XmlParseResult AppendBuffer1(StackStrF & source)
    {
        // Is the specified source buffer even valid?
        if (source.mLen > 0 && source.mPtr)
        {
            return XmlParseResult(m_Doc, m_Node.append_buffer(source.mPtr, source.mLen * sizeof(SQChar)));
        }
        // Return the default result
        return XmlParseResult();
    }

    /* --------------------------------------------------------------------------------------------
     * Parses buffer as an XML document fragment and appends all nodes as children of this node.
    */
    XmlParseResult AppendBuffer2(uint32_t options, StackStrF & source)
    {
        // Is the specified source buffer even valid?
        if (source.mLen > 0 && source.mPtr)
        {
            return XmlParseResult(m_Doc, m_Node.append_buffer(source.mPtr, source.mLen * sizeof(SQChar), options));
        }
        // Return the default result
        return XmlParseResult();
    }

    /* --------------------------------------------------------------------------------------------
     * Parses buffer as an XML document fragment and appends all nodes as children of this node.
    */
    XmlParseResult AppendBuffer3(uint32_t options, int32_t encoding, StackStrF & source)
    {
        // Is the specified source buffer even valid?
        if (source.mLen > 0 && source.mPtr)
        {
            return XmlParseResult(m_Doc, m_Node.append_buffer(source.mPtr, source.mLen * sizeof(SQChar),
                                                              options, (xml_encoding)encoding));
        }
        // Return the default result
        return XmlParseResult();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the first child attribute.
    */
    SQMOD_NODISCARD XmlAttribute GetFirstAttr() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last child attribute.
    */
    SQMOD_NODISCARD XmlAttribute GetLastAttr() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the first child node.
    */
    SQMOD_NODISCARD XmlNode GetFirstChild() const
    {
        return XmlNode(m_Doc, m_Node.first_child());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last child node.
    */
    SQMOD_NODISCARD XmlNode GetLastChild() const
    {
        return XmlNode(m_Doc, m_Node.last_child());
    }

    /* --------------------------------------------------------------------------------------------
     * Get next sibling in the children list of the parent node.
    */
    SQMOD_NODISCARD XmlNode GetNextSibling() const
    {
        return XmlNode(m_Doc, m_Node.next_sibling());
    }

    /* --------------------------------------------------------------------------------------------
     * Get previous sibling in the children list of the parent node
    */
    SQMOD_NODISCARD XmlNode GetPrevSibling() const
    {
        return XmlNode(m_Doc, m_Node.previous_sibling());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the parent node.
    */
    SQMOD_NODISCARD XmlNode GetParent() const
    {
        return XmlNode(m_Doc, m_Node.parent());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the root node.
    */
    SQMOD_NODISCARD XmlNode GetRoot() const
    {
        return XmlNode(m_Doc, m_Node.root());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the text object for the current node.
    */
    SQMOD_NODISCARD XmlText GetText() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve child node with the specified name.
    */
    XmlNode Child(StackStrF & name) const
    {
        return XmlNode(m_Doc, m_Node.child(name.mPtr));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve child attribute with the specified name.
    */
    XmlAttribute GetAttribute(StackStrF & name) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve next sibling with the specified name.
    */
    SQMOD_NODISCARD XmlNode NextSibling(StackStrF & name) const
    {
        return XmlNode(m_Doc, m_Node.next_sibling(name.mPtr));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve previous sibling with the specified name.
    */
    SQMOD_NODISCARD XmlNode PrevSibling(StackStrF & name) const
    {
        return XmlNode(m_Doc, m_Node.previous_sibling(name.mPtr));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve child attribute, starting the search from a hint.
    */
    SQMOD_NODISCARD XmlAttribute AttributeFrom(StackStrF & name, XmlAttribute & attr) const;

    /* --------------------------------------------------------------------------------------------
     * Get child value of current node; that is, value of the first child node of type PCDATA/CDATA.
    */
    SQMOD_NODISCARD const SQChar * GetChildValue() const
    {
        return m_Node.child_value();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve child value of child with specified name.
    */
    SQMOD_NODISCARD const SQChar * ChildValue(StackStrF & name) const
    {
        return m_Node.child_value(name.mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Append a child attribute with the specified name.
    */
    XmlAttribute AppendAttr(StackStrF & name);

    /* --------------------------------------------------------------------------------------------
     * Prepend a child attribute with the specified name.
    */
    XmlAttribute PrependAttr(StackStrF & name);

    /* --------------------------------------------------------------------------------------------
     * Insert a child attribute with the specified name, after the specified node.
    */
    XmlAttribute InsertAttrAfter(StackStrF & name, const XmlAttribute & attr);

    /* --------------------------------------------------------------------------------------------
     * Insert a child attribute with the specified name, before the specified node.
    */
    XmlAttribute InsertAttrBefore(StackStrF & name, const XmlAttribute & attr);

    /* --------------------------------------------------------------------------------------------
     * Append a copy of the specified attribute as a child.
    */
    XmlAttribute AppendAttrCopy(const XmlAttribute & proto);

    /* --------------------------------------------------------------------------------------------
     * Prepend a copy of the specified attribute as a child.
    */
    XmlAttribute PrependAttrCopy(const XmlAttribute & proto);

    /* --------------------------------------------------------------------------------------------
     * Insert a copy of the specified attribute as a child after the specified attribute.
    */
    XmlAttribute InsertAttrCopyAfter(const XmlAttribute & proto, const XmlAttribute & attr);

    /* --------------------------------------------------------------------------------------------
     * Insert a copy of the specified attribute as a child before the specified attribute.
    */
    XmlAttribute InsertAttrCopyBefore(const XmlAttribute & proto, const XmlAttribute & attr);

    /* --------------------------------------------------------------------------------------------
     * Append a basic child node with the specified name.
    */
    XmlNode AppendChild(StackStrF & name)
    {
        return XmlNode(m_Doc, m_Node.append_child(name.mPtr));
    }

    /* --------------------------------------------------------------------------------------------
     * Prepend a basic child node with the specified name.
    */
    XmlNode PrependChild(StackStrF & name)
    {
        return XmlNode(m_Doc, m_Node.prepend_child(name.mPtr));
    }

    /* --------------------------------------------------------------------------------------------
     * Append a basic child node.
    */
    XmlNode AppendChildNode()
    {
        return XmlNode(m_Doc, m_Node.append_child());
    }

    /* --------------------------------------------------------------------------------------------
     * Prepend a basic child node.
    */
    XmlNode PrependChildNode()
    {
        return XmlNode(m_Doc, m_Node.prepend_child());
    }

    /* --------------------------------------------------------------------------------------------
     * Append a basic child node with the specified type.
    */
    XmlNode AppendChildType(int32_t type)
    {
        return XmlNode(m_Doc, m_Node.append_child(static_cast< xml_node_type >(type)));
    }

    /* --------------------------------------------------------------------------------------------
     * Prepend a basic child node with the specified type.
    */
    XmlNode PrependChildType(int32_t type)
    {
        return XmlNode(m_Doc, m_Node.prepend_child(static_cast< xml_node_type >(type)));
    }

    /* --------------------------------------------------------------------------------------------
     * Insert a basic child node with the specified name, after the specified node.
    */
    XmlNode InsertChildAfter(StackStrF & name, const XmlNode & node)
    {
        return XmlNode(m_Doc, m_Node.insert_child_after(name.mPtr, node.m_Node));
    }

    /* --------------------------------------------------------------------------------------------
     * Insert a basic child node with the specified name, before the specified node.
    */
    XmlNode InsertChildBefore(StackStrF & name, const XmlNode & node)
    {
        return XmlNode(m_Doc, m_Node.insert_child_before(name.mPtr, node.m_Node));
    }

    /* --------------------------------------------------------------------------------------------
     * Insert a basic child node with the specified type, after the specified node.
    */
    XmlNode InsertChildTypeAfter(int32_t type, const XmlNode & node)
    {
        return XmlNode(m_Doc, m_Node.insert_child_after(static_cast< xml_node_type >(type),
                                                        node.m_Node));
    }

    /* --------------------------------------------------------------------------------------------
     * Insert a basic child node with the specified type, before the specified node.
    */
    XmlNode InsertChildTypeBefore(int32_t type, const XmlNode & node)
    {
        return XmlNode(m_Doc, m_Node.insert_child_before(static_cast< xml_node_type >(type),
                                                        node.m_Node));
    }

    /* --------------------------------------------------------------------------------------------
     * Append a copy of the specified node as a child.
    */
    XmlNode AppendCopy(const XmlNode & proto)
    {
        return XmlNode(m_Doc, m_Node.append_copy(proto.m_Node));
    }

    /* --------------------------------------------------------------------------------------------
     * Prepend a copy of the specified node as a child.
    */
    XmlNode PrependCopy(const XmlNode & proto)
    {
        return XmlNode(m_Doc, m_Node.prepend_copy(proto.m_Node));
    }

    /* --------------------------------------------------------------------------------------------
     * Insert a copy of the specified node as a child after the specified node.
    */
    XmlNode InsertCopyAfter(const XmlNode & proto, const XmlNode & node)
    {
        return XmlNode(m_Doc, m_Node.insert_copy_after(proto.m_Node, node.m_Node));
    }

    /* --------------------------------------------------------------------------------------------
     * Insert a copy of the specified node as a child before the specified node.
    */
    XmlNode InsertCopyBefore(const XmlNode & proto, const XmlNode & node)
    {
        return XmlNode(m_Doc, m_Node.insert_copy_before(proto.m_Node, node.m_Node));
    }

    /* --------------------------------------------------------------------------------------------
     * Append the specified node as a child and take ownership of it.
    */
    XmlNode AppendMove(const XmlNode & proto)
    {
        return XmlNode(m_Doc, m_Node.append_copy(proto.m_Node));
    }

    /* --------------------------------------------------------------------------------------------
     * Prepend the specified node as a child and take ownership of it.
    */
    XmlNode PrependMove(const XmlNode & proto)
    {
        return XmlNode(m_Doc, m_Node.prepend_copy(proto.m_Node));
    }

    /* --------------------------------------------------------------------------------------------
     * Insert the specified node as a child after the specified node and take ownership of it.
    */
    XmlNode InsertMoveAfter(const XmlNode & proto, const XmlNode & node)
    {
        return XmlNode(m_Doc, m_Node.insert_copy_after(proto.m_Node, node.m_Node));
    }

    /* --------------------------------------------------------------------------------------------
     * Insert the specified node as a child before the specified node and take ownership of it.
    */
    XmlNode InsertMoveBefore(const XmlNode & proto, const XmlNode & node)
    {
        return XmlNode(m_Doc, m_Node.insert_copy_before(proto.m_Node, node.m_Node));
    }

    /* --------------------------------------------------------------------------------------------
     * Remove the child attribute matching the specified name.
    */
    bool RemoveAttr(StackStrF & name)
    {
        return m_Node.remove_attribute(name.mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Remove the specified attribute.
    */
    bool RemoveAttrInst(const XmlAttribute & attr);

    /* --------------------------------------------------------------------------------------------
     * Remove the child node matching the specified name.
    */
    bool RemoveChild(StackStrF & name)
    {
        return m_Node.remove_child(name.mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Remove the specified node.
    */
    bool RemoveChildInst(const XmlNode & node)
    {
        return m_Node.remove_child(node.m_Node);
    }

    /* --------------------------------------------------------------------------------------------
     * Find child node by attribute name/value.
    */
    SQMOD_NODISCARD XmlNode FindChildByAttr2(StackStrF & attr_name, StackStrF & attr_value) const
    {
        return XmlNode(m_Doc, m_Node.find_child_by_attribute(attr_name.mPtr, attr_value.mPtr));
    }

    /* --------------------------------------------------------------------------------------------
     * Find child node by attribute name/value.
    */
    SQMOD_NODISCARD XmlNode FindChildByAttr3(StackStrF & name, StackStrF & attr_name, StackStrF & attr_value) const
    {
        return XmlNode(m_Doc, m_Node.find_child_by_attribute(name.mPtr, attr_name.mPtr, attr_value.mPtr));
    }

    /* --------------------------------------------------------------------------------------------
     * Search for a node by path consisting of node names and . or .. elements.
    */
    SQMOD_NODISCARD XmlNode FindElemByPath(StackStrF & path, SQChar delimiter) const
    {
        return XmlNode(m_Doc, m_Node.first_element_by_path(path.mPtr, delimiter));
    }
};

/* ------------------------------------------------------------------------------------------------
 * A light-weight handle for manipulating attributes in DOM tree.
*/
class XmlAttribute
{
    // --------------------------------------------------------------------------------------------
    friend class XmlNode;

protected:

    // --------------------------------------------------------------------------------------------
    typedef xml_attribute Type;

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    XmlAttribute(DocumentRef doc, const Type & attr)
        : m_Doc(std::move(doc)), m_Attr(attr)
    {
        /* ... */
    }

private:

    // ---------------------------------------------------------------------------------------------
    DocumentRef m_Doc{}; // The main xml document instance.
    Type        m_Attr{}; // The managed node attribute.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    XmlAttribute() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    XmlAttribute(const XmlAttribute & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    XmlAttribute & operator = (const XmlAttribute & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    SQMOD_NODISCARD int32_t Cmp(const XmlAttribute & o)
    {
        if (m_Attr == o.m_Attr)
        {
            return 0;
        }
        else if (m_Attr > o.m_Attr)
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
    SQMOD_NODISCARD const SQChar * ToString() const
    {
        return m_Attr.value();
    }

    /* --------------------------------------------------------------------------------------------
     * See whether this instance references a valid xml document.
    */
    SQMOD_NODISCARD bool IsValid() const
    {
        return m_Doc;
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to this document instance.
    */
    SQMOD_NODISCARD uint32_t GetRefCount() const
    {
        return m_Doc.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the attribute is empty.
    */
    SQMOD_NODISCARD bool IsEmpty() const
    {
        return m_Attr.empty();
    }

    /* --------------------------------------------------------------------------------------------
     * Get hash value (unique for handles to the same object).
    */
    SQMOD_NODISCARD SQInteger GetHashValue() const
    {
        return (SQInteger)m_Attr.hash_value();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve attribute name.
    */
    SQMOD_NODISCARD const SQChar * GetName() const
    {
        return m_Attr.name();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve attribute name.
    */
    XmlAttribute SetName(StackStrF & name)
    {
        if (!m_Attr.set_name(name.mPtr))
        {
            STHROWF("Unable to set XML attribute name");
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the attribute name.
    */
    bool ApplyName(StackStrF & name)
    {
        return m_Attr.set_name(name.mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve attribute value.
    */
    SQMOD_NODISCARD const SQChar * GetValue() const
    {
        return m_Attr.value();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve attribute value.
    */
    XmlAttribute SetValue(StackStrF & name)
    {
        if (!m_Attr.set_value(name.mPtr))
        {
            STHROWF("Unable to set XML attribute value");
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the attribute value.
    */
    bool ApplyValue(StackStrF & value)
    {
        return m_Attr.set_value(value.mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a string or the specified default value if empty.
    */
    SQMOD_NODISCARD const SQChar * AsString(StackStrF & def) const
    {
        return m_Attr.as_string(def.mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a integer or the specified default value if empty.
    */
    SQMOD_NODISCARD int32_t AsInt(int32_t def) const
    {
        return m_Attr.as_int(def);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a unsigned integer or the specified default value if empty.
    */
    SQMOD_NODISCARD uint32_t AsUint(uint32_t def) const
    {
        return m_Attr.as_uint(def);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a floating point or the specified default value if empty.
    */
    SQMOD_NODISCARD SQFloat AsFloat(SQFloat def) const
    {
        return (SQFloat)m_Attr.as_float(def);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a double floating point or the specified default value if empty.
    */
    SQMOD_NODISCARD SQFloat AsDouble(SQFloat def) const
    {
        return (SQFloat)m_Attr.as_double(def);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a long integer or the specified default value if empty.
    */
    SQMOD_NODISCARD SQInteger AsLong(SQInteger def) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a unsigned long integer or the specified default value if empty.
    */
    SQMOD_NODISCARD SQInteger AsUlong(SQInteger def) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a boolean or the specified default value if empty.
    */
    SQMOD_NODISCARD bool AsBool(bool def) const
    {
        return m_Attr.as_bool(def);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a string.
    */
    bool ApplyString(StackStrF & value)
    {
        return m_Attr.set_value(value.mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a integer.
    */
    bool ApplyInt(int32_t value)
    {
        return m_Attr.set_value(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a unsigned integer.
    */
    bool ApplyUint(uint32_t value)
    {
        return m_Attr.set_value(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a floating point.
    */
    bool ApplyFloat(SQFloat value)
    {
        return m_Attr.set_value(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a double floating point.
    */
    bool ApplyDouble(SQFloat value)
    {
        return m_Attr.set_value(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a long integer.
    */
    bool ApplyLong(SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a unsigned long integer.
    */
    bool ApplyUlong(SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a boolean.
    */
    bool ApplyBool(bool value)
    {
        return m_Attr.set_value(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a string.
    */
    SQMOD_NODISCARD const SQChar * GetString() const
    {
        return m_Attr.as_string();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a string.
    */
    XmlAttribute & SetString(StackStrF & value)
    {
        m_Attr = value.mPtr;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a integer.
    */
    SQMOD_NODISCARD int32_t GetInt() const
    {
        return m_Attr.as_int();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a integer.
    */
    XmlAttribute & SetInt(int32_t value)
    {
        m_Attr = value;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a unsigned integer.
    */
    SQMOD_NODISCARD uint32_t GetUint() const
    {
        return m_Attr.as_uint();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a unsigned integer.
    */
    XmlAttribute & SetUint(uint32_t value)
    {
        m_Attr = value;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a floating point.
    */
    SQMOD_NODISCARD SQFloat GetFloat() const
    {
        return static_cast< SQFloat >(m_Attr.as_float());
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a floating point.
    */
    XmlAttribute & SetFloat(SQFloat value)
    {
        m_Attr = value;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a double floating point.
    */
    SQMOD_NODISCARD SQFloat GetDouble() const
    {
        return static_cast< SQFloat >(m_Attr.as_double());
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a double floating point.
    */
    XmlAttribute & SetDouble(SQFloat value)
    {
        m_Attr = value;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a long integer.
    */
    SQMOD_NODISCARD SQInteger GetLong() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a long integer.
    */
    XmlAttribute & SetLong(SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a unsigned long integer.
    */
    SQMOD_NODISCARD SQInteger GetUlong() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a unsigned long integer.
    */
    XmlAttribute & SetUlong(SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a boolean.
    */
    SQMOD_NODISCARD bool GetBool() const
    {
        return m_Attr.as_bool();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a boolean.
    */
    XmlAttribute & SetBool(bool value)
    {
        m_Attr = value;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve next attribute in the attribute list of the parent node.
    */
    SQMOD_NODISCARD XmlAttribute NextAttribute() const
    {
        return XmlAttribute(m_Doc, m_Attr.next_attribute());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve previous attribute in the attribute list of the parent node.
    */
    SQMOD_NODISCARD XmlAttribute PrevAttribute() const
    {
        return XmlAttribute(m_Doc, m_Attr.previous_attribute());
    }
};

/* ------------------------------------------------------------------------------------------------
 * A helper for working with text inside PCDATA nodes.
*/
class XmlText
{
    // --------------------------------------------------------------------------------------------
    friend class XmlNode;

protected:

    // --------------------------------------------------------------------------------------------
    typedef xml_text Type;

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    XmlText(DocumentRef doc, const Type & text)
        : m_Doc(std::move(doc)), m_Text(text)
    {
        /* ... */
    }

private:

    // ---------------------------------------------------------------------------------------------
    DocumentRef m_Doc{}; // The main xml document instance.
    Type        m_Text{}; // The managed document node.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    XmlText() = default;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    XmlText(const XmlText & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    XmlText & operator = (const XmlText & o) = default;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    SQMOD_NODISCARD int32_t Cmp(const XmlText & o)
    {
        if (strcmp(m_Text.get(), o.m_Text.get()) == 0)
        {
            return 0;
        }
        else if (strlen(m_Text.get()) > strlen(o.m_Text.get()))
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
    SQMOD_NODISCARD const SQChar * ToString() const
    {
        return m_Text.get();
    }

    /* --------------------------------------------------------------------------------------------
     * See whether this instance references a valid xml document.
    */
    SQMOD_NODISCARD bool IsValid() const
    {
        return m_Doc;
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to this document instance.
    */
    SQMOD_NODISCARD uint32_t GetRefCount() const
    {
        return m_Doc.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the text is empty.
    */
    SQMOD_NODISCARD bool IsEmpty() const
    {
        return m_Text.empty();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the text value.
    */
    SQMOD_NODISCARD const SQChar * GetValue() const
    {
        return m_Text.get();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a string or the specified default value if empty.
    */
    SQMOD_NODISCARD const SQChar * AsString(StackStrF & def) const
    {
        return m_Text.as_string(def.mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a integer or the specified default value if empty.
    */
    SQMOD_NODISCARD int32_t AsInt(int32_t def) const
    {
        return m_Text.as_int(def);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a unsigned integer or the specified default value if empty.
    */
    SQMOD_NODISCARD uint32_t AsUint(uint32_t def) const
    {
        return m_Text.as_uint(def);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a floating point or the specified default value if empty.
    */
    SQMOD_NODISCARD SQFloat AsFloat(SQFloat def) const
    {
        return static_cast< SQFloat >(m_Text.as_float(static_cast< float >(def)));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a double floating point or the specified default value if empty.
    */
    SQMOD_NODISCARD SQFloat AsDouble(SQFloat def) const
    {
        return static_cast< SQFloat >(m_Text.as_double(static_cast< double >(def)));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a long integer or the specified default value if empty.
    */
    SQMOD_NODISCARD SQInteger AsLong(SQInteger def) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a unsigned long integer or the specified default value if empty.
    */
    SQMOD_NODISCARD SQInteger AsUlong(SQInteger def) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a boolean or the specified default value if empty.
    */
    SQMOD_NODISCARD bool AsBool(bool def) const
    {
        return m_Text.as_bool(def);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a string.
    */
    bool ApplyString(StackStrF & value)
    {
        return m_Text.set(value.mPtr);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a integer.
    */
    bool ApplyInt(int32_t value)
    {
        return m_Text.set(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a unsigned integer.
    */
    bool ApplyUint(uint32_t value)
    {
        return m_Text.set(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a floating point.
    */
    bool ApplyFloat(SQFloat value)
    {
        return m_Text.set(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a double floating point.
    */
    bool ApplyDouble(SQFloat value)
    {
        return m_Text.set(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a long integer.
    */
    bool ApplyLong(SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a unsigned long integer.
    */
    bool ApplyUlong(SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a boolean.
    */
    bool ApplyBool(bool value)
    {
        return m_Text.set(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a string.
    */
    SQMOD_NODISCARD const SQChar * GetString() const
    {
        return m_Text.as_string();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a string.
    */
    XmlText & SetString(StackStrF & value)
    {
        m_Text = value.mPtr;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a integer.
    */
    SQMOD_NODISCARD int32_t GetInt() const
    {
        return m_Text.as_int();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a integer.
    */
    XmlText & SetInt(int32_t value)
    {
        m_Text = value;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a unsigned integer.
    */
    SQMOD_NODISCARD uint32_t GetUint() const
    {
        return m_Text.as_uint();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a unsigned integer.
    */
    XmlText & SetUint(uint32_t value)
    {
        m_Text = value;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a floating point.
    */
    SQMOD_NODISCARD SQFloat GetFloat() const
    {
        return static_cast< SQFloat >(m_Text.as_float());
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a floating point.
    */
    XmlText & SetFloat(SQFloat value)
    {
        m_Text = value;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a double floating point.
    */
    SQMOD_NODISCARD SQFloat GetDouble() const
    {
        return static_cast< SQFloat >(m_Text.as_double());
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a double floating point.
    */
    XmlText & SetDouble(SQFloat value)
    {
        m_Text = value;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a long integer.
    */
    SQMOD_NODISCARD SQInteger GetLong() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a long integer.
    */
    XmlText & SetLong(SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a unsigned long integer.
    */
    SQMOD_NODISCARD SQInteger GetUlong() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a unsigned long integer.
    */
    XmlText & SetUlong(SQInteger value);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a boolean.
    */
    SQMOD_NODISCARD bool GetBool() const
    {
        return m_Text.as_bool();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a boolean.
    */
    XmlText & SetBool(bool value)
    {
        m_Text = value;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the data node (node_pcdata or node_cdata) for this object.
    */
    SQMOD_NODISCARD XmlNode GetData() const;
};

} // Namespace:: SqMod
