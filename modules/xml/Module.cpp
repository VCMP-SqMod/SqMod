// ------------------------------------------------------------------------------------------------
#include "Common.hpp"
#include "Attribute.hpp"
#include "Text.hpp"
#include "Node.hpp"
#include "Document.hpp"
#include "Wrapper/ParseResult.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <cstdlib>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Register the module API under the obtained virtual machine.
*/
static bool RegisterAPI(HSQUIRRELVM vm)
{
    // Make sure there's a valid virtual machine before proceeding
    if (!vm)
    {
        OutputError("%s: Cannot register API without a valid virtual machine", SQXML_NAME);
        // Registration failed
        return false;
    }

    Table xmlns(vm);

    xmlns.Bind(_SC("ParseResult"), Class< ParseResult >(vm, _SC("SqXmlParseResult"))
        // Constructors
        .Ctor()
        .Ctor< const ParseResult & >()
        // Core Meta-methods
        .Func(_SC("_cmp"), &ParseResult::Cmp)
        .SquirrelFunc(_SC("_typename"), &ParseResult::Typename)
        .Func(_SC("_tostring"), &ParseResult::ToString)
        // Properties
        .Prop(_SC("Valid"), &ParseResult::IsValid)
        .Prop(_SC("References"), &ParseResult::GetRefCount)
        .Prop(_SC("Ok"), &ParseResult::IsOk)
        .Prop(_SC("Status"), &ParseResult::GetStatus)
        .Prop(_SC("Offset"), &ParseResult::GetOffset)
        .Prop(_SC("Encoding"), &ParseResult::GetEncoding)
        .Prop(_SC("Description"), &ParseResult::GetDescription)
        // Member Methods
        .Func(_SC("Check"), &ParseResult::Check)
    );

    xmlns.Bind(_SC("Attribute"), Class< Attribute >(vm, _SC("SqXmlAttribute"))
        // Constructors
        .Ctor()
        .Ctor< const Attribute & >()
        // Core Meta-methods
        .Func(_SC("_cmp"), &Attribute::Cmp)
        .SquirrelFunc(_SC("_typename"), &Attribute::Typename)
        .Func(_SC("_tostring"), &Attribute::ToString)
        // Properties
        .Prop(_SC("Valid"), &Attribute::IsValid)
        .Prop(_SC("References"), &Attribute::GetRefCount)
        .Prop(_SC("Empty"), &Attribute::IsEmpty)
        .Prop(_SC("Hash"), &Attribute::GetHashValue)
        .Prop(_SC("Name"), &Attribute::GetName, &Attribute::SetName)
        .Prop(_SC("Value"), &Attribute::GetValue, &Attribute::SetValue)
        .Prop(_SC("Int"), &Attribute::GetInt, &Attribute::SetInt)
        .Prop(_SC("Uint"), &Attribute::GetUint, &Attribute::SetUint)
        .Prop(_SC("Float"), &Attribute::GetFloat, &Attribute::SetFloat)
        .Prop(_SC("Double"), &Attribute::GetDouble, &Attribute::SetDouble)
        .Prop(_SC("Long"), &Attribute::GetLong, &Attribute::SetLong)
        .Prop(_SC("Ulong"), &Attribute::GetUlong, &Attribute::SetUlong)
        .Prop(_SC("Bool"), &Attribute::GetBool, &Attribute::SetBool)
        .Prop(_SC("Next"), &Attribute::NextAttribute)
        .Prop(_SC("Prev"), &Attribute::PrevAttribute)
        // Member Methods
        .Func(_SC("SetName"), &Attribute::ApplyName)
        .Func(_SC("SetValue"), &Attribute::ApplyValue)
        .Func(_SC("AsString"), &Attribute::AsString)
        .Func(_SC("AsInt"), &Attribute::AsInt)
        .Func(_SC("AsUint"), &Attribute::AsUint)
        .Func(_SC("AsFloat"), &Attribute::AsFloat)
        .Func(_SC("AsDouble"), &Attribute::AsDouble)
        .Func(_SC("AsLong"), &Attribute::AsLong)
        .Func(_SC("AsUlong"), &Attribute::AsUlong)
        .Func(_SC("AsBool"), &Attribute::AsBool)
        .Func(_SC("SetString"), &Attribute::ApplyString)
        .Func(_SC("SetInt"), &Attribute::ApplyInt)
        .Func(_SC("SetUint"), &Attribute::ApplyUint)
        .Func(_SC("SetFloat"), &Attribute::ApplyFloat)
        .Func(_SC("SetDouble"), &Attribute::ApplyDouble)
        .Func(_SC("SetLong"), &Attribute::ApplyLong)
        .Func(_SC("SetUlong"), &Attribute::ApplyUlong)
        .Func(_SC("SetBool"), &Attribute::ApplyBool)
    );

    xmlns.Bind(_SC("Text"), Class< Text >(vm, _SC("SqXmlText"))
        // Constructors
        .Ctor()
        .Ctor< const Text & >()
        // Core Meta-methods
        .Func(_SC("_cmp"), &Text::Cmp)
        .SquirrelFunc(_SC("_typename"), &Text::Typename)
        .Func(_SC("_tostring"), &Text::ToString)
        // Properties
        .Prop(_SC("Valid"), &Text::IsValid)
        .Prop(_SC("References"), &Text::GetRefCount)
        .Prop(_SC("Empty"), &Text::IsEmpty)
        .Prop(_SC("Value"), &Text::GetValue)
        .Prop(_SC("Int"), &Text::GetInt, &Text::SetInt)
        .Prop(_SC("Uint"), &Text::GetUint, &Text::SetUint)
        .Prop(_SC("Float"), &Text::GetFloat, &Text::SetFloat)
        .Prop(_SC("Double"), &Text::GetDouble, &Text::SetDouble)
        .Prop(_SC("Long"), &Text::GetLong, &Text::SetLong)
        .Prop(_SC("Ulong"), &Text::GetUlong, &Text::SetUlong)
        .Prop(_SC("Bool"), &Text::GetBool, &Text::SetBool)
        .Prop(_SC("Data"), &Text::GetData)
        // Member Methods
        .Func(_SC("AsString"), &Text::AsString)
        .Func(_SC("AsInt"), &Text::AsInt)
        .Func(_SC("AsUint"), &Text::AsUint)
        .Func(_SC("AsFloat"), &Text::AsFloat)
        .Func(_SC("AsDouble"), &Text::AsDouble)
        .Func(_SC("AsLong"), &Text::AsLong)
        .Func(_SC("AsUlong"), &Text::AsUlong)
        .Func(_SC("AsBool"), &Text::AsBool)
        .Func(_SC("SetString"), &Text::ApplyString)
        .Func(_SC("SetInt"), &Text::ApplyInt)
        .Func(_SC("SetUint"), &Text::ApplyUint)
        .Func(_SC("SetFloat"), &Text::ApplyFloat)
        .Func(_SC("SetDouble"), &Text::ApplyDouble)
        .Func(_SC("SetLong"), &Text::ApplyLong)
        .Func(_SC("SetUlong"), &Text::ApplyUlong)
        .Func(_SC("SetBool"), &Text::ApplyBool)
    );

    xmlns.Bind(_SC("Node"), Class< Node >(vm, _SC("SqXmlNode"))
        // Constructors
        .Ctor()
        .Ctor< const Node & >()
        // Core Meta-methods
        .Func(_SC("_cmp"), &Node::Cmp)
        .SquirrelFunc(_SC("_typename"), &Node::Typename)
        .Func(_SC("_tostring"), &Node::ToString)
        // Properties
        .Prop(_SC("Valid"), &Node::IsValid)
        .Prop(_SC("References"), &Node::GetRefCount)
        .Prop(_SC("Empty"), &Node::IsEmpty)
        .Prop(_SC("Hash"), &Node::GetHashValue)
        .Prop(_SC("OffsetDebug"), &Node::GetOffsetDebug)
        .Prop(_SC("Type"), &Node::GetType)
        .Prop(_SC("Name"), &Node::GetName, &Node::SetName)
        .Prop(_SC("Value"), &Node::GetValue, &Node::SetValue)
        .Prop(_SC("FirstAttr"), &Node::GetFirstAttr)
        .Prop(_SC("LastAttr"), &Node::GetLastAttr)
        .Prop(_SC("FirstChild"), &Node::GetFirstChild)
        .Prop(_SC("LastChild"), &Node::GetLastChild)
        .Prop(_SC("NextSibling"), &Node::GetNextSibling)
        .Prop(_SC("PrevSibling"), &Node::GetPrevSibling)
        .Prop(_SC("Parent"), &Node::GetParent)
        .Prop(_SC("Root"), &Node::GetRoot)
        .Prop(_SC("Text"), &Node::GetText)
        .Prop(_SC("ChildValue"), &Node::GetChildValue)
        // Member Methods
        .Overload< ParseResult (Node::*)(CSStr) >(_SC("AppendBuffer"), &Node::AppendBuffer)
        .Overload< ParseResult (Node::*)(CSStr, Uint32) >(_SC("AppendBuffer"), &Node::AppendBuffer)
        .Overload< ParseResult (Node::*)(CSStr, Uint32, Int32) >(_SC("AppendBuffer"), &Node::AppendBuffer)
        .Func(_SC("SetName"), &Node::ApplyName)
        .Func(_SC("SetValue"), &Node::ApplyValue)
        .Func(_SC("GetChild"), &Node::Child)
        .Func(_SC("GetAttr"), &Node::GetAttribute)
        .Func(_SC("GetAttribute"), &Node::GetAttribute)
        .Func(_SC("GetAttrFrom"), &Node::AttributeFrom)
        .Func(_SC("GetAttributeFrom"), &Node::AttributeFrom)
        .Func(_SC("GetNextSibling"), &Node::NextSibling)
        .Func(_SC("GetPrevSibling"), &Node::PrevSibling)
        .Func(_SC("GetChildValue"), &Node::ChildValue)
        .Func(_SC("AppendAttr"), &Node::AppendAttr)
        .Func(_SC("PrependAttr"), &Node::PrependAttr)
        .Func(_SC("InsertAttrAfter"), &Node::InsertAttrAfter)
        .Func(_SC("InsertAttrBefore"), &Node::InsertAttrBefore)
        .Func(_SC("AppendAttrCopy"), &Node::AppendAttrCopy)
        .Func(_SC("PrependAttrCopy"), &Node::PrependAttrCopy)
        .Func(_SC("InsertAttrCopyAfter"), &Node::InsertAttrCopyAfter)
        .Func(_SC("InsertAttrCopyBefore"), &Node::InsertAttrCopyBefore)
        .Func(_SC("AppendChild"), &Node::AppendChild)
        .Func(_SC("PrependChild"), &Node::PrependChild)
        .Func(_SC("AppendChildNode"), &Node::AppendChildNode)
        .Func(_SC("PrependChildNode"), &Node::PrependChildNode)
        .Func(_SC("AppendChildType"), &Node::AppendChildType)
        .Func(_SC("PrependChildType"), &Node::PrependChildType)
        .Func(_SC("InsertChildAfter"), &Node::InsertChildAfter)
        .Func(_SC("InsertChildBefore"), &Node::InsertChildBefore)
        .Func(_SC("InsertChildTypeAfter"), &Node::InsertChildTypeAfter)
        .Func(_SC("InsertChildTypeBefore"), &Node::InsertChildTypeBefore)
        .Func(_SC("AppendCopy"), &Node::AppendCopy)
        .Func(_SC("PrependCopy"), &Node::PrependCopy)
        .Func(_SC("InsertCopyAfter"), &Node::InsertCopyAfter)
        .Func(_SC("InsertCopyBefore"), &Node::InsertCopyBefore)
        .Func(_SC("AppendMove"), &Node::AppendMove)
        .Func(_SC("PrependMove"), &Node::PrependMove)
        .Func(_SC("InsertMoveAfter"), &Node::InsertMoveAfter)
        .Func(_SC("InsertMoveBefore"), &Node::InsertMoveBefore)
        .Func(_SC("RemoveAttr"), &Node::RemoveAttr)
        .Func(_SC("RemoveAttrInst"), &Node::RemoveAttrInst)
        .Func(_SC("RemoveChild"), &Node::RemoveChild)
        .Func(_SC("RemoveChildInst"), &Node::RemoveChildInst)
        .Overload< Node (Node::*)(CSStr, CSStr) const >(_SC("FindChildByAttr"), &Node::FindChildByAttr)
        .Overload< Node (Node::*)(CSStr, CSStr, CSStr) const >(_SC("FindChildByAttr"), &Node::FindChildByAttr)
        .Func(_SC("FindElemByPath"), &Node::FindElemByPath)
    );

    xmlns.Bind(_SC("Document"), Class< Document, NoCopy< Document > >(vm, _SC("SqXmlDocument"))
        // Constructors
        .Ctor()
        // Core Meta-methods
        .Func(_SC("_cmp"), &Document::Cmp)
        .SquirrelFunc(_SC("_typename"), &Document::Typename)
        .Func(_SC("_tostring"), &Document::ToString)
        // Properties
        .Prop(_SC("Valid"), &Document::IsValid)
        .Prop(_SC("References"), &Document::GetRefCount)
        .Prop(_SC("Node"), &Document::GetNode)
        // Member Methods
        .Overload< void (Document::*)(void) >(_SC("Reset"), &Document::Reset)
        .Overload< void (Document::*)(const Document &) >(_SC("Reset"), &Document::Reset)
        .Overload< ParseResult (Document::*)(CSStr) >(_SC("LoadString"), &Document::LoadData)
        .Overload< ParseResult (Document::*)(CSStr, Uint32) >(_SC("LoadString"), &Document::LoadData)
        .Overload< ParseResult (Document::*)(CSStr) >(_SC("LoadFile"), &Document::LoadFile)
        .Overload< ParseResult (Document::*)(CSStr, Uint32) >(_SC("LoadFile"), &Document::LoadFile)
        .Overload< ParseResult (Document::*)(CSStr, Uint32, Int32) >(_SC("LoadFile"), &Document::LoadFile)
        .Overload< void (Document::*)(CSStr) >(_SC("SaveFile"), &Document::SaveFile)
        .Overload< void (Document::*)(CSStr, CSStr) >(_SC("SaveFile"), &Document::SaveFile)
        .Overload< void (Document::*)(CSStr, CSStr, Uint32) >(_SC("SaveFile"), &Document::SaveFile)
        .Overload< void (Document::*)(CSStr, CSStr, Uint32, Int32) >(_SC("SaveFile"), &Document::SaveFile)
    );

    RootTable(vm).Bind(_SC("SqXml"), xmlns);

    ConstTable(vm).Enum(_SC("SqXmlNodeType"), Enumeration(vm)
        .Const(_SC("Null"),                     static_cast< Int32 >(node_null))
        .Const(_SC("Document"),                 static_cast< Int32 >(node_document))
        .Const(_SC("Element"),                  static_cast< Int32 >(node_element))
        .Const(_SC("PCData"),                   static_cast< Int32 >(node_pcdata))
        .Const(_SC("CData"),                    static_cast< Int32 >(node_cdata))
        .Const(_SC("Comment"),                  static_cast< Int32 >(node_comment))
        .Const(_SC("Pi"),                       static_cast< Int32 >(node_pi))
        .Const(_SC("Declaration"),              static_cast< Int32 >(node_declaration))
        .Const(_SC("Doctype"),                  static_cast< Int32 >(node_doctype))
    );

    ConstTable(vm).Enum(_SC("SqXmlParse"), Enumeration(vm)
        .Const(_SC("Minimal"),                  static_cast< Int32 >(parse_minimal))
        .Const(_SC("Default"),                  static_cast< Int32 >(parse_default))
        .Const(_SC("Full"),                     static_cast< Int32 >(parse_full))
        .Const(_SC("Pi"),                       static_cast< Int32 >(parse_pi))
        .Const(_SC("Comments"),                 static_cast< Int32 >(parse_comments))
        .Const(_SC("CData"),                    static_cast< Int32 >(parse_cdata))
        .Const(_SC("WSPCData"),                 static_cast< Int32 >(parse_ws_pcdata))
        .Const(_SC("Escapes"),                  static_cast< Int32 >(parse_escapes))
        .Const(_SC("EOL"),                      static_cast< Int32 >(parse_eol))
        .Const(_SC("WConvAttribute"),           static_cast< Int32 >(parse_wconv_attribute))
        .Const(_SC("WNormAttribute"),           static_cast< Int32 >(parse_wnorm_attribute))
        .Const(_SC("Declaration"),              static_cast< Int32 >(parse_declaration))
        .Const(_SC("Doctype"),                  static_cast< Int32 >(parse_doctype))
        .Const(_SC("WSPCDataSingle"),           static_cast< Int32 >(parse_ws_pcdata_single))
        .Const(_SC("TrimPCData"),               static_cast< Int32 >(parse_trim_pcdata))
        .Const(_SC("Fragment"),                 static_cast< Int32 >(parse_fragment))
        .Const(_SC("EmbedPCData"),              static_cast< Int32 >(parse_embed_pcdata))
    );

    ConstTable(vm).Enum(_SC("SqXmlEncoding"), Enumeration(vm)
        .Const(_SC("Auto"),                     static_cast< Int32 >(encoding_auto))
        .Const(_SC("Utf8"),                     static_cast< Int32 >(encoding_utf8))
        .Const(_SC("Utf16LE"),                  static_cast< Int32 >(encoding_utf16_le))
        .Const(_SC("Utf16BE"),                  static_cast< Int32 >(encoding_utf16_be))
        .Const(_SC("Utf16"),                    static_cast< Int32 >(encoding_utf16))
        .Const(_SC("Utf32LE"),                  static_cast< Int32 >(encoding_utf32_le))
        .Const(_SC("Utf32BE"),                  static_cast< Int32 >(encoding_utf32_be))
        .Const(_SC("Utf32"),                    static_cast< Int32 >(encoding_utf32))
        .Const(_SC("WChar"),                    static_cast< Int32 >(encoding_wchar))
        .Const(_SC("Latin1"),                   static_cast< Int32 >(encoding_latin1))
    );

    ConstTable(vm).Enum(_SC("SqXmlFormat"), Enumeration(vm)
        .Const(_SC("Indent"),                   static_cast< Int32 >(format_indent))
        .Const(_SC("WriteBOM"),                 static_cast< Int32 >(format_write_bom))
        .Const(_SC("Raw"),                      static_cast< Int32 >(format_raw))
        .Const(_SC("NoDeclaration"),            static_cast< Int32 >(format_no_declaration))
        .Const(_SC("NoEscapes"),                static_cast< Int32 >(format_no_escapes))
        .Const(_SC("SaveFileText"),             static_cast< Int32 >(format_save_file_text))
        .Const(_SC("IndentAttributes"),         static_cast< Int32 >(format_indent_attributes))
        .Const(_SC("Default"),                  static_cast< Int32 >(format_default))
    );

    ConstTable(vm).Enum(_SC("SqXmlParseStatus"), Enumeration(vm)
        .Const(_SC("Ok"),                       static_cast< Int32 >(status_ok))
        .Const(_SC("FileNotFound"),             static_cast< Int32 >(status_file_not_found))
        .Const(_SC("IOError"),                  static_cast< Int32 >(status_io_error))
        .Const(_SC("OutOfMemory"),              static_cast< Int32 >(status_out_of_memory))
        .Const(_SC("InternalError"),            static_cast< Int32 >(status_internal_error))
        .Const(_SC("UnrecognizedTag"),          static_cast< Int32 >(status_unrecognized_tag))
        .Const(_SC("BadPi"),                    static_cast< Int32 >(status_bad_pi))
        .Const(_SC("BadComment"),               static_cast< Int32 >(status_bad_comment))
        .Const(_SC("BadCData"),                 static_cast< Int32 >(status_bad_cdata))
        .Const(_SC("BadDoctype"),               static_cast< Int32 >(status_bad_doctype))
        .Const(_SC("BadPCData"),                static_cast< Int32 >(status_bad_pcdata))
        .Const(_SC("BadStartElement"),          static_cast< Int32 >(status_bad_start_element))
        .Const(_SC("BadAttribute"),             static_cast< Int32 >(status_bad_attribute))
        .Const(_SC("BadEndElement"),            static_cast< Int32 >(status_bad_end_element))
        .Const(_SC("EndElementMismatch"),       static_cast< Int32 >(status_end_element_mismatch))
        .Const(_SC("AppendInvalidRoot"),        static_cast< Int32 >(status_append_invalid_root))
        .Const(_SC("NoDocumentElement"),        static_cast< Int32 >(status_no_document_element))
    );

    ConstTable(vm).Enum(_SC("SqXmlXpathValueType"), Enumeration(vm)
        .Const(_SC("None"),                     static_cast< Int32 >(xpath_type_none))
        .Const(_SC("NodeSet"),                  static_cast< Int32 >(xpath_type_node_set))
        .Const(_SC("Number"),                   static_cast< Int32 >(xpath_type_number))
        .Const(_SC("String"),                   static_cast< Int32 >(xpath_type_string))
        .Const(_SC("Boolean"),                  static_cast< Int32 >(xpath_type_boolean))
    );

    // Registration was successful
    return true;
}

