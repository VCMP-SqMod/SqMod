// ------------------------------------------------------------------------------------------------
#include "Library/RegEx.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqratConst.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqRxMatchTypename, _SC("SqRxMatch"))
SQMOD_DECL_TYPENAME(SqRxMatchesTypename, _SC("SqRxMatches"))
SQMOD_DECL_TYPENAME(SqRxInstanceTypename, _SC("SqRxInstance"))

// ------------------------------------------------------------------------------------------------
bool RxInstance::STUDY = true;
int RxInstance::OPTIONS = 0;
int RxInstance::STUDY_OPTIONS = 0;

// ================================================================================================
void Register_RegEx(HSQUIRRELVM vm)
{
    RootTable(vm).Bind(SqRxMatchTypename::Str,
       Class< RxMatch >(vm, SqRxMatchTypename::Str)
       // Constructors
       .Ctor()
       .Ctor< SQInteger >()
       .Ctor< SQInteger, SQInteger >()
       // Meta-methods
       .SquirrelFunc(_SC("_typename"), &SqRxMatchTypename::Fn)
       // Properties
       .Prop(_SC("Offset"), &RxMatch::GetOffset, &RxMatch::SetOffset)
       .Prop(_SC("Length"), &RxMatch::GetLength, &RxMatch::SetLength)
       .Prop(_SC("End"), &RxMatch::GetEnd)
       // Member Methods
       .Func(_SC("SubStr"), &RxMatch::SubStr)
    );
    RootTable(vm).Bind(SqRxMatchesTypename::Str,
       Class< RxMatches >(vm, SqRxMatchesTypename::Str)
       // Constructors
       .Ctor()
       // Meta-methods
       .SquirrelFunc(_SC("_typename"), &SqRxMatchesTypename::Fn)
       // Properties
       .Prop(_SC("Front"), &RxMatches::Front)
       .Prop(_SC("Back"), &RxMatches::Back)
       .Prop(_SC("Empty"), &RxMatches::Empty)
       .Prop(_SC("Size"), &RxMatches::Size)
       .Prop(_SC("Capacity"), &RxMatches::Capacity, &RxMatches::Reserve)
       // Member Methods
       .Func(_SC("Get"), &RxMatches::Get)
       .Func(_SC("Reserve"), &RxMatches::Reserve)
       .Func(_SC("Compact"), &RxMatches::Compact)
       .Func(_SC("Clear"), &RxMatches::Clear)
       .Func(_SC("Pop"), &RxMatches::Pop)
       .Func(_SC("EraseAt"), &RxMatches::EraseAt)
       .Func(_SC("EraseFrom"), &RxMatches::EraseFrom)
       .Func(_SC("Each"), &RxMatches::Each)
       .Func(_SC("EachRange"), &RxMatches::EachRange)
       .Func(_SC("While"), &RxMatches::While)
       .Func(_SC("WhileRange"), &RxMatches::WhileRange)
       .Func(_SC("SubStr"), &RxMatches::SubStr)
    );
    RootTable(vm).Bind(_SC("SqRx"),
        Class< RxInstance, NoCopy< RxInstance > >(vm, SqRxInstanceTypename::Str)
        // Constructors
        .Ctor()
        .Ctor< StackStrF & >()
        .Ctor< int, StackStrF & >()
        .Ctor< int, bool, StackStrF & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqRxInstanceTypename::Fn)
        //.Func(_SC("_tostring"), &CPlayer::ToString)
        // Static Values
        .SetStaticValue(_SC("STUDY"), RxInstance::STUDY)
        .SetStaticValue(_SC("OPTIONS"), RxInstance::OPTIONS)
        .SetStaticValue(_SC("STUDY_OPTIONS"), RxInstance::STUDY_OPTIONS)
        // Properties
        .Prop(_SC("Valid"), &RxInstance::IsValid)
        .Prop(_SC("Studied"), &RxInstance::IsStudied)
        // Member Methods
        .FmtFunc(_SC("CompileF"), &RxInstance::Compile1)
        .FmtFunc(_SC("CompileExF"), &RxInstance::Compile2)
        .FmtFunc(_SC("TryCompileF"), &RxInstance::TryCompile1)
        .FmtFunc(_SC("TryCompileExF"), &RxInstance::TryCompile2)
        .FmtFunc(_SC("MatchFirst"), &RxInstance::MatchFirst)
        .FmtFunc(_SC("MatchFirstEx"), &RxInstance::MatchFirst_)
        .FmtFunc(_SC("MatchFirstFrom"), &RxInstance::MatchFirstFrom)
        .FmtFunc(_SC("MatchFirstFromEx"), &RxInstance::MatchFirstFrom_)
        .FmtFunc(_SC("Match"), &RxInstance::Match)
        .FmtFunc(_SC("MatchEx"), &RxInstance::Match_)
        .FmtFunc(_SC("MatchFrom"), &RxInstance::MatchFrom)
        .FmtFunc(_SC("MatchFromEx"), &RxInstance::MatchFrom_)
        .FmtFunc(_SC("Matches"), &RxInstance::Matches)
        .FmtFunc(_SC("MatchesEx"), &RxInstance::Matches_)
        .FmtFunc(_SC("MatchesEx2"), &RxInstance::MatchesEx)
        // Member Overloads
        .Overload(_SC("Compile"), &RxInstance::Compile1)
        .Overload(_SC("Compile"), &RxInstance::Compile2)
        .Overload(_SC("TryCompile"), &RxInstance::TryCompile1)
        .Overload(_SC("TryCompile"), &RxInstance::TryCompile2)
        .Overload(_SC("Study"), &RxInstance::Study0)
        .Overload(_SC("Study"), &RxInstance::Study1)
    );
    // --------------------------------------------------------------------------------------------
    ConstTable(vm).Enum(_SC("SqRxOption"), Enumeration(vm)
        .Const(_SC("Caseless"),         static_cast< SQInteger >(PCRE_CASELESS))
        .Const(_SC("Multiline"),        static_cast< SQInteger >(PCRE_MULTILINE))
        .Const(_SC("Dotall"),           static_cast< SQInteger >(PCRE_DOTALL))
        .Const(_SC("Extended"),         static_cast< SQInteger >(PCRE_EXTENDED))
        .Const(_SC("Anchored"),         static_cast< SQInteger >(PCRE_ANCHORED))
        .Const(_SC("DollarEndOnly"),    static_cast< SQInteger >(PCRE_DOLLAR_ENDONLY))
        .Const(_SC("Extra"),            static_cast< SQInteger >(PCRE_EXTRA))
        .Const(_SC("NotBOL"),           static_cast< SQInteger >(PCRE_NOTBOL))
        .Const(_SC("NotEOL"),           static_cast< SQInteger >(PCRE_NOTEOL))
        .Const(_SC("UnGreedy"),         static_cast< SQInteger >(PCRE_UNGREEDY))
        .Const(_SC("NotEmpty"),         static_cast< SQInteger >(PCRE_NOTEMPTY))
        .Const(_SC("UTF8"),             static_cast< SQInteger >(PCRE_UTF8))
        .Const(_SC("UTF16"),            static_cast< SQInteger >(PCRE_UTF16))
        .Const(_SC("UTF32"),            static_cast< SQInteger >(PCRE_UTF32))
        .Const(_SC("NoAutoCapture"),    static_cast< SQInteger >(PCRE_NO_AUTO_CAPTURE))
        .Const(_SC("NoUTF8Check"),      static_cast< SQInteger >(PCRE_NO_UTF8_CHECK))
        .Const(_SC("NoUTF16Check"),     static_cast< SQInteger >(PCRE_NO_UTF16_CHECK))
        .Const(_SC("NoUTF32Check"),     static_cast< SQInteger >(PCRE_NO_UTF32_CHECK))
        .Const(_SC("AutoCallout"),      static_cast< SQInteger >(PCRE_AUTO_CALLOUT))
        .Const(_SC("PartialSoft"),      static_cast< SQInteger >(PCRE_PARTIAL_SOFT))
        .Const(_SC("Partial"),          static_cast< SQInteger >(PCRE_PARTIAL))
        .Const(_SC("NeverUTF"),         static_cast< SQInteger >(PCRE_NEVER_UTF))
        .Const(_SC("DfaShortest"),      static_cast< SQInteger >(PCRE_DFA_SHORTEST))
        .Const(_SC("NoAutoPossess"),    static_cast< SQInteger >(PCRE_NO_AUTO_POSSESS))
        .Const(_SC("DfaRestart"),       static_cast< SQInteger >(PCRE_DFA_RESTART))
        .Const(_SC("FirstLine"),        static_cast< SQInteger >(PCRE_FIRSTLINE))
        .Const(_SC("DupNames"),         static_cast< SQInteger >(PCRE_DUPNAMES))
        .Const(_SC("NewLineCR"),        static_cast< SQInteger >(PCRE_NEWLINE_CR))
        .Const(_SC("NewLineLF"),        static_cast< SQInteger >(PCRE_NEWLINE_LF))
        .Const(_SC("NewLineCRLF"),      static_cast< SQInteger >(PCRE_NEWLINE_CRLF))
        .Const(_SC("NewLineAny"),       static_cast< SQInteger >(PCRE_NEWLINE_ANY))
        .Const(_SC("NewLineAnyCRLF"),   static_cast< SQInteger >(PCRE_NEWLINE_ANYCRLF))
        .Const(_SC("BsrAnyCRLF"),       static_cast< SQInteger >(PCRE_BSR_ANYCRLF))
        .Const(_SC("BsrUnicode"),       static_cast< SQInteger >(PCRE_BSR_UNICODE))
        .Const(_SC("JavaScriptCompat"), static_cast< SQInteger >(PCRE_JAVASCRIPT_COMPAT))
        .Const(_SC("NoStartOptimize"),  static_cast< SQInteger >(PCRE_NO_START_OPTIMIZE))
        .Const(_SC("NoStartOptimise"),  static_cast< SQInteger >(PCRE_NO_START_OPTIMISE))
        .Const(_SC("PartialHard"),      static_cast< SQInteger >(PCRE_PARTIAL_HARD))
        .Const(_SC("NotEmptyAtStart"),  static_cast< SQInteger >(PCRE_NOTEMPTY_ATSTART))
        .Const(_SC("UCP"),              static_cast< SQInteger >(PCRE_UCP))
    );
    // --------------------------------------------------------------------------------------------
    ConstTable(vm).Enum(_SC("SqRxError"), Enumeration(vm)
        .Const(_SC("NoMatch"),       	static_cast< SQInteger >(PCRE_ERROR_NOMATCH))
        .Const(_SC("Null"),       		static_cast< SQInteger >(PCRE_ERROR_NULL))
        .Const(_SC("BadOption"),     	static_cast< SQInteger >(PCRE_ERROR_BADOPTION))
        .Const(_SC("BadMagic"),      	static_cast< SQInteger >(PCRE_ERROR_BADMAGIC))
        .Const(_SC("UnknownOpCode"), 	static_cast< SQInteger >(PCRE_ERROR_UNKNOWN_OPCODE))
        .Const(_SC("UnknownNode"),   	static_cast< SQInteger >(PCRE_ERROR_UNKNOWN_NODE))
        .Const(_SC("NoMemory"),      	static_cast< SQInteger >(PCRE_ERROR_NOMEMORY))
        .Const(_SC("NoSubstring"),   	static_cast< SQInteger >(PCRE_ERROR_NOSUBSTRING))
        .Const(_SC("MatchLimit"),    	static_cast< SQInteger >(PCRE_ERROR_MATCHLIMIT))
        .Const(_SC("Callout"),       	static_cast< SQInteger >(PCRE_ERROR_CALLOUT))
        .Const(_SC("BadUTF8"),       	static_cast< SQInteger >(PCRE_ERROR_BADUTF8))
        .Const(_SC("BadUTF16"),      	static_cast< SQInteger >(PCRE_ERROR_BADUTF16))
        .Const(_SC("BadUTF32"),      	static_cast< SQInteger >(PCRE_ERROR_BADUTF32))
        .Const(_SC("BadUTF8Offset"), 	static_cast< SQInteger >(PCRE_ERROR_BADUTF8_OFFSET))
        .Const(_SC("BadUTF16Offset"),	static_cast< SQInteger >(PCRE_ERROR_BADUTF16_OFFSET))
        .Const(_SC("Partial"),       	static_cast< SQInteger >(PCRE_ERROR_PARTIAL))
        .Const(_SC("BadPartial"),    	static_cast< SQInteger >(PCRE_ERROR_BADPARTIAL))
        .Const(_SC("Internal"),      	static_cast< SQInteger >(PCRE_ERROR_INTERNAL))
        .Const(_SC("BadCount"),      	static_cast< SQInteger >(PCRE_ERROR_BADCOUNT))
        .Const(_SC("DfaUitem"),      	static_cast< SQInteger >(PCRE_ERROR_DFA_UITEM))
        .Const(_SC("DfaUcond"),      	static_cast< SQInteger >(PCRE_ERROR_DFA_UCOND))
        .Const(_SC("DfaUmLimit"),    	static_cast< SQInteger >(PCRE_ERROR_DFA_UMLIMIT))
        .Const(_SC("DfaWsSize"),     	static_cast< SQInteger >(PCRE_ERROR_DFA_WSSIZE))
        .Const(_SC("DfaRecurse"),    	static_cast< SQInteger >(PCRE_ERROR_DFA_RECURSE))
        .Const(_SC("RecursionLimit"),	static_cast< SQInteger >(PCRE_ERROR_RECURSIONLIMIT))
        .Const(_SC("NullWsLimit"),   	static_cast< SQInteger >(PCRE_ERROR_NULLWSLIMIT))
        .Const(_SC("BadNewLine"),    	static_cast< SQInteger >(PCRE_ERROR_BADNEWLINE))
        .Const(_SC("BadOffset"),     	static_cast< SQInteger >(PCRE_ERROR_BADOFFSET))
        .Const(_SC("ShortUTF8"),     	static_cast< SQInteger >(PCRE_ERROR_SHORTUTF8))
        .Const(_SC("ShortUTF16"),    	static_cast< SQInteger >(PCRE_ERROR_SHORTUTF16))
        .Const(_SC("RecurseLoop"),   	static_cast< SQInteger >(PCRE_ERROR_RECURSELOOP))
        .Const(_SC("JitStackLimit"), 	static_cast< SQInteger >(PCRE_ERROR_JIT_STACKLIMIT))
        .Const(_SC("BadMode"),       	static_cast< SQInteger >(PCRE_ERROR_BADMODE))
        .Const(_SC("BadEndianness"), 	static_cast< SQInteger >(PCRE_ERROR_BADENDIANNESS))
        .Const(_SC("DfaBadRestart"), 	static_cast< SQInteger >(PCRE_ERROR_DFA_BADRESTART))
        .Const(_SC("JitBadOption"),  	static_cast< SQInteger >(PCRE_ERROR_JIT_BADOPTION))
        .Const(_SC("BadLength"),     	static_cast< SQInteger >(PCRE_ERROR_BADLENGTH))
        .Const(_SC("Unset"),       		static_cast< SQInteger >(PCRE_ERROR_UNSET))
    );
}

} // Namespace:: SqMod
