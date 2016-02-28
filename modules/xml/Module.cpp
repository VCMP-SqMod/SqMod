// --------------------------------------------------------------------------------------------
#include "Module.hpp"
#include "Common.hpp"
#include "Attribute.hpp"
#include "Text.hpp"
#include "Node.hpp"
#include "Document.hpp"

// --------------------------------------------------------------------------------------------
#include <sqrat.h>

// --------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

// --------------------------------------------------------------------------------------------
#if defined(WIN32) || defined(_WIN32)
    #include <Windows.h>
#endif

namespace SqMod {

// --------------------------------------------------------------------------------------------
PluginFuncs*        _Func = NULL;
PluginCallbacks*    _Clbk = NULL;
PluginInfo*         _Info = NULL;

// --------------------------------------------------------------------------------------------
HSQAPI              _SqAPI = NULL;
HSQEXPORTS          _SqMod = NULL;
HSQUIRRELVM         _SqVM = NULL;

/* ------------------------------------------------------------------------------------------------
 * Bind speciffic functions to certain server events.
*/
void BindCallbacks();

/* ------------------------------------------------------------------------------------------------
 * Undo changes made with BindCallbacks().
*/
void UnbindCallbacks();

/* --------------------------------------------------------------------------------------------
 * Register the module API under the specified virtual machine.
*/
void RegisterAPI(HSQUIRRELVM vm);

/* --------------------------------------------------------------------------------------------
 * Initialize the plugin by obtaining the API provided by the host plugin.
*/
void OnSquirrelInitialize()
{
    // Attempt to import the plugin API exported by the host plugin
    _SqMod = sq_api_import(_Func);
    // Did we failed to obtain the plugin exports?
    if(!_SqMod)
        OutputError("Failed to attach [%s] on host plugin.", SQXML_NAME);
    else
    {
        // Obtain the Squirrel API
        _SqAPI = _SqMod->GetSquirrelAPI();
        // Expand the Squirrel API into global functions
        sq_api_expand(_SqAPI);
    }
}

/* --------------------------------------------------------------------------------------------
 * Load the module on the virtual machine provided by the host module.
*/
void OnSquirrelLoad()
{
    // Make sure that we have a valid plugin API
    if (!_SqMod)
        return; /* Unable to proceed. */
    // Obtain the Squirrel API and VM
    _SqVM = _SqMod->GetSquirrelVM();
    // Make sure that a valid virtual machine exists
    if (!_SqVM)
        return; /* Unable to proceed. */
    // Set this as the default database
    DefaultVM::Set(_SqVM);
    // Register the module API
    RegisterAPI(_SqVM);
    // Notify about the current status
    OutputMessage("Registered: %s", SQXML_NAME);
}

/* --------------------------------------------------------------------------------------------
 * The virtual machine is about to be terminated and script resources should be released.
*/
void OnSquirrelTerminate()
{
    OutputMessage("Terminating: %s", SQXML_NAME);
    // Release the current database (if any)
    DefaultVM::Set(NULL);
}

/* --------------------------------------------------------------------------------------------
 * Validate the module API to make sure we don't run into issues.
*/
bool CheckAPIVer(CCStr ver)
{
    // Obtain the numeric representation of the API version
    long vernum = strtol(ver, NULL, 10);
    // Check against version mismatch
    if (vernum == SQMOD_API_VER)
        return true;
    // Log the incident
    OutputError("API version mismatch on %s", SQXML_NAME);
    OutputMessage("=> Requested: %ld Have: %ld", vernum, SQMOD_API_VER);
    // Invoker should not attempt to communicate through the module API
    return false;
}

/* --------------------------------------------------------------------------------------------
 * React to command sent by other plugins.
*/
static int OnInternalCommand(unsigned int type, const char * text)
{
    switch(type)
    {
        case SQMOD_INITIALIZE_CMD:
            if (CheckAPIVer(text))
                OnSquirrelInitialize();
        break;
        case SQMOD_LOAD_CMD:
            OnSquirrelLoad();
        break;
        case SQMOD_TERMINATE_CMD:
            OnSquirrelTerminate();
        break;
        default: break;
    }
    return 1;
}

/* --------------------------------------------------------------------------------------------
 * The server was initialized and this plugin was loaded successfully.
*/
static int OnInitServer()
{
    return 1;
}

static void OnShutdownServer(void)
{
    // The server may still send callbacks
    UnbindCallbacks();
}

// ------------------------------------------------------------------------------------------------
void BindCallbacks()
{
    _Clbk->OnInitServer             = OnInitServer;
    _Clbk->OnInternalCommand        = OnInternalCommand;
    _Clbk->OnShutdownServer         = OnShutdownServer;
}

// ------------------------------------------------------------------------------------------------
void UnbindCallbacks()
{
    _Clbk->OnInitServer             = NULL;
    _Clbk->OnInternalCommand        = NULL;
    _Clbk->OnShutdownServer         = NULL;
}

// --------------------------------------------------------------------------------------------
void RegisterAPI(HSQUIRRELVM vm)
{
    Table xmlns(vm);

    xmlns.Bind(_SC("ParseResult"), Class< ParseResult >(vm, _SC("SqXmlParseResult"))
        /* Constructors */
        .Ctor()
        .Ctor< const ParseResult & >()
        /* Core Metamethods */
        .Func(_SC("_cmp"), &ParseResult::Cmp)
        .SquirrelFunc(_SC("_typename"), &ParseResult::Typename)
        .Func(_SC("_tostring"), &ParseResult::ToString)
        /* Properties */
        .Prop(_SC("Valid"), &ParseResult::IsValid)
        .Prop(_SC("References"), &ParseResult::GetRefCount)
        .Prop(_SC("Ok"), &ParseResult::IsOk)
        .Prop(_SC("Status"), &ParseResult::GetStatus)
        .Prop(_SC("Offset"), &ParseResult::GetOffset)
        .Prop(_SC("Encoding"), &ParseResult::GetEncoding)
        .Prop(_SC("Description"), &ParseResult::GetDescription)
        /* Functions */
        .Func(_SC("Check"), &ParseResult::Check)
    );

    xmlns.Bind(_SC("Attribute"), Class< Attribute >(vm, _SC("SqXmlAttribute"))
        /* Constructors */
        .Ctor()
        .Ctor< const Attribute & >()
        /* Core Metamethods */
        .Func(_SC("_cmp"), &Attribute::Cmp)
        .SquirrelFunc(_SC("_typename"), &Attribute::Typename)
        .Func(_SC("_tostring"), &Attribute::ToString)
        /* Properties */
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
        /* Functions */
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
        /* Constructors */
        .Ctor()
        .Ctor< const Text & >()
        /* Core Metamethods */
        .Func(_SC("_cmp"), &Text::Cmp)
        .SquirrelFunc(_SC("_typename"), &Text::Typename)
        .Func(_SC("_tostring"), &Text::ToString)
        /* Properties */
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
        /* Functions */
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
        /* Constructors */
        .Ctor()
        .Ctor< const Node & >()
        /* Core Metamethods */
        .Func(_SC("_cmp"), &Node::Cmp)
        .SquirrelFunc(_SC("_typename"), &Node::Typename)
        .Func(_SC("_tostring"), &Node::ToString)
        /* Properties */
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
        /* Functions */
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
        /* Constructors */
        .Ctor()
        /* Core Metamethods */
        .Func(_SC("_cmp"), &Document::Cmp)
        .SquirrelFunc(_SC("_typename"), &Document::Typename)
        .Func(_SC("_tostring"), &Document::ToString)
        /* Properties */
        .Prop(_SC("Valid"), &Document::IsValid)
        .Prop(_SC("References"), &Document::GetRefCount)
        .Prop(_SC("Node"), &Document::GetNode)
        /* Functions */
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

// --------------------------------------------------------------------------------------------
void OutputMessageImpl(const char * msg, va_list args)
{
#if defined(WIN32) || defined(_WIN32)
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csb_before;
    GetConsoleScreenBufferInfo( hstdout, &csb_before);
    SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN);
    printf("[SQMOD] ");

    SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
    vprintf(msg, args);
    puts("");

    SetConsoleTextAttribute(hstdout, csb_before.wAttributes);
#else
    printf("%c[0;32m[SQMOD]%c[0;37m", 27, 27, msg);
    vprintf(msg, args);
    puts("");
#endif
}

// --------------------------------------------------------------------------------------------
void OutputErrorImpl(const char * msg, va_list args)
{
#if defined(WIN32) || defined(_WIN32)
    HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csb_before;
    GetConsoleScreenBufferInfo( hstdout, &csb_before);
    SetConsoleTextAttribute(hstdout, FOREGROUND_RED | FOREGROUND_INTENSITY);
    printf("[SQMOD] ");

    SetConsoleTextAttribute(hstdout, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
    vprintf(msg, args);
    puts("");

    SetConsoleTextAttribute(hstdout, csb_before.wAttributes);
#else
    printf("%c[0;32m[SQMOD]%c[0;37m", 27, 27, msg);
    vprintf(msg, args);
    puts("");
#endif
}

// --------------------------------------------------------------------------------------------
void OutputDebug(const char * msg, ...)
{
#ifdef _DEBUG
    // Initialize the arguments list
    va_list args;
    va_start(args, msg);
    // Call the output function
    OutputMessageImpl(msg, args);
    // Finalize the arguments list
    va_end(args);
#else
    SQMOD_UNUSED_VAR(msg);
#endif
}

// --------------------------------------------------------------------------------------------
void OutputMessage(const char * msg, ...)
{
    // Initialize the arguments list
    va_list args;
    va_start(args, msg);
    // Call the output function
    OutputMessageImpl(msg, args);
    // Finalize the arguments list
    va_end(args);
}

// --------------------------------------------------------------------------------------------
void OutputError(const char * msg, ...)
{
    // Initialize the arguments list
    va_list args;
    va_start(args, msg);
    // Call the output function
    OutputErrorImpl(msg, args);
    // Finalize the arguments list
    va_end(args);
}

} // Namespace:: SqMod

// --------------------------------------------------------------------------------------------
SQMOD_API_EXPORT unsigned int VcmpPluginInit(PluginFuncs* functions, PluginCallbacks* callbacks, PluginInfo* info)
{
    using namespace SqMod;
    // Output plugin header
    puts("");
    OutputMessage("--------------------------------------------------------------------");
    OutputMessage("Plugin: %s", SQXML_NAME);
    OutputMessage("Author: %s", SQXML_AUTHOR);
    OutputMessage("Legal: %s", SQXML_COPYRIGHT);
    OutputMessage("--------------------------------------------------------------------");
    puts("");
    // Attempt to find the host plugin ID
    int host_plugin_id = functions->FindPlugin((char *)(SQMOD_HOST_NAME));
    // See if our plugin was loaded after the host plugin
    if (host_plugin_id < 0)
    {
        OutputError("%s could find the host plugin", SQXML_NAME);
        // Don't load!
        return SQMOD_FAILURE;
    }
    // Should never reach this point but just in case
    else if (host_plugin_id > (info->nPluginId))
    {
        OutputError("%s loaded after the host plugin", SQXML_NAME);
        // Don't load!
        return SQMOD_FAILURE;
    }
    // Store server proxies
    _Func = functions;
    _Clbk = callbacks;
    _Info = info;
    // Assign plugin information
    _Info->uPluginVer = SQXML_VERSION;
    strcpy(_Info->szName, SQXML_HOST_NAME);
    // Bind callbacks
    BindCallbacks();
    // Notify that the plugin was successfully loaded
    OutputMessage("Successfully loaded %s", SQXML_NAME);
    // Dummy spacing
    puts("");
    // Done!
    return SQMOD_SUCCESS;
}
