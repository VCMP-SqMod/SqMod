// ------------------------------------------------------------------------------------------------
#include "Library/IO/Stream.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqratConst.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(StringStreamTn, _SC("SqStringStream"))
SQMOD_DECL_TYPENAME(FileStreamTn, _SC("SqFileStream"))
SQMOD_DECL_TYPENAME(OStringStreamTn, _SC("SqOStringStream"))
SQMOD_DECL_TYPENAME(OFileStreamTn, _SC("SqOFileStream"))
SQMOD_DECL_TYPENAME(IStringStreamTn, _SC("SqIStringStream"))
SQMOD_DECL_TYPENAME(IFileStreamTn, _SC("SqIFileStream"))

// ------------------------------------------------------------------------------------------------
SQMOD_DECL_TYPENAME(CInStreamTn, _SC("SqCInStream"))
SQMOD_DECL_TYPENAME(COutStreamTn, _SC("SqCOutStream"))
SQMOD_DECL_TYPENAME(CErrStreamTn, _SC("SqCErrStream"))
SQMOD_DECL_TYPENAME(CLogStreamTn, _SC("SqCLogStream"))

// ------------------------------------------------------------------------------------------------
static LightObj FileToString(StackStrF & name)
{
    // Open file
    std::ifstream t(name.mPtr);
    String str;
    // Pre-allocate buffer
    t.seekg(0, std::ios::end);   
    str.reserve(t.tellg());
    t.seekg(0, std::ios::beg);
    // Read contents
    str.assign((std::istreambuf_iterator< SQChar >(t)), std::istreambuf_iterator< SQChar >());
    // Transform to object
    return LightObj(str.data(), static_cast< SQInteger >(str.size()));
}

// ------------------------------------------------------------------------------------------------
static void StringToFile(StackStrF & name, StackStrF & str)
{
    // Open file
    std::ofstream out(name.mPtr);
    // Write contents
    out.write(str.mPtr, static_cast< std::streamsize >(str.mLen));
    // Close file
    out.close();
}

// ------------------------------------------------------------------------------------------------
using StringStream  = SqStream< std::stringstream >;
using FileStream    = SqStream< std::fstream >;
using OStringStream = SqStream< std::ostringstream >;
using OFileStream   = SqStream< std::ofstream >;
using IStringStream = SqStream< std::istringstream >;
using IFileStream   = SqStream< std::ifstream >;

// ------------------------------------------------------------------------------------------------
using CInStream  = SqStream< SqGlobalStream< CInTag > >;
using COutStream  = SqStream< SqGlobalStream< COutTag > >;
using CErrStream  = SqStream< SqGlobalStream< CErrTag > >;
using CLogStream  = SqStream< SqGlobalStream< CLogTag > >;

