#ifndef _XML_NODE_HPP_
#define _XML_NODE_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * A light-weight handle for manipulating nodes in DOM tree.
*/
class Node
{
    // --------------------------------------------------------------------------------------------
    friend class Document;
    friend class Text;

protected:

    // --------------------------------------------------------------------------------------------
    typedef xml_node Type;

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    Node(const DocumentRef doc, const Type & node)
        : m_Doc(doc), m_Node(node)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Validate the document reference and throw an error if invalid.
    */
    bool Validate() const;

private:

    // ---------------------------------------------------------------------------------------------
    DocumentRef  m_Doc; /* The main xml document instance. */
    Type         m_Node; /* The managed document node. */

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Node()
        : m_Doc(), m_Node()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    Node(const Node & o)
        : m_Doc(o.m_Doc), m_Node(o.m_Node)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Node()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    Node & operator = (const Node & o)
    {
        m_Doc = o.m_Doc;
        m_Node = o.m_Node;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Node & o)
    {
        if (m_Node == o.m_Node)
            return 0;
        else if (m_Node > o.m_Node)
            return 1;
        else
            return -1;
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
        return (SQInteger)m_Node.hash_value();
    }

    /* --------------------------------------------------------------------------------------------
     * Get node offset in parsed file/string (in char_t units) for debugging purposes.
    */
    SQInteger GetOffsetDebug() const
    {
        return (SQInteger)m_Node.offset_debug();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve node type.
    */
    Int32 GetType() const
    {
        return (Int32)m_Node.type();
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
    void SetName(CSStr name);

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
    void SetValue(CSStr name);

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
    ParseResult AppendBuffer(CSStr source)
    {
        if (source)
            return ParseResult(m_Doc, m_Node.append_buffer(source,
                    std::char_traits< SQChar >::length(source) * sizeof(SQChar)));
        return ParseResult();
    }

    /* --------------------------------------------------------------------------------------------
     * Parses buffer as an XML document fragment and appends all nodes as children of this node.
    */
    ParseResult AppendBuffer(CSStr source, Uint32 options)
    {
        if (source)
            return ParseResult(m_Doc, m_Node.append_buffer(source,
                    std::char_traits< SQChar >::length(source) * sizeof(SQChar), options));
        return ParseResult();
    }

    /* --------------------------------------------------------------------------------------------
     * Parses buffer as an XML document fragment and appends all nodes as children of this node.
    */
    ParseResult AppendBuffer(CSStr source, Uint32 options, Int32 encoding)
    {
        if (source)
            return ParseResult(m_Doc, m_Node.append_buffer(source,
                    std::char_traits< SQChar >::length(source) * sizeof(SQChar)
                    , options, (xml_encoding)encoding));
        return ParseResult();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the first child attribute.
    */
    Attribute GetFirstAttr() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last child attribute.
    */
    Attribute GetLastAttr() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the first child node.
    */
    Node GetFirstChild() const
    {
        return Node(m_Doc, m_Node.first_child());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the last child node.
    */
    Node GetLastChild() const
    {
        return Node(m_Doc, m_Node.last_child());
    }

    /* --------------------------------------------------------------------------------------------
     * Get next sibling in the children list of the parent node.
    */
    Node GetNextSibling() const
    {
        return Node(m_Doc, m_Node.next_sibling());
    }

    /* --------------------------------------------------------------------------------------------
     * Get previous sibling in the children list of the parent node
    */
    Node GetPrevSibling() const
    {
        return Node(m_Doc, m_Node.previous_sibling());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the parent node.
    */
    Node GetParent() const
    {
        return Node(m_Doc, m_Node.parent());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the root node.
    */
    Node GetRoot() const
    {
        return Node(m_Doc, m_Node.root());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the text object for the current node.
    */
    Text GetText() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve child node with the specified name.
    */
    Node Child(CSStr name) const
    {
        return Node(m_Doc, m_Node.child(name));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve child attribute with the specified name.
    */
    Attribute GetAttribute(CSStr name) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve next sibling with the specified name.
    */
    Node NextSibling(CSStr name) const
    {
        return Node(m_Doc, m_Node.next_sibling(name));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve previous sibling with the specified name.
    */
    Node PrevSibling(CSStr name) const
    {
        return Node(m_Doc, m_Node.previous_sibling(name));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve child attribute, starting the search from a hint.
    */
    Attribute AttributeFrom(CSStr name, Attribute & attr) const;

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
    Attribute AppendAttr(CSStr name);

    /* --------------------------------------------------------------------------------------------
     * Prepend a child attribute with the specified name.
    */
    Attribute PrependAttr(CSStr name);

    /* --------------------------------------------------------------------------------------------
     * Insert a child attribute with the specified name, after the specified node.
    */
    Attribute InsertAttrAfter(CSStr name, const Attribute & attr);

    /* --------------------------------------------------------------------------------------------
     * Insert a child attribute with the specified name, before the specified node.
    */
    Attribute InsertAttrBefore(CSStr name, const Attribute & attr);

    /* --------------------------------------------------------------------------------------------
     * Append a copy of the specified attribute as a child.
    */
    Attribute AppendAttrCopy(const Attribute & proto);

    /* --------------------------------------------------------------------------------------------
     * Prepend a copy of the specified attribute as a child.
    */
    Attribute PrependAttrCopy(const Attribute & proto);

    /* --------------------------------------------------------------------------------------------
     * Insert a copy of the specified attribute as a child after the specified attribute.
    */
    Attribute InsertAttrCopyAfter(const Attribute & proto, const Attribute & attr);

    /* --------------------------------------------------------------------------------------------
     * Insert a copy of the specified attribute as a child before the specified attribute.
    */
    Attribute InsertAttrCopyBefore(const Attribute & proto, const Attribute & attr);

    /* --------------------------------------------------------------------------------------------
     * Append a basic child node with the specified name.
    */
    Node AppendChild(CSStr name)
    {
        return Node(m_Doc, m_Node.append_child(name));
    }

    /* --------------------------------------------------------------------------------------------
     * Prepend a basic child node with the specified name.
    */
    Node PrependChild(CSStr name)
    {
        return Node(m_Doc, m_Node.prepend_child(name));
    }

    /* --------------------------------------------------------------------------------------------
     * Append a basic child node.
    */
    Node AppendChildNode()
    {
        return Node(m_Doc, m_Node.append_child());
    }

    /* --------------------------------------------------------------------------------------------
     * Prepend a basic child node.
    */
    Node PrependChildNode()
    {
        return Node(m_Doc, m_Node.prepend_child());
    }

    /* --------------------------------------------------------------------------------------------
     * Append a basic child node with the specified type.
    */
    Node AppendChildType(Int32 type)
    {
        return Node(m_Doc, m_Node.append_child((xml_node_type)type));
    }

    /* --------------------------------------------------------------------------------------------
     * Prepend a basic child node with the specified type.
    */
    Node PrependChildType(Int32 type)
    {
        return Node(m_Doc, m_Node.prepend_child((xml_node_type)type));
    }

    /* --------------------------------------------------------------------------------------------
     * Insert a basic child node with the specified name, after the specified node.
    */
    Node InsertChildAfter(CSStr name, const Node & node)
    {
        return Node(m_Doc, m_Node.insert_child_after(name, node.m_Node));
    }

    /* --------------------------------------------------------------------------------------------
     * Insert a basic child node with the specified name, before the specified node.
    */
    Node InsertChildBefore(CSStr name, const Node & node)
    {
        return Node(m_Doc, m_Node.insert_child_before(name, node.m_Node));
    }

    /* --------------------------------------------------------------------------------------------
     * Insert a basic child node with the specified type, after the specified node.
    */
    Node InsertChildTypeAfter(Int32 type, const Node & node)
    {
        return Node(m_Doc, m_Node.insert_child_after((xml_node_type)type, node.m_Node));
    }

    /* --------------------------------------------------------------------------------------------
     * Insert a basic child node with the specified type, before the specified node.
    */
    Node InsertChildTypeBefore(Int32 type, const Node & node)
    {
        return Node(m_Doc, m_Node.insert_child_before((xml_node_type)type, node.m_Node));
    }

    /* --------------------------------------------------------------------------------------------
     * Append a copy of the specified node as a child.
    */
    Node AppendCopy(const Node & proto)
    {
        return Node(m_Doc, m_Node.append_copy(proto.m_Node));
    }

    /* --------------------------------------------------------------------------------------------
     * Prepend a copy of the specified node as a child.
    */
    Node PrependCopy(const Node & proto)
    {
        return Node(m_Doc, m_Node.prepend_copy(proto.m_Node));
    }

    /* --------------------------------------------------------------------------------------------
     * Insert a copy of the specified node as a child after the specified node.
    */
    Node InsertCopyAfter(const Node & proto, const Node & node)
    {
        return Node(m_Doc, m_Node.insert_copy_after(proto.m_Node, node.m_Node));
    }

    /* --------------------------------------------------------------------------------------------
     * Insert a copy of the specified node as a child before the specified node.
    */
    Node InsertCopyBefore(const Node & proto, const Node & node)
    {
        return Node(m_Doc, m_Node.insert_copy_before(proto.m_Node, node.m_Node));
    }

    /* --------------------------------------------------------------------------------------------
     * Append the specified node as a child and take ownersip of it.
    */
    Node AppendMove(const Node & proto)
    {
        return Node(m_Doc, m_Node.append_copy(proto.m_Node));
    }

    /* --------------------------------------------------------------------------------------------
     * Prepend the specified node as a child and take ownersip of it.
    */
    Node PrependMove(const Node & proto)
    {
        return Node(m_Doc, m_Node.prepend_copy(proto.m_Node));
    }

    /* --------------------------------------------------------------------------------------------
     * Insert the specified node as a child after the specified node and take ownersip of it.
    */
    Node InsertMoveAfter(const Node & proto, const Node & node)
    {
        return Node(m_Doc, m_Node.insert_copy_after(proto.m_Node, node.m_Node));
    }

    /* --------------------------------------------------------------------------------------------
     * Insert the specified node as a child before the specified node and take ownersip of it.
    */
    Node InsertMoveBefore(const Node & proto, const Node & node)
    {
        return Node(m_Doc, m_Node.insert_copy_before(proto.m_Node, node.m_Node));
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
    bool RemoveAttrInst(const Attribute & attr);

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
    bool RemoveChildInst(const Node & node)
    {
        return m_Node.remove_child(node.m_Node);
    }

    /* --------------------------------------------------------------------------------------------
     * Find child node by attribute name/value.
    */
    Node FindChildByAttr(CSStr attr_name, CSStr attr_value) const
    {
        return Node(m_Doc, m_Node.find_child_by_attribute(attr_name, attr_value));
    }

    /* --------------------------------------------------------------------------------------------
     * Find child node by attribute name/value.
    */
    Node FindChildByAttr(CSStr name, CSStr attr_name, CSStr attr_value) const
    {
        return Node(m_Doc, m_Node.find_child_by_attribute(name, attr_name, attr_value));
    }

    /* --------------------------------------------------------------------------------------------
     * Search for a node by path consisting of node names and . or .. elements.
    */
    Node FindElemByPath(CSStr path, SQChar delimiter) const
    {
        return Node(m_Doc, m_Node.first_element_by_path(path, delimiter));
    }
};

} // Namespace:: SqMod

#endif // _XML_NODE_HPP_