/* ------------------------------------------------------------------------------------------------
 * Load the module on the virtual machine provided by the host module.
*/
static bool OnSquirrelLoad()
{
    // Make sure that we have a valid module API
    if (!SqMod_GetSquirrelVM)
    {
        OutputError("%s: Cannot obtain the Squirrel virtual machine without the module API", SQXML_NAME);
        // Unable to proceed!
        return false;
    }
    // Obtain the Squirrel virtual machine from the host plug-in
    DefaultVM::Set(SqMod_GetSquirrelVM());
    // Make sure that a valid virtual machine exists
    if (!DefaultVM::Get())
    {
        OutputError("%s: Squirrel virtual machine obtained from the host plug-in is invalid", SQXML_NAME);
        // Unable to proceed!
        return false;
    }
    // Prevent common null objects from using dead virtual machines
    NullArray() = Array();
    NullTable() = Table();
    NullObject() = Object();
    NullFunction() = Function();
    // Register the module API
    if (RegisterAPI(DefaultVM::Get()))
    {
        OutputMessage("Registered: %s", SQXML_NAME);
    }
    else
    {
        return false;
    }
    // At this point, the module was successfully loaded
    return true;
}

/* ------------------------------------------------------------------------------------------------
 * The virtual machine is about to be terminated and script resources should be released.
*/
static void OnSquirrelTerminate()
{
    OutputMessage("Terminating: %s", SQXML_NAME);
    // Release null objects just in case
    NullObject().Release();
    NullTable().Release();
    NullArray().Release();
    NullFunction().ReleaseGently();
    // Release script resources...
}

