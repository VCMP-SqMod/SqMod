// ------------------------------------------------------------------------------------------------
#include "Library/Utils/BufferWrapper.hpp"
#include "Library/Utils/BufferInterpreter.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstring>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
extern void Register_BufferInterpreter(Table & bns);

// ------------------------------------------------------------------------------------------------
Object CreateBufferWrapperCopy(const Buffer & b)
{
    // Attempt to create the requested buffer
    try
    {
        // Remember the current stack size
        const StackGuard sg;
        // Transform the pointer into a script object
        PushVar< const BufferWrapper & >(DefaultVM::Get(), BufferWrapper(b));
        // Get the object from the stack and return it
        return Var< Object >(DefaultVM::Get(), -1).value;
    }
    catch (const Sqrat::Exception & e)
    {
        throw e; // Re-throw
    }
    catch (const std::exception & e)
    {
        STHROWF("%s", e.what()); // Re-package
    }
    // Shouldn't really reach this point
    return NullObject();
}

// ------------------------------------------------------------------------------------------------
Object CreateBufferWrapperMove(Buffer && b)
{
    // Attempt to create the requested buffer
    try
    {
        // Remember the current stack size
        const StackGuard sg;
        // Transform the pointer into a script object
        PushVar< const BufferWrapper & >(DefaultVM::Get(), BufferWrapper(std::move(b)));
        // Get the object from the stack and return it
        return Var< Object >(DefaultVM::Get(), -1).value;
    }
    catch (const Sqrat::Exception & e)
    {
        throw e; // Re-throw
    }
    catch (const std::exception & e)
    {
        STHROWF("%s", e.what()); // Re-package
    }
    // Shouldn't really reach this point
    return NullObject();
}

// ------------------------------------------------------------------------------------------------
Object BufferWrapper::Create(SzType n)
{
    // Attempt to create the requested buffer
    try
    {
        return MakeObject(BufferWrapper(SRef(new Buffer(n))));
    }
    catch (const Sqrat::Exception & e)
    {
        throw e; // Re-throw
    }
    catch (const std::exception & e)
    {
        STHROWF("%s", e.what()); // Re-package
    }
    // Shouldn't really reach this point
    return NullObject();
}

// ------------------------------------------------------------------------------------------------
void BufferWrapper::WriteByte(SQInteger val)
{
    // Validate the managed buffer reference
    Validate();
    // Perform the requested operation
    m_Buffer->Push< Uint8 >(ConvTo< Uint8 >::From(val));
}

// ------------------------------------------------------------------------------------------------
void BufferWrapper::WriteShort(SQInteger val)
{
    // Validate the managed buffer reference
    Validate();
    // Perform the requested operation
    m_Buffer->Push< Int16 >(ConvTo< Int16 >::From(val));
}

// ------------------------------------------------------------------------------------------------
void BufferWrapper::WriteInt(SQInteger val)
{
    // Validate the managed buffer reference
    Validate();
    // Perform the requested operation
    m_Buffer->Push< Int32 >(ConvTo< Int32 >::From(val));
}

// ------------------------------------------------------------------------------------------------
void BufferWrapper::WriteFloat(SQFloat val)
{
    // Validate the managed buffer reference
    Validate();
    // Perform the requested operation
    m_Buffer->Push< Float32 >(ConvTo< Float32 >::From(val));
}

// ------------------------------------------------------------------------------------------------
void BufferWrapper::WriteString(CSStr val)
{
    // Validate the managed buffer reference
    Validate();
    // Is the given string value even valid?
    if (!val)
    {
        STHROWF("Invalid string argument: null");
    }
    // Calculate the string length
    Uint16 length = ConvTo< Uint16 >::From(std::strlen(val));
    // Change the size endianness to big endian
    Uint16 size = ((length >> 8) & 0xFF) | ((length & 0xFF) << 8);
    // Write the size and then the string contents
    m_Buffer->Push< Uint16 >(size);
    m_Buffer->AppendS(val, length);
}

