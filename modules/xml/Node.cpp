// ------------------------------------------------------------------------------------------------
#include "Node.hpp"
#include "Attribute.hpp"
#include "Text.hpp"
#include "Module.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
bool Node::Validate() const
{
    if (m_Doc)
        return true;
    // Invalid document reference
    _SqMod->SqThrow("Invalid XML document reference");
    return false;
}

// ------------------------------------------------------------------------------------------------
void Node::SetName(CSStr name)
{
    if (!m_Node.set_name(name))
        _SqMod->SqThrow("Unable to set XML node name");
}

// ------------------------------------------------------------------------------------------------
void Node::SetValue(CSStr name)
{
    if (!m_Node.set_value(name))
        _SqMod->SqThrow("Unable to set XML node value");
}

// ------------------------------------------------------------------------------------------------
Attribute Node::GetFirstAttr() const
{
    return Attribute(m_Doc, m_Node.first_attribute());
}

// ------------------------------------------------------------------------------------------------
Attribute Node::GetLastAttr() const
{
    return Attribute(m_Doc, m_Node.last_attribute());
}

// ------------------------------------------------------------------------------------------------
Text Node::GetText() const
{
    return Text(m_Doc, m_Node.text());
}

// ------------------------------------------------------------------------------------------------
Attribute Node::GetAttribute(CSStr name) const
{
    return Attribute(m_Doc, m_Node.attribute(name));
}

// ------------------------------------------------------------------------------------------------
Attribute Node::AttributeFrom(CSStr name, Attribute & attr) const
{
    return Attribute(m_Doc, m_Node.attribute(name, attr.m_Attr));
}

// ------------------------------------------------------------------------------------------------
Attribute Node::AppendAttr(CSStr name)
{
    return Attribute(m_Doc, m_Node.append_attribute(name));
}

// ------------------------------------------------------------------------------------------------
Attribute Node::PrependAttr(CSStr name)
{
    return Attribute(m_Doc, m_Node.prepend_attribute(name));
}

// ------------------------------------------------------------------------------------------------
Attribute Node::InsertAttrAfter(CSStr name, const Attribute & attr)
{
    return Attribute(m_Doc, m_Node.insert_attribute_after(name, attr.m_Attr));
}

// ------------------------------------------------------------------------------------------------
Attribute Node::InsertAttrBefore(CSStr name, const Attribute & attr)
{
    return Attribute(m_Doc, m_Node.insert_attribute_before(name, attr.m_Attr));
}

// ------------------------------------------------------------------------------------------------
Attribute Node::AppendAttrCopy(const Attribute & proto)
{
    return Attribute(m_Doc, m_Node.append_copy(proto.m_Attr));
}

// ------------------------------------------------------------------------------------------------
Attribute Node::PrependAttrCopy(const Attribute & proto)
{
    return Attribute(m_Doc, m_Node.prepend_copy(proto.m_Attr));
}

// ------------------------------------------------------------------------------------------------
Attribute Node::InsertAttrCopyAfter(const Attribute & proto, const Attribute & attr)
{
    return Attribute(m_Doc, m_Node.insert_copy_after(proto.m_Attr, attr.m_Attr));
}

// ------------------------------------------------------------------------------------------------
Attribute Node::InsertAttrCopyBefore(const Attribute & proto, const Attribute & attr)
{
    return Attribute(m_Doc, m_Node.insert_copy_before(proto.m_Attr, attr.m_Attr));
}

// ------------------------------------------------------------------------------------------------
bool Node::RemoveAttrInst(const Attribute & attr)
{
    return m_Node.remove_attribute(attr.m_Attr);
}

} // Namespace:: SqMod