/* ================================================================================================
 * Utility used to reduce code duplication when registering streams.
*/
template < class T > struct RegisterStream
{
    /* --------------------------------------------------------------------------------------------
     * Class object to be registered.
    */
    Class< T, NoCopy< T > > mCls;
    /* --------------------------------------------------------------------------------------------
     * Base constructor.
    */
    RegisterStream(HSQUIRRELVM vm, const SQChar * tn)
        : mCls(vm, tn)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Base members.
    */
    template < class Tn > RegisterStream & Base()
    {
        mCls
            // Core Meta-methods
            .SquirrelFunc(_SC("_typename"), &Tn::Fn)
            // Properties
            .Prop(_SC("Flags"), &T::GetFlags, &T::SetFlags)
            .Prop(_SC("Width"), &T::GetWidth, &T::SetWidth)
            .Prop(_SC("Precision"), &T::GetPrecision, &T::SetPrecision)
            .Prop(_SC("Good"), &T::Good)
            .Prop(_SC("EOF"), &T::EOF_)
            .Prop(_SC("Fail"), &T::Fail)
            .Prop(_SC("Bad"), &T::Bad)
            .Prop(_SC("Fill"), &T::GetFill, &T::SetFill)
            .Prop(_SC("State"), &T::GetState, &T::SetState)
            .Prop(_SC("Exceptions"), &T::GetExceptions, &T::SetExceptions)
            // Member Methods
            .Func(_SC("UnSetF"), &T::UnSetF)
            .Func(_SC("SetWidth"), &T::SetWidth)
            .Func(_SC("SetPrecision"), &T::SetPrecision)
            .Func(_SC("SetFill"), &T::SetFill)
            .Func(_SC("AdjustBuffer"), &T::AdjustBuffer)
            .Func(_SC("ReclaimBuffer"), &T::ReclaimBuffer)
            // Member Overloads
            .Overload(_SC("SetF"), &T::SetF)
            .Overload(_SC("SetF"), &T::SetF_)
            .Overload(_SC("Clear"), &T::Clear)
            .Overload(_SC("Clear"), &T::ClearEx)
        ;
        // Allow chaining
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Output members.
    */
    RegisterStream & Output()
    {
        mCls
            // Member Methods
            .Func(_SC("TellP"), &T::TellP)
            .Func(_SC("Put"), &T::Put)
            .Func(_SC("Flush"), &T::Flush)
            .Func(_SC("Write"), &T::WriteString)
            .Func(_SC("WriteBuffer"), &T::WriteBuffer)
            // Member Overloads
            .Overload(_SC("SeekP"), &T::SeekP)
            .Overload(_SC("SeekP"), &T::SeekP_)
        ;
        // Allow chaining
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Input members.
    */
    RegisterStream & Input()
    {
        mCls
            // Properties
            .Prop(_SC("GCount"), &T::GCount)
            // Member Methods
            .Func(_SC("TellG"), &T::TellG)
            .Func(_SC("Peek"), &T::Peek)
            .Func(_SC("UnGet"), &T::UnGet)
            .Func(_SC("PutBack"), &T::PutBack)
            .Func(_SC("Read"), &T::ReadString)
            .Func(_SC("ReadBuffer"), &T::ReadBuffer)
            .Func(_SC("ReadSome"), &T::ReadSomeString)
            .Func(_SC("ReadSomeBuffer"), &T::ReadSomeBuffer)
            // Member Overloads
            .Overload(_SC("SeekG"), &T::SeekG)
            .Overload(_SC("SeekG"), &T::SeekG_)
            .Overload(_SC("Get"), &T::GetString0)
            .Overload(_SC("Get"), &T::GetString1)
            .Overload(_SC("Get"), &T::GetString2)
            .Overload(_SC("GetBuffer"), &T::GetBuffer1)
            .Overload(_SC("GetBuffer"), &T::GetBuffer2)
            .Overload(_SC("GetLine"), &T::GetLineString1)
            .Overload(_SC("GetLine"), &T::GetLineString2)
            .Overload(_SC("GetLineBuffer"), &T::GetLineBuffer1)
            .Overload(_SC("GetLineBuffer"), &T::GetLineBuffer2)
            .Overload(_SC("Ignore"), &T::Ignore0)
            .Overload(_SC("Ignore"), &T::Ignore1)
            .Overload(_SC("Ignore"), &T::Ignore2)
        ;
        // Allow chaining
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * String type members.
    */
    RegisterStream & StringType()
    {
        mCls
            // Constructors
            .template Ctor()
            .template Ctor< SQInteger >()
            // Properties
            .Prop(_SC("Str"), &T::GetStr, &T::SetStr)
        ;
        // Allow chaining
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * File type members.
    */
    RegisterStream & FileType()
    {
        mCls
            // Constructors
            .template Ctor< StackStrF & >()
            .template Ctor< SQInteger, StackStrF & >()
            // Properties
            .Prop(_SC("IsOpen"), &T::IsOpen)
            // Member Methods
            .Func(_SC("Close"), &T::Close)
            // Member Overloads
            .Overload(_SC("Open"), &T::Open)
            .Overload(_SC("Open"), &T::Open_)
        ;
        // Allow chaining
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Global stream type members.
    */
    RegisterStream & GlobalType()
    {
        mCls
            // Constructors
            .template Ctor()
        ;
        // Allow chaining
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Bind the class object to a table.
    */
    void Bind(Table & ns, const SQChar * name)
    {
        ns.Bind(name, mCls);
    }
};
// ================================================================================================
void Register_Stream(HSQUIRRELVM vm)
{
    Table sns(vm);
    // --------------------------------------------------------------------------------------------
    RegisterStream< StringStream >(vm, StringStreamTn::Str)
        .Base< StringStreamTn >()
        .Output()
        .Input()
        .StringType()
        .Bind(sns, "String");
    // --------------------------------------------------------------------------------------------
    RegisterStream< FileStream >(vm, FileStreamTn::Str)
        .Base< FileStreamTn >()
        .Output()
        .Input()
        .FileType()
        .Bind(sns, "File");
    // --------------------------------------------------------------------------------------------
    RegisterStream< OStringStream >(vm, OStringStreamTn::Str)
        .Base< OStringStreamTn >()
        .Output()
        .StringType()
        .Bind(sns, "OutputString");
    // --------------------------------------------------------------------------------------------
    RegisterStream< OFileStream >(vm, OFileStreamTn::Str)
        .Base< OFileStreamTn >()
        .Output()
        .FileType()
        .Bind(sns, "OutputFile");
    // --------------------------------------------------------------------------------------------
    RegisterStream< IStringStream >(vm, IStringStreamTn::Str)
        .Base< IStringStreamTn >()
        .Input()
        .StringType()
        .Bind(sns, "InputString");
    // --------------------------------------------------------------------------------------------
    RegisterStream< IFileStream >(vm, IFileStreamTn::Str)
        .Base< IFileStreamTn >()
        .Input()
        .FileType()
        .Bind(sns, "InputFile");
    // --------------------------------------------------------------------------------------------
    sns.FmtFunc(_SC("FileToString"), FileToString);
    sns.FmtFunc(_SC("StringToFile"), StringToFile);
    // --------------------------------------------------------------------------------------------
    /* This feature is implemented but not enabled because it's rather pointless */
    //RegisterStream< CInStream >(vm, CInStreamTn::Str)
    //    .Base< CInStreamTn >()
    //    .Input()
    //    .GlobalType()
    //    .Bind(sns, "CIn");
    // --------------------------------------------------------------------------------------------
    //RegisterStream< COutStream >(vm, COutStreamTn::Str)
    //    .Base< COutStreamTn >()
    //    .Output()
    //    .GlobalType()
    //    .Bind(sns, "COut");
    // --------------------------------------------------------------------------------------------
    //RegisterStream< CErrStream >(vm, CErrStreamTn::Str)
    //    .Base< CErrStreamTn >()
    //    .Output()
    //    .GlobalType()
    //    .Bind(sns, "CErr");
    // --------------------------------------------------------------------------------------------
    //RegisterStream< CLogStream >(vm, CLogStreamTn::Str)
    //    .Base< CLogStreamTn >()
    //    .Output()
    //    .GlobalType()
    //    .Bind(sns, "CLog");
    // --------------------------------------------------------------------------------------------
    RootTable(vm).Bind(_SC("SqStream"), sns);
    // --------------------------------------------------------------------------------------------
    ConstTable(vm).Enum(_SC("SqIoState"), Enumeration(vm)
        .Const(_SC("GoodBit"),                      SQInteger(std::ios_base::goodbit))
        .Const(_SC("BadBit"),                       SQInteger(std::ios_base::badbit))
        .Const(_SC("FailBit"),                      SQInteger(std::ios_base::failbit))
        .Const(_SC("EofBit"),                       SQInteger(std::ios_base::eofbit))
    );
    // --------------------------------------------------------------------------------------------
    ConstTable(vm).Enum(_SC("SqFmtFlags"), Enumeration(vm)
        .Const(_SC("Dec"),                          SQInteger(std::ios_base::dec))
        .Const(_SC("Oct"),                          SQInteger(std::ios_base::oct))
        .Const(_SC("Hex"),                          SQInteger(std::ios_base::hex))
        .Const(_SC("BaseField"),                    SQInteger(std::ios_base::basefield))
        .Const(_SC("Left"),                         SQInteger(std::ios_base::left))
        .Const(_SC("Right"),                        SQInteger(std::ios_base::right))
        .Const(_SC("Internal"),                     SQInteger(std::ios_base::internal))
        .Const(_SC("AdjustField"),                  SQInteger(std::ios_base::adjustfield))
        .Const(_SC("Scientific"),                   SQInteger(std::ios_base::scientific))
        .Const(_SC("Fixed"),                        SQInteger(std::ios_base::fixed))
        .Const(_SC("FloatField"),                   SQInteger(std::ios_base::floatfield))
        .Const(_SC("BoolAlpha"),                    SQInteger(std::ios_base::boolalpha))
        .Const(_SC("ShowBase"),                     SQInteger(std::ios_base::showbase))
        .Const(_SC("ShowPoint"),                    SQInteger(std::ios_base::showpoint))
        .Const(_SC("ShowPos"),                      SQInteger(std::ios_base::showpos))
        .Const(_SC("SkipWS"),                       SQInteger(std::ios_base::skipws))
        .Const(_SC("UnitBuf"),                      SQInteger(std::ios_base::unitbuf))
        .Const(_SC("UpperCase"),                    SQInteger(std::ios_base::uppercase))
    );
    // --------------------------------------------------------------------------------------------
    ConstTable(vm).Enum(_SC("SqOpenMode"), Enumeration(vm)
        .Const(_SC("App"),                          SQInteger(std::ios_base::app))
        .Const(_SC("Ate"),                          SQInteger(std::ios_base::ate))
        .Const(_SC("AtEnd"),                        SQInteger(std::ios_base::ate))
        .Const(_SC("Binary"),                       SQInteger(std::ios_base::binary))
        .Const(_SC("In"),                           SQInteger(std::ios_base::in))
        .Const(_SC("Out"),                          SQInteger(std::ios_base::out))
        .Const(_SC("Trunc"),                        SQInteger(std::ios_base::trunc))
    );
    // --------------------------------------------------------------------------------------------
    ConstTable(vm).Enum(_SC("SqSeekDir"), Enumeration(vm)
        .Const(_SC("Beg"),                          SQInteger(std::ios_base::beg))
        .Const(_SC("Beginning"),                    SQInteger(std::ios_base::beg))
        .Const(_SC("Cur"),                          SQInteger(std::ios_base::cur))
        .Const(_SC("Current"),                      SQInteger(std::ios_base::cur))
        .Const(_SC("End"),                          SQInteger(std::ios_base::end))
    );
}

} // Namespace:: SqMod
