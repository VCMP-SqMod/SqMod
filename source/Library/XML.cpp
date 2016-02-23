// ------------------------------------------------------------------------------------------------
#include "Library/XML.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {
namespace XML {

// ------------------------------------------------------------------------------------------------
void XmlParseResult::Check() const
{
    if (m_Result.status != status_ok)
        SqThrow("Xml parse error [%s]", m_Result.description());
}

// ------------------------------------------------------------------------------------------------
XmlNode XmlText::GetData() const
{
    return XmlNode(m_Doc, m_Text.data());
}

// ------------------------------------------------------------------------------------------------
XmlNode XmlDocument::GetNode() const
{
    if (m_Doc)
        return XmlNode(m_Doc, m_Doc->document_element());
    return XmlNode();
}

} // Namespace:: XML

// ================================================================================================
void Register_XML(HSQUIRRELVM vm)
{
    using namespace XML;

    Table xmlns(vm);

    xmlns.Bind(_SC("ParseResult"), Class< XmlParseResult >(vm, _SC("SqXmlParseResult"))
        /* Constructors */
        .Ctor()
        .Ctor< const XmlParseResult & >()
        /* Core Metamethods */
        .Func(_SC("_cmp"), &XmlParseResult::Cmp)
        .Func(_SC("_tostring"), &XmlParseResult::ToString)
        /* Properties */
        .Prop(_SC("Valid"), &XmlParseResult::IsValid)
        .Prop(_SC("References"), &XmlParseResult::GetRefCount)
        .Prop(_SC("Ok"), &XmlParseResult::IsOk)
        .Prop(_SC("Status"), &XmlParseResult::GetStatus)
        .Prop(_SC("Offset"), &XmlParseResult::GetOffset)
        .Prop(_SC("Encoding"), &XmlParseResult::GetEncoding)
        .Prop(_SC("Description"), &XmlParseResult::GetDescription)
        /* Functions */
        .Func(_SC("Check"), &XmlParseResult::Check)
    );

    xmlns.Bind(_SC("Attribute"), Class< XmlAttribute >(vm, _SC("SqXmlAttribute"))
        /* Constructors */
        .Ctor()
        .Ctor< const XmlAttribute & >()
        /* Core Metamethods */
        .Func(_SC("_cmp"), &XmlAttribute::Cmp)
        .Func(_SC("_tostring"), &XmlAttribute::ToString)
        /* Properties */
        .Prop(_SC("Valid"), &XmlAttribute::IsValid)
        .Prop(_SC("References"), &XmlAttribute::GetRefCount)
        .Prop(_SC("Empty"), &XmlAttribute::IsEmpty)
        .Prop(_SC("Hash"), &XmlAttribute::GetHashValue)
        .Prop(_SC("Name"), &XmlAttribute::GetName, &XmlAttribute::SetName)
        .Prop(_SC("Value"), &XmlAttribute::GetValue, &XmlAttribute::SetValue)
        .Prop(_SC("Int"), &XmlAttribute::GetInt, &XmlAttribute::SetInt)
        .Prop(_SC("Uint"), &XmlAttribute::GetUint, &XmlAttribute::SetUint)
        .Prop(_SC("Float"), &XmlAttribute::GetFloat, &XmlAttribute::SetFloat)
        .Prop(_SC("Double"), &XmlAttribute::GetDouble, &XmlAttribute::SetDouble)
        .Prop(_SC("Long"), &XmlAttribute::GetLong, &XmlAttribute::SetLong)
        .Prop(_SC("Ulong"), &XmlAttribute::GetUlong, &XmlAttribute::SetUlong)
        .Prop(_SC("Bool"), &XmlAttribute::GetBool, &XmlAttribute::SetBool)
        .Prop(_SC("Next"), &XmlAttribute::NextAttribute)
        .Prop(_SC("Prev"), &XmlAttribute::PrevAttribute)
        /* Functions */
        .Func(_SC("SetName"), &XmlAttribute::ApplyName)
        .Func(_SC("SetValue"), &XmlAttribute::ApplyValue)
        .Func(_SC("AsString"), &XmlAttribute::AsString)
        .Func(_SC("AsInt"), &XmlAttribute::AsInt)
        .Func(_SC("AsUint"), &XmlAttribute::AsUint)
        .Func(_SC("AsFloat"), &XmlAttribute::AsFloat)
        .Func(_SC("AsDouble"), &XmlAttribute::AsDouble)
        .Func(_SC("AsLong"), &XmlAttribute::AsLong)
        .Func(_SC("AsUlong"), &XmlAttribute::AsUlong)
        .Func(_SC("AsBool"), &XmlAttribute::AsBool)
        .Func(_SC("SetString"), &XmlAttribute::ApplyString)
        .Func(_SC("SetInt"), &XmlAttribute::ApplyInt)
        .Func(_SC("SetUint"), &XmlAttribute::ApplyUint)
        .Func(_SC("SetFloat"), &XmlAttribute::ApplyFloat)
        .Func(_SC("SetDouble"), &XmlAttribute::ApplyDouble)
        .Func(_SC("SetLong"), &XmlAttribute::ApplyLong)
        .Func(_SC("SetUlong"), &XmlAttribute::ApplyUlong)
        .Func(_SC("SetBool"), &XmlAttribute::ApplyBool)
    );

    xmlns.Bind(_SC("Text"), Class< XmlText >(vm, _SC("SqXmlText"))
        /* Constructors */
        .Ctor()
        .Ctor< const XmlText & >()
        /* Core Metamethods */
        .Func(_SC("_cmp"), &XmlText::Cmp)
        .Func(_SC("_tostring"), &XmlText::ToString)
        /* Properties */
        .Prop(_SC("Valid"), &XmlText::IsValid)
        .Prop(_SC("References"), &XmlText::GetRefCount)
        .Prop(_SC("Empty"), &XmlText::IsEmpty)
        .Prop(_SC("Value"), &XmlText::GetValue)
        .Prop(_SC("Int"), &XmlText::GetInt, &XmlText::SetInt)
        .Prop(_SC("Uint"), &XmlText::GetUint, &XmlText::SetUint)
        .Prop(_SC("Float"), &XmlText::GetFloat, &XmlText::SetFloat)
        .Prop(_SC("Double"), &XmlText::GetDouble, &XmlText::SetDouble)
        .Prop(_SC("Long"), &XmlText::GetLong, &XmlText::SetLong)
        .Prop(_SC("Ulong"), &XmlText::GetUlong, &XmlText::SetUlong)
        .Prop(_SC("Bool"), &XmlText::GetBool, &XmlText::SetBool)
        .Prop(_SC("Data"), &XmlText::GetData)
        /* Functions */
        .Func(_SC("AsString"), &XmlText::AsString)
        .Func(_SC("AsInt"), &XmlText::AsInt)
        .Func(_SC("AsUint"), &XmlText::AsUint)
        .Func(_SC("AsFloat"), &XmlText::AsFloat)
        .Func(_SC("AsDouble"), &XmlText::AsDouble)
        .Func(_SC("AsLong"), &XmlText::AsLong)
        .Func(_SC("AsUlong"), &XmlText::AsUlong)
        .Func(_SC("AsBool"), &XmlText::AsBool)
        .Func(_SC("SetString"), &XmlText::ApplyString)
        .Func(_SC("SetInt"), &XmlText::ApplyInt)
        .Func(_SC("SetUint"), &XmlText::ApplyUint)
        .Func(_SC("SetFloat"), &XmlText::ApplyFloat)
        .Func(_SC("SetDouble"), &XmlText::ApplyDouble)
        .Func(_SC("SetLong"), &XmlText::ApplyLong)
        .Func(_SC("SetUlong"), &XmlText::ApplyUlong)
        .Func(_SC("SetBool"), &XmlText::ApplyBool)
    );

    xmlns.Bind(_SC("Node"), Class< XmlNode >(vm, _SC("SqXmlNode"))
        /* Constructors */
        .Ctor()
        .Ctor< const XmlNode & >()
        /* Core Metamethods */
        .Func(_SC("_cmp"), &XmlNode::Cmp)
        .Func(_SC("_tostring"), &XmlNode::ToString)
        /* Properties */
        .Prop(_SC("Valid"), &XmlNode::IsValid)
        .Prop(_SC("References"), &XmlNode::GetRefCount)
        .Prop(_SC("Empty"), &XmlNode::IsEmpty)
        .Prop(_SC("Hash"), &XmlNode::GetHashValue)
        .Prop(_SC("OffsetDebug"), &XmlNode::GetOffsetDebug)
        .Prop(_SC("Type"), &XmlNode::GetType)
        .Prop(_SC("Name"), &XmlNode::GetName, &XmlNode::SetName)
        .Prop(_SC("Value"), &XmlNode::GetValue, &XmlNode::SetValue)
        .Prop(_SC("FirstAttr"), &XmlNode::GetFirstAttr)
        .Prop(_SC("LastAttr"), &XmlNode::GetLastAttr)
        .Prop(_SC("FirstChild"), &XmlNode::GetFirstChild)
        .Prop(_SC("LastChild"), &XmlNode::GetLastChild)
        .Prop(_SC("NextSibling"), &XmlNode::GetNextSibling)
        .Prop(_SC("PrevSibling"), &XmlNode::GetPrevSibling)
        .Prop(_SC("Parent"), &XmlNode::GetParent)
        .Prop(_SC("Root"), &XmlNode::GetRoot)
        .Prop(_SC("Text"), &XmlNode::GetText)
        .Prop(_SC("ChildValue"), &XmlNode::GetChildValue)
        /* Functions */
        .Overload< XmlParseResult (XmlNode::*)(CSStr) >(_SC("AppendBuffer"), &XmlNode::AppendBuffer)
        .Overload< XmlParseResult (XmlNode::*)(CSStr, Uint32) >(_SC("AppendBuffer"), &XmlNode::AppendBuffer)
        .Overload< XmlParseResult (XmlNode::*)(CSStr, Uint32, Int32) >(_SC("AppendBuffer"), &XmlNode::AppendBuffer)
        .Func(_SC("SetName"), &XmlNode::ApplyName)
        .Func(_SC("SetValue"), &XmlNode::ApplyValue)
        .Func(_SC("GetChild"), &XmlNode::Child)
        .Func(_SC("GetAttr"), &XmlNode::Attribute)
        .Func(_SC("GetAttribute"), &XmlNode::Attribute)
        .Func(_SC("GetAttrFrom"), &XmlNode::AttributeFrom)
        .Func(_SC("GetAttributeFrom"), &XmlNode::AttributeFrom)
        .Func(_SC("GetNextSibling"), &XmlNode::NextSibling)
        .Func(_SC("GetPrevSibling"), &XmlNode::PrevSibling)
        .Func(_SC("GetChildValue"), &XmlNode::ChildValue)
        .Func(_SC("AppendAttr"), &XmlNode::AppendAttr)
        .Func(_SC("PrependAttr"), &XmlNode::PrependAttr)
        .Func(_SC("InsertAttrAfter"), &XmlNode::InsertAttrAfter)
        .Func(_SC("InsertAttrBefore"), &XmlNode::InsertAttrBefore)
        .Func(_SC("AppendAttrCopy"), &XmlNode::AppendAttrCopy)
        .Func(_SC("PrependAttrCopy"), &XmlNode::PrependAttrCopy)
        .Func(_SC("InsertAttrCopyAfter"), &XmlNode::InsertAttrCopyAfter)
        .Func(_SC("InsertAttrCopyBefore"), &XmlNode::InsertAttrCopyBefore)
        .Func(_SC("AppendChild"), &XmlNode::AppendChild)
        .Func(_SC("PrependChild"), &XmlNode::PrependChild)
        .Func(_SC("AppendChildNode"), &XmlNode::AppendChildNode)
        .Func(_SC("PrependChildNode"), &XmlNode::PrependChildNode)
        .Func(_SC("AppendChildType"), &XmlNode::AppendChildType)
        .Func(_SC("PrependChildType"), &XmlNode::PrependChildType)
        .Func(_SC("InsertChildAfter"), &XmlNode::InsertChildAfter)
        .Func(_SC("InsertChildBefore"), &XmlNode::InsertChildBefore)
        .Func(_SC("InsertChildTypeAfter"), &XmlNode::InsertChildTypeAfter)
        .Func(_SC("InsertChildTypeBefore"), &XmlNode::InsertChildTypeBefore)
        .Func(_SC("AppendCopy"), &XmlNode::AppendCopy)
        .Func(_SC("PrependCopy"), &XmlNode::PrependCopy)
        .Func(_SC("InsertCopyAfter"), &XmlNode::InsertCopyAfter)
        .Func(_SC("InsertCopyBefore"), &XmlNode::InsertCopyBefore)
        .Func(_SC("AppendMove"), &XmlNode::AppendMove)
        .Func(_SC("PrependMove"), &XmlNode::PrependMove)
        .Func(_SC("InsertMoveAfter"), &XmlNode::InsertMoveAfter)
        .Func(_SC("InsertMoveBefore"), &XmlNode::InsertMoveBefore)
        .Func(_SC("RemoveAttr"), &XmlNode::RemoveAttr)
        .Func(_SC("RemoveAttrInst"), &XmlNode::RemoveAttrInst)
        .Func(_SC("RemoveChild"), &XmlNode::RemoveChild)
        .Func(_SC("RemoveChildInst"), &XmlNode::RemoveChildInst)
        .Overload< XmlNode (XmlNode::*)(CSStr, CSStr) const >(_SC("FindChildByAttr"), &XmlNode::FindChildByAttr)
        .Overload< XmlNode (XmlNode::*)(CSStr, CSStr, CSStr) const >(_SC("FindChildByAttr"), &XmlNode::FindChildByAttr)
        .Func(_SC("FindElemByPath"), &XmlNode::FindElemByPath)
    );

    xmlns.Bind(_SC("Document"), Class< XmlDocument, NoCopy< XmlDocument > >(vm, _SC("SqXmlDocument"))
        /* Constructors */
        .Ctor()
        /* Core Metamethods */
        .Func(_SC("_cmp"), &XmlDocument::Cmp)
        .Func(_SC("_tostring"), &XmlDocument::ToString)
        /* Properties */
        .Prop(_SC("Valid"), &XmlDocument::IsValid)
        .Prop(_SC("References"), &XmlDocument::GetRefCount)
        .Prop(_SC("Node"), &XmlDocument::GetNode)
        /* Functions */
        .Overload< void (XmlDocument::*)(void) >(_SC("Reset"), &XmlDocument::Reset)
        .Overload< void (XmlDocument::*)(const XmlDocument &) >(_SC("Reset"), &XmlDocument::Reset)
        .Overload< XmlParseResult (XmlDocument::*)(CSStr) >(_SC("LoadString"), &XmlDocument::LoadString)
        .Overload< XmlParseResult (XmlDocument::*)(CSStr, Uint32) >(_SC("LoadString"), &XmlDocument::LoadString)
        .Overload< XmlParseResult (XmlDocument::*)(CSStr) >(_SC("LoadFile"), &XmlDocument::LoadFile)
        .Overload< XmlParseResult (XmlDocument::*)(CSStr, Uint32) >(_SC("LoadFile"), &XmlDocument::LoadFile)
        .Overload< XmlParseResult (XmlDocument::*)(CSStr, Uint32, Int32) >(_SC("LoadFile"), &XmlDocument::LoadFile)
        .Overload< void (XmlDocument::*)(CSStr) >(_SC("SaveFile"), &XmlDocument::SaveFile)
        .Overload< void (XmlDocument::*)(CSStr, CSStr) >(_SC("SaveFile"), &XmlDocument::SaveFile)
        .Overload< void (XmlDocument::*)(CSStr, CSStr, Uint32) >(_SC("SaveFile"), &XmlDocument::SaveFile)
        .Overload< void (XmlDocument::*)(CSStr, CSStr, Uint32, Int32) >(_SC("SaveFile"), &XmlDocument::SaveFile)
    );

    RootTable(vm).Bind(_SC("SqXml"), xmlns);

    ConstTable(vm).Enum(_SC("SqXmlNodeType"), Enumeration(vm)
        .Const(_SC("Null"),                     Int32(node_null))
        .Const(_SC("Document"),                 Int32(node_document))
        .Const(_SC("Element"),                  Int32(node_element))
        .Const(_SC("PCData"),                   Int32(node_pcdata))
        .Const(_SC("CData"),                    Int32(node_cdata))
        .Const(_SC("Comment"),                  Int32(node_comment))
        .Const(_SC("Pi"),                       Int32(node_pi))
        .Const(_SC("Declaration"),              Int32(node_declaration))
        .Const(_SC("Doctype"),                  Int32(node_doctype))
    );

    ConstTable(vm).Enum(_SC("SqXmlParse"), Enumeration(vm)
        .Const(_SC("Minimal"),                  Int32(parse_minimal))
        .Const(_SC("Default"),                  Int32(parse_default))
        .Const(_SC("Full"),                     Int32(parse_full))
        .Const(_SC("Pi"),                       Int32(parse_pi))
        .Const(_SC("Comments"),                 Int32(parse_comments))
        .Const(_SC("CData"),                    Int32(parse_cdata))
        .Const(_SC("WSPCData"),                 Int32(parse_ws_pcdata))
        .Const(_SC("Escapes"),                  Int32(parse_escapes))
        .Const(_SC("EOL"),                      Int32(parse_eol))
        .Const(_SC("WConvAttribute"),           Int32(parse_wconv_attribute))
        .Const(_SC("WNormAttribute"),           Int32(parse_wnorm_attribute))
        .Const(_SC("Declaration"),              Int32(parse_declaration))
        .Const(_SC("Doctype"),                  Int32(parse_doctype))
        .Const(_SC("WSPCDataSingle"),           Int32(parse_ws_pcdata_single))
        .Const(_SC("TrimPCData"),               Int32(parse_trim_pcdata))
        .Const(_SC("Fragment"),                 Int32(parse_fragment))
        .Const(_SC("EmbedPCData"),              Int32(parse_embed_pcdata))
    );

    ConstTable(vm).Enum(_SC("SqXmlEncoding"), Enumeration(vm)
        .Const(_SC("Auto"),                     Int32(encoding_auto))
        .Const(_SC("Utf8"),                     Int32(encoding_utf8))
        .Const(_SC("Utf16LE"),                  Int32(encoding_utf16_le))
        .Const(_SC("Utf16BE"),                  Int32(encoding_utf16_be))
        .Const(_SC("Utf16"),                    Int32(encoding_utf16))
        .Const(_SC("Utf32LE"),                  Int32(encoding_utf32_le))
        .Const(_SC("Utf32BE"),                  Int32(encoding_utf32_be))
        .Const(_SC("Utf32"),                    Int32(encoding_utf32))
        .Const(_SC("WChar"),                    Int32(encoding_wchar))
        .Const(_SC("Latin1"),                   Int32(encoding_latin1))
    );

    ConstTable(vm).Enum(_SC("SqXmlFormat"), Enumeration(vm)
        .Const(_SC("Indent"),                   Int32(format_indent))
        .Const(_SC("WriteBOM"),                 Int32(format_write_bom))
        .Const(_SC("Raw"),                      Int32(format_raw))
        .Const(_SC("NoDeclaration"),            Int32(format_no_declaration))
        .Const(_SC("NoEscapes"),                Int32(format_no_escapes))
        .Const(_SC("SaveFileText"),             Int32(format_save_file_text))
        .Const(_SC("IndentAttributes"),         Int32(format_indent_attributes))
        .Const(_SC("Default"),                  Int32(format_default))
    );

    ConstTable(vm).Enum(_SC("SqXmlParseStatus"), Enumeration(vm)
        .Const(_SC("Ok"),                       Int32(status_ok))
        .Const(_SC("FileNotFound"),             Int32(status_file_not_found))
        .Const(_SC("IOError"),                  Int32(status_io_error))
        .Const(_SC("OutOfMemory"),              Int32(status_out_of_memory))
        .Const(_SC("InternalError"),            Int32(status_internal_error))
        .Const(_SC("UnrecognizedTag"),          Int32(status_unrecognized_tag))
        .Const(_SC("BadPi"),                    Int32(status_bad_pi))
        .Const(_SC("BadComment"),               Int32(status_bad_comment))
        .Const(_SC("BadCData"),                 Int32(status_bad_cdata))
        .Const(_SC("BadDoctype"),               Int32(status_bad_doctype))
        .Const(_SC("BadPCData"),                Int32(status_bad_pcdata))
        .Const(_SC("BadStartElement"),          Int32(status_bad_start_element))
        .Const(_SC("BadAttribute"),             Int32(status_bad_attribute))
        .Const(_SC("BadEndElement"),            Int32(status_bad_end_element))
        .Const(_SC("EndElementMismatch"),       Int32(status_end_element_mismatch))
        .Const(_SC("AppendInvalidRoot"),        Int32(status_append_invalid_root))
        .Const(_SC("NoDocumentElement"),        Int32(status_no_document_element))
    );
    ConstTable(vm).Enum(_SC("SqXmlXpathValueType"), Enumeration(vm)
        .Const(_SC("None"),                     Int32(xpath_type_none))
        .Const(_SC("NodeSet"),                  Int32(xpath_type_node_set))
        .Const(_SC("Number"),                   Int32(xpath_type_number))
        .Const(_SC("String"),                   Int32(xpath_type_string))
        .Const(_SC("Boolean"),                  Int32(xpath_type_boolean))
    );
}

} // Namespace:: SqMod
