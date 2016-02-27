// ------------------------------------------------------------------------------------------------
#include "Text.hpp"
#include "Node.hpp"
#include "Module.hpp"

// ------------------------------------------------------------------------------------------------
#include <string.h>
#include <sqrat.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
bool Text::Validate() const
{
    if (m_Doc)
        return true;
    // Invalid document reference
    _SqMod->SqThrow("Invalid XML document reference");
    return false;
}

// ------------------------------------------------------------------------------------------------
Int32 Text::Cmp(const Text & o)
{
    if (strcmp(m_Text.get(), o.m_Text.get()) == 0)
        return 0;
    else if (strlen(m_Text.get()) > strlen(o.m_Text.get()))
        return 1;
    else
        return -1;
}

// ------------------------------------------------------------------------------------------------
Object Text::AsLong(Object & def) const
{
    // Obtain the initial stack size
    const Int32 top = sq_gettop(_SqVM);
    // Push the specified object onto the stack
    Var< Object >::push(_SqVM, def);
    // The resulted long integer value
    Int64 longint = 0;
    // Attempt to get the numeric value inside the specified object
    if (SQ_FAILED(_SqMod->GetSLongValue(_SqVM, -1, &longint)))
        _SqMod->SqThrow("Invalid long integer specified");
    // Push a long integer instance with the requested value on the stack
    _SqMod->PushSLongObject(_SqVM, m_Text.as_llong(longint));
    // Obtain the object from the stack
    Var< Object > inst(_SqVM, -1);
    // Remove an pushed values (if any) to restore the stack
    sq_pop(_SqVM, sq_gettop(_SqVM) - top);
    // Return the long integer instance
    return inst.value;
}

// ------------------------------------------------------------------------------------------------
Object Text::AsUlong(Object & def) const
{
    // Obtain the initial stack size
    const Int32 top = sq_gettop(_SqVM);
    // Push the specified object onto the stack
    Var< Object >::push(_SqVM, def);
    // The resulted long integer value
    Uint64 longint = 0;
    // Attempt to get the numeric value inside the specified object
    if (SQ_FAILED(_SqMod->GetULongValue(_SqVM, -1, &longint)))
        _SqMod->SqThrow("Invalid long integer specified");
    // Push a long integer instance with the requested value on the stack
    _SqMod->PushULongObject(_SqVM, m_Text.as_ullong(longint));
    // Obtain the object from the stack
    Var< Object > inst(_SqVM, -1);
    // Remove an pushed values (if any) to restore the stack
    sq_pop(_SqVM, sq_gettop(_SqVM) - top);
    // Return the long integer instance
    return inst.value;
}

// ------------------------------------------------------------------------------------------------
bool Text::ApplyLong(Object & value)
{
    // Obtain the initial stack size
    const Int32 top = sq_gettop(_SqVM);
    // Push the specified object onto the stack
    Var< Object & >::push(_SqVM, value);
    // The resulted long integer value
    Int64 longint = 0;
    // Whether the operation succeeded
    bool res = false;
    // Attempt to get the numeric value inside the specified object
    if (SQ_FAILED(_SqMod->GetSLongValue(_SqVM, -1, &longint)))
        _SqMod->SqThrow("Invalid long integer specified");
    // Assign the obtained value
    else
        res = m_Text.set(longint);
    // Remove an pushed values (if any) to restore the stack
    sq_pop(_SqVM, sq_gettop(_SqVM) - top);
    // Return the result
    return res;
}

// ------------------------------------------------------------------------------------------------
bool Text::ApplyUlong(Object & value)
{
    // Obtain the initial stack size
    const Int32 top = sq_gettop(_SqVM);
    // Push the specified object onto the stack
    Var< Object & >::push(_SqVM, value);
    // The resulted long integer value
    Uint64 longint = 0;
    // Whether the operation succeeded
    bool res = false;
    // Attempt to get the numeric value inside the specified object
    if (SQ_FAILED(_SqMod->GetULongValue(_SqVM, -1, &longint)))
        _SqMod->SqThrow("Invalid long integer specified");
    // Assign the obtained value
    else
        res = m_Text.set(longint);
    // Remove an pushed values (if any) to restore the stack
    sq_pop(_SqVM, sq_gettop(_SqVM) - top);
    // Return the result
    return res;
}

// ------------------------------------------------------------------------------------------------
Object Text::GetLong() const
{
    // Obtain the initial stack size
    const Int32 top = sq_gettop(_SqVM);
    // Push a long integer instance with the requested value on the stack
    _SqMod->PushSLongObject(_SqVM, m_Text.as_llong());
    // Obtain the object from the stack
    Var< Object > inst(_SqVM, -1);
    // Remove an pushed values (if any) to restore the stack
    sq_pop(_SqVM, sq_gettop(_SqVM) - top);
    // Return the long integer instance
    return inst.value;
}

// ------------------------------------------------------------------------------------------------
void Text::SetLong(Object & value)
{
    // Obtain the initial stack size
    const Int32 top = sq_gettop(_SqVM);
    // Push the specified object onto the stack
    Var< Object & >::push(_SqVM, value);
    // The resulted long integer value
    Int64 longint = 0;
    // Attempt to get the numeric value inside the specified object
    if (SQ_FAILED(_SqMod->GetSLongValue(_SqVM, -1, &longint)))
        _SqMod->SqThrow("Invalid long integer specified");
    // Assign the obtained value
    else
        m_Text = longint;
    // Remove an pushed values (if any) to restore the stack
    sq_pop(_SqVM, sq_gettop(_SqVM) - top);
}

// ------------------------------------------------------------------------------------------------
Object Text::GetUlong() const
{
    // Obtain the initial stack size
    const Int32 top = sq_gettop(_SqVM);
    // Push a long integer instance with the requested value on the stack
    _SqMod->PushULongObject(_SqVM, m_Text.as_ullong());
    // Obtain the object from the stack
    Var< Object > inst(_SqVM, -1);
    // Remove an pushed values (if any) to restore the stack
    sq_pop(_SqVM, sq_gettop(_SqVM) - top);
    // Return the long integer instance
    return inst.value;
}

// ------------------------------------------------------------------------------------------------
void Text::SetUlong(Object & value)
{
    // Obtain the initial stack size
    const Int32 top = sq_gettop(_SqVM);
    // Push the specified object onto the stack
    Var< Object & >::push(_SqVM, value);
    // The resulted long integer value
    Uint64 longint = 0;
    // Attempt to get the numeric value inside the specified object
    if (SQ_FAILED(_SqMod->GetULongValue(_SqVM, -1, &longint)))
        _SqMod->SqThrow("Invalid long integer specified");
    // Assign the obtained value
    else
        m_Text = longint;
    // Remove an pushed values (if any) to restore the stack
    sq_pop(_SqVM, sq_gettop(_SqVM) - top);
}

// ------------------------------------------------------------------------------------------------
Node Text::GetData() const
{
    return Node(m_Doc, m_Text.data());
}

} // Namespace:: SqMod