// ------------------------------------------------------------------------------------------------
void BufferWrapper::WriteRawString(CSStr val)
{
    // Validate the managed buffer reference
    Validate();
    // Is the given string value even valid?
    if (!val)
    {
        STHROWF("Invalid string argument: null");
    }
    // Calculate the string length
    Uint16 length = ConvTo< Uint16 >::From(std::strlen(val));
    // Write the the string contents
    m_Buffer->AppendS(val, length);
}

// ------------------------------------------------------------------------------------------------
SQInteger BufferWrapper::ReadByte()
{
    // Validate the managed buffer reference
    ValidateDeeper();
    // Are we out of the memory buffer range?
    if (m_Buffer->Position< Int8 >() >= m_Buffer->Size< Int8 >())
    {
        STHROWF("Value size (%u starting at %u) is out of bounds (%u)",
                    sizeof(Int8), m_Buffer->Position(), m_Buffer->Capacity());
    }
    // Read one element from the buffer
    const Int8 value = m_Buffer->Cursor< Int8 >();
    // Advance the buffer cursor
    m_Buffer->Advance< Int8 >(1);
    // Return the requested information
    return value;
}

// ------------------------------------------------------------------------------------------------
SQInteger BufferWrapper::ReadShort()
{
    // Validate the managed buffer reference
    ValidateDeeper();
    // Are we out of the memory buffer range?
    if (m_Buffer->Position< Int16 >() >= m_Buffer->Size< Int16 >())
    {
        STHROWF("Value size (%u starting at %u) is out of bounds (%u)",
                    sizeof(Int16), m_Buffer->Position(), m_Buffer->Capacity());
    }
    // Read one element from the buffer
    const Int16 value = m_Buffer->Cursor< Int16 >();
    // Advance the buffer cursor
    m_Buffer->Advance< Int16 >(1);
    // Return the requested information
    return value;
}

// ------------------------------------------------------------------------------------------------
SQInteger BufferWrapper::ReadInt()
{
    // Validate the managed buffer reference
    ValidateDeeper();
    // Are we out of the memory buffer range?
    if (m_Buffer->Position< Int32 >() >= m_Buffer->Size< Int32 >())
    {
        STHROWF("Value size (%u starting at %u) is out of bounds (%u)",
                    sizeof(Int32), m_Buffer->Position(), m_Buffer->Capacity());
    }
    // Read one element from the buffer
    const Int32 value = m_Buffer->Cursor< Int32 >();
    // Advance the buffer cursor
    m_Buffer->Advance< Int32 >(1);
    // Return the requested information
    return value;
}

// ------------------------------------------------------------------------------------------------
SQFloat BufferWrapper::ReadFloat()
{
    // Validate the managed buffer reference
    ValidateDeeper();
    // Are we out of the memory buffer range?
    if (m_Buffer->Position< Float32 >() >= m_Buffer->Size< Float32 >())
    {
        STHROWF("Value size (%u starting at %u) is out of bounds (%u)",
                    sizeof(Float32), m_Buffer->Position(), m_Buffer->Capacity());
    }
    // Read one element from the buffer
    const Float32 value = m_Buffer->Cursor< Float32 >();
    // Advance the buffer cursor
    m_Buffer->Advance< Float32 >(1);
    // Return the requested information
    return value;
}

