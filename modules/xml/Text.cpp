// ------------------------------------------------------------------------------------------------
#include "Text.hpp"
#include "Node.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstring>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger Text::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqXmlText");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
Int32 Text::Cmp(const Text & o)
{
    if (strcmp(m_Text.get(), o.m_Text.get()) == 0)
    {
        return 0;
    }
    else if (strlen(m_Text.get()) > strlen(o.m_Text.get()))
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

// ------------------------------------------------------------------------------------------------
Object Text::AsLong(Object & def) const
{
    // Obtain the initial stack size
    const StackGuard sg;
    // Push the specified object onto the stack
    Var< Object >::push(DefaultVM::Get(), def);
    // The resulted long integer value
    Int64 longint = 0;
    // Attempt to get the numeric value inside the specified object
    if (SQ_FAILED(SqMod_GetSLongValue(DefaultVM::Get(), -1, &longint)))
    {
        STHROWF("Invalid long integer specified");
    }
    // Push a long integer instance with the requested value on the stack
    SqMod_PushSLongObject(DefaultVM::Get(), m_Text.as_llong(longint));
    // Obtain the object from the stack and return it
    return Var< Object >(DefaultVM::Get(), -1).value;
}

// ------------------------------------------------------------------------------------------------
Object Text::AsUlong(Object & def) const
{
    // Obtain the initial stack size
    const StackGuard sg;
    // Push the specified object onto the stack
    Var< Object >::push(DefaultVM::Get(), def);
    // The resulted long integer value
    Uint64 longint = 0;
    // Attempt to get the numeric value inside the specified object
    if (SQ_FAILED(SqMod_GetULongValue(DefaultVM::Get(), -1, &longint)))
    {
        STHROWF("Invalid long integer specified");
    }
    // Push a long integer instance with the requested value on the stack
    SqMod_PushULongObject(DefaultVM::Get(), m_Text.as_ullong(longint));
    // Obtain the object from the stack and return it
    return Var< Object >(DefaultVM::Get(), -1).value;
}

// ------------------------------------------------------------------------------------------------
bool Text::ApplyLong(Object & value)
{
    // Obtain the initial stack size
    const StackGuard sg;
    // Push the specified object onto the stack
    Var< Object & >::push(DefaultVM::Get(), value);
    // The resulted long integer value
    Int64 longint = 0;
    // Attempt to get the numeric value inside the specified object
    if (SQ_FAILED(SqMod_GetSLongValue(DefaultVM::Get(), -1, &longint)))
    {
        STHROWF("Invalid long integer specified");
    }
    // Assign the obtained value and return the result
    return m_Text.set(longint);
}

// ------------------------------------------------------------------------------------------------
bool Text::ApplyUlong(Object & value)
{
    // Obtain the initial stack size
    const StackGuard sg;
    // Push the specified object onto the stack
    Var< Object & >::push(DefaultVM::Get(), value);
    // The resulted long integer value
    Uint64 longint = 0;
    // Attempt to get the numeric value inside the specified object
    if (SQ_FAILED(SqMod_GetULongValue(DefaultVM::Get(), -1, &longint)))
    {
        STHROWF("Invalid long integer specified");
    }
    // Assign the obtained value and return the result
    return m_Text.set(longint);
}

// ------------------------------------------------------------------------------------------------
Object Text::GetLong() const
{
    // Obtain the initial stack size
    const StackGuard sg;
    // Push a long integer instance with the requested value on the stack
    SqMod_PushSLongObject(DefaultVM::Get(), m_Text.as_llong());
    // Obtain the object from the stack and return it
    return Var< Object >(DefaultVM::Get(), -1).value;
}

// ------------------------------------------------------------------------------------------------
void Text::SetLong(Object & value)
{
    // Obtain the initial stack size
    const StackGuard sg;
    // Push the specified object onto the stack
    Var< Object & >::push(DefaultVM::Get(), value);
    // The resulted long integer value
    Int64 longint = 0;
    // Attempt to get the numeric value inside the specified object
    if (SQ_FAILED(SqMod_GetSLongValue(DefaultVM::Get(), -1, &longint)))
    {
        STHROWF("Invalid long integer specified");
    }
    // Assign the obtained value
    m_Text = longint;
}

// ------------------------------------------------------------------------------------------------
Object Text::GetUlong() const
{
    // Obtain the initial stack size
    const StackGuard sg;
    // Push a long integer instance with the requested value on the stack
    SqMod_PushULongObject(DefaultVM::Get(), m_Text.as_ullong());
    // Obtain the object from the stack and return it
    return Var< Object >(DefaultVM::Get(), -1).value;
}

// ------------------------------------------------------------------------------------------------
void Text::SetUlong(Object & value)
{
    // Obtain the initial stack size
    const StackGuard sg;
    // Push the specified object onto the stack
    Var< Object & >::push(DefaultVM::Get(), value);
    // The resulted long integer value
    Uint64 longint = 0;
    // Attempt to get the numeric value inside the specified object
    if (SQ_FAILED(SqMod_GetULongValue(DefaultVM::Get(), -1, &longint)))
    {
        STHROWF("Invalid long integer specified");
    }
    // Assign the obtained value
    m_Text = longint;
}

// ------------------------------------------------------------------------------------------------
Node Text::GetData() const
{
    return Node(m_Doc, m_Text.data());
}

} // Namespace:: SqMod
