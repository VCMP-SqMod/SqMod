#pragma once

// ------------------------------------------------------------------------------------------------
#include "Library/XML/Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

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
    XmlNode(const DocumentRef doc, const Type & node)
        : m_Doc(doc), m_Node(node)
    {
        /* ... */
    }

private:

    // ---------------------------------------------------------------------------------------------
    DocumentRef  m_Doc; // The main xml document instance.
    Type         m_Node; // The managed document node.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    XmlNode()
        : m_Doc(), m_Node()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    XmlNode(const XmlNode & o)
        : m_Doc(o.m_Doc), m_Node(o.m_Node)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~XmlNode()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    XmlNode & operator = (const XmlNode & o)
    {
        m_Doc = o.m_Doc;
        m_Node = o.m_Node;
        return *this;
    }

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

} // Namespace:: SqMod
