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
    operator bool () const //NOLINT (intentionally implicit)
    {
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance pointer.
    */
    operator Pointer () //NOLINT (intentionally implicit)
    {
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance pointer.
    */
    operator ConstPtr () const //NOLINT (intentionally implicit)
    {
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance reference.
    */
    operator Reference () //NOLINT (intentionally implicit)
    {
        assert(m_Ptr);
        return *m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed instance reference.
    */
    operator ConstRef () const //NOLINT (intentionally implicit)
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
    Int32 Cmp(const XmlDocument & o) const
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
    CSStr ToString() const
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
     * Removes all nodes, leaving the empty document.
    */
    void Reset()
    {
        // Validate the document handle
        m_Doc.Validate();
        // Perform the requested operation
        m_Doc->reset();
    }

    /* --------------------------------------------------------------------------------------------
     * Removes all nodes, then copies the entire contents of the specified document.
    */
    void Reset(const XmlDocument & doc)
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
    XmlParseResult LoadData(CSStr source)
    {
        // Make sure that we are allowed to load in data
        CanLoad();
        // Perform the requested operation and return the result
        return XmlParseResult(m_Doc, m_Doc->load_string(source));
    }

    /* --------------------------------------------------------------------------------------------
     * Load document from zero-terminated string. (LoadString collides with the windows api)
    */
    XmlParseResult LoadData(CSStr source, Uint32 options)
    {
        // Make sure that we are allowed to load in data
        CanLoad();
        // Perform the requested operation and return the result
        return XmlParseResult(m_Doc, m_Doc->load_string(source, options));
    }

    /* --------------------------------------------------------------------------------------------
     * Load document from file on disk.
    */
    XmlParseResult LoadFile(CSStr filepath)
    {
        // Make sure that we are allowed to load in data
        CanLoad();
        // Perform the requested operation and return the result
        return XmlParseResult(m_Doc, m_Doc->load_file(filepath));
    }

    /* --------------------------------------------------------------------------------------------
     * Load document from file on disk.
    */
    XmlParseResult LoadFile(CSStr filepath, Uint32 options)
    {
        // Make sure that we are allowed to load in data
        CanLoad();
        // Perform the requested operation and return the result
        return XmlParseResult(m_Doc, m_Doc->load_file(filepath, options));
    }

    /* --------------------------------------------------------------------------------------------
     * Load document from file on disk.
    */
    XmlParseResult LoadFile(CSStr filepath, Uint32 options, Int32 encoding)
    {
        // Make sure that we are allowed to load in data
        CanLoad();
        // Perform the requested operation and return the result
        return XmlParseResult(m_Doc, m_Doc->load_file(filepath, options,
                                                      static_cast< xml_encoding >(encoding)));
    }

    /* --------------------------------------------------------------------------------------------
     * Save XML to file on disk.
    */
    void SaveFile(CSStr filepath)
    {
        // Validate the document handle
        m_Doc.Validate();
        // Perform the requested operation
        m_Doc->save_file(filepath);
    }

    /* --------------------------------------------------------------------------------------------
     * Save XML to file on disk.
    */
    void SaveFile(CSStr filepath, CSStr indent)
    {
        // Validate the document handle
        m_Doc.Validate();
        // Perform the requested operation
        m_Doc->save_file(filepath, indent);
    }

    /* --------------------------------------------------------------------------------------------
     * Save XML to file on disk.
    */
    void SaveFile(CSStr filepath, CSStr indent, Uint32 format)
    {
        // Validate the document handle
        m_Doc.Validate();
        // Perform the requested operation
        m_Doc->save_file(filepath, indent, format);
    }

    /* --------------------------------------------------------------------------------------------
     * Save XML to file on disk.
    */
    void SaveFile(CSStr filepath, CSStr indent, Uint32 format, Int32 encoding)
    {
        // Validate the document handle
        m_Doc.Validate();
        // Perform the requested operation
        m_Doc->save_file(filepath, indent, format, static_cast< xml_encoding >(encoding));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the document root node.
    */
    XmlNode GetNode() const;
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
    Int32 Cmp(const XmlNode & o)
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
    CSStr ToString() const
    {
        return m_Node.value();
    }

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
     * See whether the node is empty.
    */
    bool IsEmpty() const
    {
        return m_Node.empty();
    }

    /* --------------------------------------------------------------------------------------------
     * Get hash value (unique for handles to the same object).
    */
    SQInteger GetHashValue() const
    {
        return static_cast< SQInteger >(m_Node.hash_value());
    }

    /* --------------------------------------------------------------------------------------------
     * Get node offset in parsed file/string (in char_t units) for debugging purposes.
    */
    SQInteger GetOffsetDebug() const
    {
        return static_cast< SQInteger >(m_Node.offset_debug());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve node type.
    */
    Int32 GetType() const
    {
        return static_cast< Int32 >(m_Node.type());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve node name.
    */
    CSStr GetName() const
    {
        return m_Node.name();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve node name.
    */
    void SetName(CSStr name)
    {
        if (!m_Node.set_name(name))
        {
            STHROWF("Unable to set XML node name");
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the node name.
    */
    bool ApplyName(CSStr name)
    {
        return m_Node.set_name(name);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve node value.
    */
    CSStr GetValue() const
    {
        return m_Node.value();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve node value.
    */
    void SetValue(CSStr name)
    {
        if (!m_Node.set_value(name))
        {
            STHROWF("Unable to set XML node value");
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the node value.
    */
    bool ApplyValue(CSStr value)
    {
        return m_Node.set_value(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Parses buffer as an XML document fragment and appends all nodes as children of this node.
    */
    XmlParseResult AppendBuffer(CSStr source)
    {
        // Is the specified source buffer even valid?
        if (source)
        {
            return XmlParseResult(m_Doc, m_Node.append_buffer(source,
                    std::char_traits< SQChar >::length(source) * sizeof(SQChar)));
        }
        // Return the default result
        return XmlParseResult();
    }

    /* --------------------------------------------------------------------------------------------
     * Parses buffer as an XML document fragment and appends all nodes as children of this node.
    */
    XmlParseResult AppendBuffer(CSStr source, Uint32 options)
    {
        // Is the specified source buffer even valid?
        if (source)
        {
            return XmlParseResult(m_Doc, m_Node.append_buffer(source,
                    std::char_traits< SQChar >::length(source) * sizeof(SQChar), options));
        }
        // Return the default result
        return XmlParseResult();
    }

    /* --------------------------------------------------------------------------------------------
     * Parses buffer as an XML document fragment and appends all nodes as children of this node.
    */
    XmlParseResult AppendBuffer(CSStr source, Uint32 options, Int32 encoding)
    {
        // Is the specified source buffer even valid?
        if (source)
        {
            return XmlParseResult(m_Doc, m_Node.append_buffer(source,
                    std::char_traits< SQChar >::length(source) * sizeof(SQChar),
                                                              options, (xml_encoding)encoding));
        }
        // Return the default result
        return XmlParseResult();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the first child attribute.
    */
    XmlAttribute GetFirstAttr() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last child attribute.
    */
    XmlAttribute GetLastAttr() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the first child node.
    */
    XmlNode GetFirstChild() const
    {
        return XmlNode(m_Doc, m_Node.first_child());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last child node.
    */
    XmlNode GetLastChild() const
    {
        return XmlNode(m_Doc, m_Node.last_child());
    }

    /* --------------------------------------------------------------------------------------------
     * Get next sibling in the children list of the parent node.
    */
    XmlNode GetNextSibling() const
    {
        return XmlNode(m_Doc, m_Node.next_sibling());
    }

    /* --------------------------------------------------------------------------------------------
     * Get previous sibling in the children list of the parent node
    */
    XmlNode GetPrevSibling() const
    {
        return XmlNode(m_Doc, m_Node.previous_sibling());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the parent node.
    */
    XmlNode GetParent() const
    {
        return XmlNode(m_Doc, m_Node.parent());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the root node.
    */
    XmlNode GetRoot() const
    {
        return XmlNode(m_Doc, m_Node.root());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the text object for the current node.
    */
    XmlText GetText() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve child node with the specified name.
    */
    XmlNode Child(CSStr name) const
    {
        return XmlNode(m_Doc, m_Node.child(name));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve child attribute with the specified name.
    */
    XmlAttribute GetAttribute(CSStr name) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve next sibling with the specified name.
    */
    XmlNode NextSibling(CSStr name) const
    {
        return XmlNode(m_Doc, m_Node.next_sibling(name));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve previous sibling with the specified name.
    */
    XmlNode PrevSibling(CSStr name) const
    {
        return XmlNode(m_Doc, m_Node.previous_sibling(name));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve child attribute, starting the search from a hint.
    */
    XmlAttribute AttributeFrom(CSStr name, XmlAttribute & attr) const;

    /* --------------------------------------------------------------------------------------------
     * Get child value of current node; that is, value of the first child node of type PCDATA/CDATA.
    */
    CSStr GetChildValue() const
    {
        return m_Node.child_value();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve child value of child with specified name.
    */
    CSStr ChildValue(CSStr name) const
    {
        return m_Node.child_value(name);
    }

    /* --------------------------------------------------------------------------------------------
     * Append a child attribute with the specified name.
    */
    XmlAttribute AppendAttr(CSStr name);

    /* --------------------------------------------------------------------------------------------
     * Prepend a child attribute with the specified name.
    */
    XmlAttribute PrependAttr(CSStr name);

    /* --------------------------------------------------------------------------------------------
     * Insert a child attribute with the specified name, after the specified node.
    */
    XmlAttribute InsertAttrAfter(CSStr name, const XmlAttribute & attr);

    /* --------------------------------------------------------------------------------------------
     * Insert a child attribute with the specified name, before the specified node.
    */
    XmlAttribute InsertAttrBefore(CSStr name, const XmlAttribute & attr);

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
    XmlNode AppendChild(CSStr name)
    {
        return XmlNode(m_Doc, m_Node.append_child(name));
    }

    /* --------------------------------------------------------------------------------------------
     * Prepend a basic child node with the specified name.
    */
    XmlNode PrependChild(CSStr name)
    {
        return XmlNode(m_Doc, m_Node.prepend_child(name));
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
    XmlNode AppendChildType(Int32 type)
    {
        return XmlNode(m_Doc, m_Node.append_child(static_cast< xml_node_type >(type)));
    }

    /* --------------------------------------------------------------------------------------------
     * Prepend a basic child node with the specified type.
    */
    XmlNode PrependChildType(Int32 type)
    {
        return XmlNode(m_Doc, m_Node.prepend_child(static_cast< xml_node_type >(type)));
    }

    /* --------------------------------------------------------------------------------------------
     * Insert a basic child node with the specified name, after the specified node.
    */
    XmlNode InsertChildAfter(CSStr name, const XmlNode & node)
    {
        return XmlNode(m_Doc, m_Node.insert_child_after(name, node.m_Node));
    }

    /* --------------------------------------------------------------------------------------------
     * Insert a basic child node with the specified name, before the specified node.
    */
    XmlNode InsertChildBefore(CSStr name, const XmlNode & node)
    {
        return XmlNode(m_Doc, m_Node.insert_child_before(name, node.m_Node));
    }

    /* --------------------------------------------------------------------------------------------
     * Insert a basic child node with the specified type, after the specified node.
    */
    XmlNode InsertChildTypeAfter(Int32 type, const XmlNode & node)
    {
        return XmlNode(m_Doc, m_Node.insert_child_after(static_cast< xml_node_type >(type),
                                                        node.m_Node));
    }

    /* --------------------------------------------------------------------------------------------
     * Insert a basic child node with the specified type, before the specified node.
    */
    XmlNode InsertChildTypeBefore(Int32 type, const XmlNode & node)
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
    bool RemoveAttr(CSStr name)
    {
        return m_Node.remove_attribute(name);
    }

    /* --------------------------------------------------------------------------------------------
     * Remove the specified attribute.
    */
    bool RemoveAttrInst(const XmlAttribute & attr);

    /* --------------------------------------------------------------------------------------------
     * Remove the child node matching the specified name.
    */
    bool RemoveChild(CSStr name)
    {
        return m_Node.remove_child(name);
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
    XmlNode FindChildByAttr(CSStr attr_name, CSStr attr_value) const
    {
        return XmlNode(m_Doc, m_Node.find_child_by_attribute(attr_name, attr_value));
    }

    /* --------------------------------------------------------------------------------------------
     * Find child node by attribute name/value.
    */
    XmlNode FindChildByAttr(CSStr name, CSStr attr_name, CSStr attr_value) const
    {
        return XmlNode(m_Doc, m_Node.find_child_by_attribute(name, attr_name, attr_value));
    }

    /* --------------------------------------------------------------------------------------------
     * Search for a node by path consisting of node names and . or .. elements.
    */
    XmlNode FindElemByPath(CSStr path, SQChar delimiter) const
    {
        return XmlNode(m_Doc, m_Node.first_element_by_path(path, delimiter));
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
    Int32 Cmp(const XmlAttribute & o)
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
    CSStr ToString() const
    {
        return m_Attr.value();
    }

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
     * See whether the attribute is empty.
    */
    bool IsEmpty() const
    {
        return m_Attr.empty();
    }

    /* --------------------------------------------------------------------------------------------
     * Get hash value (unique for handles to the same object).
    */
    SQInteger GetHashValue() const
    {
        return (SQInteger)m_Attr.hash_value();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve attribute name.
    */
    CSStr GetName() const
    {
        return m_Attr.name();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve attribute name.
    */
    void SetName(CSStr name)
    {
        if (!m_Attr.set_name(name))
        {
            STHROWF("Unable to set XML attribute name");
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the attribute name.
    */
    bool ApplyName(CSStr name)
    {
        return m_Attr.set_name(name);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve attribute value.
    */
    CSStr GetValue() const
    {
        return m_Attr.value();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve attribute value.
    */
    void SetValue(CSStr name)
    {
        if (!m_Attr.set_value(name))
        {
            STHROWF("Unable to set XML attribute value");
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the attribute value.
    */
    bool ApplyValue(CSStr value)
    {
        return m_Attr.set_value(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a string or the specified default value if empty.
    */
    CSStr AsString(CSStr def) const
    {
        return m_Attr.as_string(def);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a integer or the specified default value if empty.
    */
    Int32 AsInt(Int32 def) const
    {
        return m_Attr.as_int(def);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a unsigned integer or the specified default value if empty.
    */
    Uint32 AsUint(Uint32 def) const
    {
        return m_Attr.as_uint(def);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a floating point or the specified default value if empty.
    */
    SQFloat AsFloat(SQFloat def) const
    {
        return (SQFloat)m_Attr.as_float(def);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a double floating point or the specified default value if empty.
    */
    SQFloat AsDouble(SQFloat def) const
    {
        return (SQFloat)m_Attr.as_double(def);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a long integer or the specified default value if empty.
    */
    LightObj AsLong(const SLongInt & def) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a unsigned long integer or the specified default value if empty.
    */
    LightObj AsUlong(const ULongInt & def) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a boolean or the specified default value if empty.
    */
    bool AsBool(bool def) const
    {
        return m_Attr.as_bool(def);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a string.
    */
    bool ApplyString(CSStr value)
    {
        return m_Attr.set_value(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a integer.
    */
    bool ApplyInt(Int32 value)
    {
        return m_Attr.set_value(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a unsigned integer.
    */
    bool ApplyUint(Uint32 value)
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
    bool ApplyLong(const SLongInt & value);

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a unsigned long integer.
    */
    bool ApplyUlong(const ULongInt & value);

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
    CSStr GetString() const
    {
        return m_Attr.as_string();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a string.
    */
    void SetString(CSStr value)
    {
        m_Attr = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a integer.
    */
    Int32 GetInt() const
    {
        return m_Attr.as_int();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a integer.
    */
    void SetInt(Int32 value)
    {
        m_Attr = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a unsigned integer.
    */
    Uint32 GetUint() const
    {
        return m_Attr.as_uint();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a unsigned integer.
    */
    void SetUint(Uint32 value)
    {
        m_Attr = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a floating point.
    */
    SQFloat GetFloat() const
    {
        return static_cast< SQFloat >(m_Attr.as_float());
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a floating point.
    */
    void SetFloat(SQFloat value)
    {
        m_Attr = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a double floating point.
    */
    SQFloat GetDouble() const
    {
        return static_cast< SQFloat >(m_Attr.as_double());
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a double floating point.
    */
    void SetDouble(SQFloat value)
    {
        m_Attr = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a long integer.
    */
    LightObj GetLong() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a long integer.
    */
    void SetLong(const SLongInt & value);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a unsigned long integer.
    */
    LightObj GetUlong() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a unsigned long integer.
    */
    void SetUlong(const ULongInt & value);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a boolean.
    */
    bool GetBool() const
    {
        return m_Attr.as_bool();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a boolean.
    */
    void SetBool(bool value)
    {
        m_Attr = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve next attribute in the attribute list of the parent node.
    */
    XmlAttribute NextAttribute() const
    {
        return XmlAttribute(m_Doc, m_Attr.next_attribute());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve previous attribute in the attribute list of the parent node.
    */
    XmlAttribute PrevAttribute() const
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
    Int32 Cmp(const XmlText & o)
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
    CSStr ToString() const
    {
        return m_Text.get();
    }

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
     * See whether the text is empty.
    */
    bool IsEmpty() const
    {
        return m_Text.empty();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the text value.
    */
    CSStr GetValue() const
    {
        return m_Text.get();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a string or the specified default value if empty.
    */
    CSStr AsString(CSStr def) const
    {
        return m_Text.as_string(def);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a integer or the specified default value if empty.
    */
    Int32 AsInt(Int32 def) const
    {
        return m_Text.as_int(def);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a unsigned integer or the specified default value if empty.
    */
    Uint32 AsUint(Uint32 def) const
    {
        return m_Text.as_uint(def);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a floating point or the specified default value if empty.
    */
    SQFloat AsFloat(SQFloat def) const
    {
        return static_cast< SQFloat >(m_Text.as_float(static_cast< Float32 >(def)));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a double floating point or the specified default value if empty.
    */
    SQFloat AsDouble(SQFloat def) const
    {
        return static_cast< SQFloat >(m_Text.as_double(static_cast< Float64 >(def)));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a long integer or the specified default value if empty.
    */
    LightObj AsLong(const SLongInt & def) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a unsigned long integer or the specified default value if empty.
    */
    LightObj AsUlong(const ULongInt & def) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a boolean or the specified default value if empty.
    */
    bool AsBool(bool def) const
    {
        return m_Text.as_bool(def);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a string.
    */
    bool ApplyString(CSStr value)
    {
        return m_Text.set(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a integer.
    */
    bool ApplyInt(Int32 value)
    {
        return m_Text.set(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a unsigned integer.
    */
    bool ApplyUint(Uint32 value)
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
    bool ApplyLong(const SLongInt & value);

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a unsigned long integer.
    */
    bool ApplyUlong(const ULongInt & value);

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
    CSStr GetString() const
    {
        return m_Text.as_string();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a string.
    */
    void SetString(CSStr value)
    {
        m_Text = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a integer.
    */
    Int32 GetInt() const
    {
        return m_Text.as_int();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a integer.
    */
    void SetInt(Int32 value)
    {
        m_Text = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a unsigned integer.
    */
    Uint32 GetUint() const
    {
        return m_Text.as_uint();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a unsigned integer.
    */
    void SetUint(Uint32 value)
    {
        m_Text = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a floating point.
    */
    SQFloat GetFloat() const
    {
        return static_cast< SQFloat >(m_Text.as_float());
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a floating point.
    */
    void SetFloat(SQFloat value)
    {
        m_Text = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a double floating point.
    */
    SQFloat GetDouble() const
    {
        return static_cast< SQFloat >(m_Text.as_double());
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a double floating point.
    */
    void SetDouble(SQFloat value)
    {
        m_Text = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a long integer.
    */
    LightObj GetLong() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a long integer.
    */
    void SetLong(const SLongInt & value);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a unsigned long integer.
    */
    LightObj GetUlong() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a unsigned long integer.
    */
    void SetUlong(const ULongInt & value);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a boolean.
    */
    bool GetBool() const
    {
        return m_Text.as_bool();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a boolean.
    */
    void SetBool(bool value)
    {
        m_Text = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the data node (node_pcdata or node_cdata) for this object.
    */
    XmlNode GetData() const;
};

} // Namespace:: SqMod