/* ------------------------------------------------------------------------------------------------
 * The virtual machined is about to be closed. Last chance to release anything manually.
*/
static void OnSquirrelClosing()
{
	// Nothing to release manually...
}

/* ------------------------------------------------------------------------------------------------
 * The virtual machined was closed and all memory associated with it was released.
*/
static void OnSquirrelReleased()
{
    // Release the current virtual machine, if any
    DefaultVM::Set(nullptr);
}

/* ------------------------------------------------------------------------------------------------
 * React to command sent by other plug-ins.
*/
static uint8_t OnPluginCommand(uint32_t command_identifier, CCStr message)
{
    switch(command_identifier)
    {
        case SQMOD_INITIALIZE_CMD:
        {
            if (CheckModuleAPIVer(message, SQXML_NAME))
            {
                try
                {
                    ImportModuleAPI(_Func, SQXML_NAME);
                }
                catch (const Sqrat::Exception & e)
                {
                    OutputError("%s", e.what());
                    // Failed to initialize
                    return 0;
                }
            }
        } break;
        case SQMOD_LOAD_CMD:
        {
            return OnSquirrelLoad();
        } break;
        case SQMOD_TERMINATE_CMD:
        {
            OnSquirrelTerminate();
        } break;
        case SQMOD_CLOSING_CMD:
        {
            OnSquirrelClosing();
        } break;
        case SQMOD_RELEASED_CMD:
        {
            OnSquirrelReleased();
        } break;
        default: break;
    }
    return 1;
}

