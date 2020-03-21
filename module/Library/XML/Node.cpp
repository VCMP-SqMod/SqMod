// ------------------------------------------------------------------------------------------------
#include "Library/XML/Node.hpp"
#include "Library/XML/Attribute.hpp"
#include "Library/XML/Text.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger XmlNode::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqXmlNode");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
XmlAttribute XmlNode::GetFirstAttr() const
{
    return XmlAttribute(m_Doc, m_Node.first_attribute());
}

// ------------------------------------------------------------------------------------------------
XmlAttribute XmlNode::GetLastAttr() const
{
    return XmlAttribute(m_Doc, m_Node.last_attribute());
}

// ------------------------------------------------------------------------------------------------
XmlText XmlNode::GetText() const
{
    return XmlText(m_Doc, m_Node.text());
}

// ------------------------------------------------------------------------------------------------
XmlAttribute XmlNode::GetAttribute(CSStr name) const
{
    return XmlAttribute(m_Doc, m_Node.attribute(name));
}

// ------------------------------------------------------------------------------------------------
XmlAttribute XmlNode::AttributeFrom(CSStr name, XmlAttribute & attr) const
{
    return XmlAttribute(m_Doc, m_Node.attribute(name, attr.m_Attr));
}

// ------------------------------------------------------------------------------------------------
XmlAttribute XmlNode::AppendAttr(CSStr name)
{
    return XmlAttribute(m_Doc, m_Node.append_attribute(name));
}

// ------------------------------------------------------------------------------------------------
XmlAttribute XmlNode::PrependAttr(CSStr name)
{
    return XmlAttribute(m_Doc, m_Node.prepend_attribute(name));
}

// ------------------------------------------------------------------------------------------------
XmlAttribute XmlNode::InsertAttrAfter(CSStr name, const XmlAttribute & attr)
{
    return XmlAttribute(m_Doc, m_Node.insert_attribute_after(name, attr.m_Attr));
}

// ------------------------------------------------------------------------------------------------
XmlAttribute XmlNode::InsertAttrBefore(CSStr name, const XmlAttribute & attr)
{
    return XmlAttribute(m_Doc, m_Node.insert_attribute_before(name, attr.m_Attr));
}

// ------------------------------------------------------------------------------------------------
XmlAttribute XmlNode::AppendAttrCopy(const XmlAttribute & proto)
{
    return XmlAttribute(m_Doc, m_Node.append_copy(proto.m_Attr));
}

// ------------------------------------------------------------------------------------------------
XmlAttribute XmlNode::PrependAttrCopy(const XmlAttribute & proto)
{
    return XmlAttribute(m_Doc, m_Node.prepend_copy(proto.m_Attr));
}

// ------------------------------------------------------------------------------------------------
XmlAttribute XmlNode::InsertAttrCopyAfter(const XmlAttribute & proto, const XmlAttribute & attr)
{
    return XmlAttribute(m_Doc, m_Node.insert_copy_after(proto.m_Attr, attr.m_Attr));
}

// ------------------------------------------------------------------------------------------------
XmlAttribute XmlNode::InsertAttrCopyBefore(const XmlAttribute & proto, const XmlAttribute & attr)
{
    return XmlAttribute(m_Doc, m_Node.insert_copy_before(proto.m_Attr, attr.m_Attr));
}

// ------------------------------------------------------------------------------------------------
bool XmlNode::RemoveAttrInst(const XmlAttribute & attr)
{
    return m_Node.remove_attribute(attr.m_Attr);
}

} // Namespace:: SqMod
