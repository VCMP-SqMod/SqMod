// ------------------------------------------------------------------------------------------------
#include "PocoLib/RegEx.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqratConst.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(SqRegEx, _SC("SqRegEx"))
SQMOD_DECL_TYPENAME(SqRegExMatch, _SC("SqRegExMatch"))
SQMOD_DECL_TYPENAME(SqRegExMatches, _SC("SqRegExMatches"))

// ------------------------------------------------------------------------------------------------


// ================================================================================================
void Register_POCO_RegEx(HSQUIRRELVM vm, Table & ns)
{
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("RegExMatch"),
        Class< PcRegExMatch >(vm, SqRegExMatch::Str)
        // Constructors
        .Ctor()
        .Ctor< SQInteger >()
        .Ctor< SQInteger, SQInteger >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqRegExMatch::Fn)
        // Properties
        .Prop(_SC("Offset"), &PcRegExMatch::GetOffset, &PcRegExMatch::SetOffset)
        .Prop(_SC("Length"), &PcRegExMatch::GetLength, &PcRegExMatch::SetLength)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("RegExMatches"),
        Class< PcRegExMatches >(vm, SqRegExMatches::Str)
        // Constructors
        .Ctor()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqRegExMatches::Fn)
        // Properties
        .Prop(_SC("Front"), &PcRegExMatches::Front)
        .Prop(_SC("Back"), &PcRegExMatches::Back)
        .Prop(_SC("Empty"), &PcRegExMatches::Empty)
        .Prop(_SC("Size"), &PcRegExMatches::Size)
        .Prop(_SC("Capacity"), &PcRegExMatches::Capacity, &PcRegExMatches::Reserve)
        // Member Methods
        .Func(_SC("Get"), &PcRegExMatches::Get)
        .Func(_SC("Reserve"), &PcRegExMatches::Reserve)
        .Func(_SC("Compact"), &PcRegExMatches::Compact)
        .Func(_SC("Clear"), &PcRegExMatches::Clear)
        .Func(_SC("Pop"), &PcRegExMatches::Pop)
        .Func(_SC("EraseAt"), &PcRegExMatches::EraseAt)
        .Func(_SC("EraseFrom"), &PcRegExMatches::EraseFrom)
        .Func(_SC("Each"), &PcRegExMatches::Each)
        .Func(_SC("EachRange"), &PcRegExMatches::EachRange)
        .Func(_SC("While"), &PcRegExMatches::While)
        .Func(_SC("WhileRange"), &PcRegExMatches::WhileRange)
    );
    // --------------------------------------------------------------------------------------------
    ns.Bind(_SC("RegEx"),
        Class< PcRegEx, NoCopy< PcRegEx > >(vm, SqRegEx::Str)
        // Constructors
        .Ctor< StackStrF & >()
        .Ctor< int, StackStrF & >()
        .Ctor< int, bool, StackStrF & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SqRegEx::Fn)
        // Member Methods
        //.Func(_SC("Assign"), &PcRegEx::assign)
        // Overloaded Member Methods
        .Overload(_SC("MatchFirst"), &PcRegEx::MatchFirst)
        .Overload(_SC("MatchFirst"), &PcRegEx::MatchFirst_)
        .Overload(_SC("MatchFirstFrom"), &PcRegEx::MatchFirstFrom)
        .Overload(_SC("MatchFirstFrom"), &PcRegEx::MatchFirstFrom_)
        .Overload(_SC("Match"), &PcRegEx::Match)
        .Overload(_SC("Match"), &PcRegEx::Match_)
        .Overload(_SC("MatchFrom"), &PcRegEx::MatchFrom)
        .Overload(_SC("MatchFrom"), &PcRegEx::MatchFrom_)
        .Overload(_SC("Matches"), &PcRegEx::Matches)
        .Overload(_SC("Matches"), &PcRegEx::Matches_)
        .Overload(_SC("Matches"), &PcRegEx::MatchesEx)
    );
    // --------------------------------------------------------------------------------------------
    ConstTable(vm).Enum(_SC("SqRegExOption"), Enumeration(vm)
        .Const(_SC("Caseless"),         static_cast< SQInteger >(Poco::RegularExpression::RE_CASELESS))
        .Const(_SC("MultiLine"),        static_cast< SQInteger >(Poco::RegularExpression::RE_MULTILINE))
        .Const(_SC("DotAll"),           static_cast< SQInteger >(Poco::RegularExpression::RE_DOTALL))
        .Const(_SC("Extended"),         static_cast< SQInteger >(Poco::RegularExpression::RE_EXTENDED))
        .Const(_SC("Anchored"),         static_cast< SQInteger >(Poco::RegularExpression::RE_ANCHORED))
        .Const(_SC("DollarEndOnly"),    static_cast< SQInteger >(Poco::RegularExpression::RE_DOLLAR_ENDONLY))
        .Const(_SC("Extra"),            static_cast< SQInteger >(Poco::RegularExpression::RE_EXTRA))
        .Const(_SC("NotBOL"),           static_cast< SQInteger >(Poco::RegularExpression::RE_NOTBOL))
        .Const(_SC("NotEOL"),           static_cast< SQInteger >(Poco::RegularExpression::RE_NOTEOL))
        .Const(_SC("Ungreedy"),         static_cast< SQInteger >(Poco::RegularExpression::RE_UNGREEDY))
        .Const(_SC("NotEmpty"),         static_cast< SQInteger >(Poco::RegularExpression::RE_NOTEMPTY))
        .Const(_SC("UTF8"),             static_cast< SQInteger >(Poco::RegularExpression::RE_UTF8))
        .Const(_SC("NoAutoCapture"),    static_cast< SQInteger >(Poco::RegularExpression::RE_NO_AUTO_CAPTURE))
        .Const(_SC("NoUTF8Check"),      static_cast< SQInteger >(Poco::RegularExpression::RE_NO_UTF8_CHECK))
        .Const(_SC("FirstLine"),        static_cast< SQInteger >(Poco::RegularExpression::RE_FIRSTLINE))
        .Const(_SC("DupNames"),         static_cast< SQInteger >(Poco::RegularExpression::RE_DUPNAMES))
        .Const(_SC("NewLineCR"),        static_cast< SQInteger >(Poco::RegularExpression::RE_NEWLINE_CR))
        .Const(_SC("NewLineLF"),        static_cast< SQInteger >(Poco::RegularExpression::RE_NEWLINE_LF))
        .Const(_SC("NewLineCRLF"),      static_cast< SQInteger >(Poco::RegularExpression::RE_NEWLINE_CRLF))
        .Const(_SC("NewLineAny"),       static_cast< SQInteger >(Poco::RegularExpression::RE_NEWLINE_ANY))
        .Const(_SC("NewLineAnyCRLF"),   static_cast< SQInteger >(Poco::RegularExpression::RE_NEWLINE_ANYCRLF))
        .Const(_SC("Global"),           static_cast< SQInteger >(Poco::RegularExpression::RE_GLOBAL))
        .Const(_SC("NoVars"),           static_cast< SQInteger >(Poco::RegularExpression::RE_NO_VARS))
    );
}

} // Namespace:: SqMod