// ------------------------------------------------------------------------------------------------
Object BufferWrapper::ReadString()
{
    // Validate the managed buffer reference
    ValidateDeeper();
    // Are we out of the memory buffer range?
    if (m_Buffer->Position< Int16 >() >= m_Buffer->Size< Int16 >())
    {
        STHROWF("Value size (%u starting at %u) is out of bounds (%u)",
                    sizeof(Int16), m_Buffer->Position(), m_Buffer->Capacity());
    }
    // Read one element from the buffer
    Int16 length = m_Buffer->Cursor< Int16 >();
    // Convert the length to little endian
    length = ((length >> 8) & 0xFF) | ((length & 0xFF) << 8);
    // Validate the obtained length
    if ((m_Buffer->Position() + sizeof(Int16) + length) >= m_Buffer->Size())
    {
        STHROWF("String size (%u starting at %u) is out of bounds (%u)",
                    length, m_Buffer->Position() + sizeof(Int16), m_Buffer->Capacity());
    }
    // Advance the buffer to the actual string
    m_Buffer->Advance< Int16 >(1);
    // Remember the current stack size
    const StackGuard sg;
    // Attempt to create the string as an object
    sq_pushstring(DefaultVM::Get(), &m_Buffer->Cursor(), length);
    // Advance the cursor after the string
    m_Buffer->Advance(length);
    // Return the resulted object
    return Var< Object >(DefaultVM::Get(), -1).value;
}

// ------------------------------------------------------------------------------------------------
Object BufferWrapper::ReadRawString(Uint32 len)
{
    // Validate the managed buffer reference
    ValidateDeeper();
    // Validate the obtained length
    if ((m_Buffer->Position() + len) >= m_Buffer->Size())
    {
        STHROWF("String size (%u starting at %u) is out of bounds (%u)",
                    len, m_Buffer->Position(), m_Buffer->Capacity());
    }
    // Remember the current stack size
    const StackGuard sg;
    // Attempt to create the string as an object
    sq_pushstring(DefaultVM::Get(), &m_Buffer->Cursor(), len);
    // Advance the cursor after the string
    m_Buffer->Advance(len);
    // Return the resulted object
    return Var< Object >(DefaultVM::Get(), -1).value;
}

// ------------------------------------------------------------------------------------------------
BufferInterpreter< Int8 > BufferWrapper::GetInt8Interpreter() const
{
    return BufferInterpreter< Int8 >(m_Buffer);
}

// ------------------------------------------------------------------------------------------------
BufferInterpreter< Uint8 > BufferWrapper::GetUint8Interpreter() const
{
    return BufferInterpreter< Uint8 >(m_Buffer);
}

// ------------------------------------------------------------------------------------------------
BufferInterpreter< Int16 > BufferWrapper::GetInt16Interpreter() const
{
    return BufferInterpreter< Int16 >(m_Buffer);
}

// ------------------------------------------------------------------------------------------------
BufferInterpreter< Uint16 > BufferWrapper::GetUint16Interpreter() const
{
    return BufferInterpreter< Uint16 >(m_Buffer);
}

// ------------------------------------------------------------------------------------------------
BufferInterpreter< Int32 > BufferWrapper::GetInt32Interpreter() const
{
    return BufferInterpreter< Int32 >(m_Buffer);
}

// ------------------------------------------------------------------------------------------------
BufferInterpreter< Uint32 > BufferWrapper::GetUint32Interpreter() const
{
    return BufferInterpreter< Uint32 >(m_Buffer);
}

// ------------------------------------------------------------------------------------------------
BufferInterpreter< Int64 > BufferWrapper::GetInt64Interpreter() const
{
    return BufferInterpreter< Int64 >(m_Buffer);
}

// ------------------------------------------------------------------------------------------------
BufferInterpreter< Uint64 > BufferWrapper::GetUint64Interpreter() const
{
    return BufferInterpreter< Uint64 >(m_Buffer);
}

// ------------------------------------------------------------------------------------------------
BufferInterpreter< Float32 > BufferWrapper::GetFloat32Interpreter() const
{
    return BufferInterpreter< Float32 >(m_Buffer);
}

// ------------------------------------------------------------------------------------------------
BufferInterpreter< Float64 > BufferWrapper::GetFloat64Interpreter() const
{
    return BufferInterpreter< Float64 >(m_Buffer);
}

