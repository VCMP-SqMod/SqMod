// ------------------------------------------------------------------------------------------------
#include "Node.hpp"
#include "Attribute.hpp"
#include "Text.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger Node::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqXmlNode");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
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
