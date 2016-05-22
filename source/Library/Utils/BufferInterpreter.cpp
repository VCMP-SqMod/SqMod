// ------------------------------------------------------------------------------------------------
#include "Library/Utils/BufferInterpreter.hpp"
#include "Library/Utils/BufferWrapper.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
const SharedPtr< Buffer > & GetBufferBufferRef(const BufferWrapper & buffer)
{
    return buffer.GetRef();
}

// ------------------------------------------------------------------------------------------------
template < typename T > static void RegisterInterpreter(Table & bns, CSStr cname, CSStr bname)
{
    typedef BufferInterpreter< T > Interpreter;

    bns.Bind(bname,
        Class< Interpreter >(bns.GetVM(), cname)
        // Constructors
        .Ctor()
        // Properties
        .Prop(_SC("Front"), &Interpreter::GetFront, &Interpreter::SetFront)
        .Prop(_SC("Next"), &Interpreter::GetNext, &Interpreter::SetNext)
        .Prop(_SC("Back"), &Interpreter::GetBack, &Interpreter::SetBack)
        .Prop(_SC("Prev"), &Interpreter::GetPrev, &Interpreter::SetPrev)
        .Prop(_SC("Cursor"), &Interpreter::GetCursor, &Interpreter::SetCursor)
        .Prop(_SC("Before"), &Interpreter::GetBefore, &Interpreter::SetBefore)
        .Prop(_SC("After"), &Interpreter::GetAfter, &Interpreter::SetAfter)
        .Prop(_SC("Max"), &Interpreter::GetMax)
        .Prop(_SC("Size"), &Interpreter::GetSize)
        .Prop(_SC("Capacity"), &Interpreter::GetCapacity)
        .Prop(_SC("Position"), &Interpreter::GetPosition)
        .Prop(_SC("Remaining"), &Interpreter::GetRemaining)
        // Member Methods
        .Func(_SC("Use"), &Interpreter::UseBuffer)
        .Func(_SC("Get"), &Interpreter::Get)
        .Func(_SC("Set"), &Interpreter::Set)
        .Func(_SC("Advance"), &Interpreter::Advance)
        .Func(_SC("Retreat"), &Interpreter::Retreat)
        .Func(_SC("Push"), &Interpreter::Push)
        .Func(_SC("Grow"), &Interpreter::Grow)
        .Func(_SC("Adjust"), &Interpreter::Adjust)
    );
}

// ================================================================================================
void Register_BufferInterpreter(Table & bns)
{
    RegisterInterpreter< Int8 >(bns, _SC("SqBufferInterpreterS8"), _SC("S8Interpreter"));
    RegisterInterpreter< Uint8 >(bns, _SC("SqBufferInterpreterU8"), _SC("U8Interpreter"));
    RegisterInterpreter< Int16 >(bns, _SC("SqBufferInterpreterS16"), _SC("S16Interpreter"));
    RegisterInterpreter< Uint16 >(bns, _SC("SqBufferInterpreterU16"), _SC("U16Interpreter"));
    RegisterInterpreter< Int32 >(bns, _SC("SqBufferInterpreterS32"), _SC("S32Interpreter"));
    RegisterInterpreter< Uint32 >(bns, _SC("SqBufferInterpreterU32"), _SC("U32Interpreter"));
    RegisterInterpreter< Int64 >(bns, _SC("SqBufferInterpreterS64"), _SC("S64Interpreter"));
    RegisterInterpreter< Uint64 >(bns, _SC("SqBufferInterpreterU64"), _SC("U64Interpreter"));
    RegisterInterpreter< Float32 >(bns, _SC("SqBufferInterpreterF32"), _SC("F32Interpreter"));
    RegisterInterpreter< Float64 >(bns, _SC("SqBufferInterpreterF64"), _SC("F64Interpreter"));
}

} // Namespace:: SqMod