// ================================================================================================
void Register_Buffer(HSQUIRRELVM vm)
{
    Table bns(vm);

    bns.Bind(_SC("Wrapper"),
        Class< BufferWrapper >(vm, _SC("SqBufferWrapper"))
        // Constructors
        .Ctor()
        .Ctor< const BufferWrapper & >()
        // Properties
        .Prop(_SC("Front"), &BufferWrapper::GetFront, &BufferWrapper::SetFront)
        .Prop(_SC("Next"), &BufferWrapper::GetNext, &BufferWrapper::SetNext)
        .Prop(_SC("Back"), &BufferWrapper::GetBack, &BufferWrapper::SetBack)
        .Prop(_SC("Prev"), &BufferWrapper::GetPrev, &BufferWrapper::SetPrev)
        .Prop(_SC("Cursor"), &BufferWrapper::GetCursor, &BufferWrapper::SetCursor)
        .Prop(_SC("Before"), &BufferWrapper::GetBefore, &BufferWrapper::SetBefore)
        .Prop(_SC("After"), &BufferWrapper::GetAfter, &BufferWrapper::SetAfter)
        .Prop(_SC("Max"), &BufferWrapper::GetMax)
        .Prop(_SC("Size"), &BufferWrapper::GetSize)
        .Prop(_SC("Capacity"), &BufferWrapper::GetCapacity)
        .Prop(_SC("Position"), &BufferWrapper::GetPosition)
        .Prop(_SC("Remaining"), &BufferWrapper::GetRemaining)
        .Prop(_SC("Int8"), &BufferWrapper::GetInt8Interpreter)
        .Prop(_SC("Uint8"), &BufferWrapper::GetUint8Interpreter)
        .Prop(_SC("Int16"), &BufferWrapper::GetInt16Interpreter)
        .Prop(_SC("Uint16"), &BufferWrapper::GetUint16Interpreter)
        .Prop(_SC("Int32"), &BufferWrapper::GetInt32Interpreter)
        .Prop(_SC("Uint32"), &BufferWrapper::GetUint32Interpreter)
        .Prop(_SC("Int64"), &BufferWrapper::GetInt64Interpreter)
        .Prop(_SC("Uint64"), &BufferWrapper::GetUint64Interpreter)
        .Prop(_SC("Float32"), &BufferWrapper::GetFloat32Interpreter)
        .Prop(_SC("Float64"), &BufferWrapper::GetFloat64Interpreter)
        // Member Methods
        .Func(_SC("Get"), &BufferWrapper::Get)
        .Func(_SC("Set"), &BufferWrapper::Set)
        .Func(_SC("Advance"), &BufferWrapper::Advance)
        .Func(_SC("Retreat"), &BufferWrapper::Retreat)
        .Func(_SC("Push"), &BufferWrapper::Push)
        .Func(_SC("Grow"), &BufferWrapper::Grow)
        .Func(_SC("Adjust"), &BufferWrapper::Adjust)
        .Func(_SC("WriteByte"), &BufferWrapper::WriteByte)
        .Func(_SC("WriteShort"), &BufferWrapper::WriteShort)
        .Func(_SC("WriteInt"), &BufferWrapper::WriteInt)
        .Func(_SC("WriteFloat"), &BufferWrapper::WriteFloat)
        .Func(_SC("WriteString"), &BufferWrapper::WriteString)
        .Func(_SC("WriteRawString"), &BufferWrapper::WriteRawString)
        .Func(_SC("ReadByte"), &BufferWrapper::ReadByte)
        .Func(_SC("ReadShort"), &BufferWrapper::ReadShort)
        .Func(_SC("ReadInt"), &BufferWrapper::ReadInt)
        .Func(_SC("ReadFloat"), &BufferWrapper::ReadFloat)
        .Func(_SC("ReadString"), &BufferWrapper::ReadString)
        .Func(_SC("ReadRawString"), &BufferWrapper::ReadRawString)
    );

    Register_BufferInterpreter(bns);

    bns.Func(_SC("Create"), &BufferWrapper::Create);

    RootTable(vm).Bind(_SC("SqBuffer"), bns);
}

} // Namespace:: SqMod