/* ------------------------------------------------------------------------------------------------
 * The server was initialized and this plug-in was loaded successfully.
*/
static uint8_t OnServerInitialise()
{
    return 1; // Initialization was successful
}

/* ------------------------------------------------------------------------------------------------
 * The server is about to shutdown gracefully.
*/
static void OnServerShutdown(void)
{
    // The server may still send callbacks
    _Clbk->OnServerInitialise       = nullptr;
    _Clbk->OnServerShutdown         = nullptr;
    _Clbk->OnPluginCommand          = nullptr;
}

} // Namespace:: SqMod

// ------------------------------------------------------------------------------------------------
SQMOD_API_EXPORT unsigned int VcmpPluginInit(PluginFuncs * functions, PluginCallbacks * callbacks, PluginInfo * info)
{
    using namespace SqMod;
    // Output plug-in header
    puts("");
    OutputMessage("--------------------------------------------------------------------");
    OutputMessage("Plug-in: %s", SQXML_NAME);
    OutputMessage("Author: %s", SQXML_AUTHOR);
    OutputMessage("Legal: %s", SQXML_COPYRIGHT);
    OutputMessage("--------------------------------------------------------------------");
    puts("");
    // Make sure that the module was loaded after the host plug-in
    if (!CheckModuleOrder(functions, info->pluginId, SQXML_NAME))
    {
        return SQMOD_FAILURE;
    }
    // Store server proxies
    _Func = functions;
    _Clbk = callbacks;
    _Info = info;
    // Assign plug-in version
    _Info->pluginVersion = SQXML_VERSION;
    _Info->apiMajorVersion = PLUGIN_API_MAJOR;
    _Info->apiMinorVersion = PLUGIN_API_MINOR;
    // Assign the plug-in name
    std::snprintf(_Info->name, sizeof(_Info->name), "%s", SQXML_HOST_NAME);
    // Bind to the server callbacks
    _Clbk->OnServerInitialise       = OnServerInitialise;
    _Clbk->OnServerShutdown         = OnServerShutdown;
    _Clbk->OnPluginCommand          = OnPluginCommand;
    // Notify that the plug-in was successfully loaded
    OutputMessage("Successfully loaded %s", SQXML_NAME);
    // Dummy spacing
    puts("");
    // Done!
    return SQMOD_SUCCESS;
}
