// ------------------------------------------------------------------------------------------------
#include "Attribute.hpp"
#include "Module.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqrat.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
bool Attribute::Validate() const
{
    if (m_Doc)
        return true;
    // Invalid document reference
    _SqMod->SqThrow("Invalid XML document reference");
    return false;
}

// ------------------------------------------------------------------------------------------------
void Attribute::SetName(CSStr name)
{
    if (!m_Attr.set_name(name))
        _SqMod->SqThrow("Unable to set xml attribute name");
}

// ------------------------------------------------------------------------------------------------
void Attribute::SetValue(CSStr name)
{
    if (!m_Attr.set_value(name))
        _SqMod->SqThrow("Unable to set xml attribute value");
}

// ------------------------------------------------------------------------------------------------
Object Attribute::AsLong(Object & def) const
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
    _SqMod->PushSLongObject(_SqVM, m_Attr.as_llong(longint));
    // Obtain the object from the stack
    Var< Object > inst(_SqVM, -1);
    // Remove an pushed values (if any) to restore the stack
    sq_pop(_SqVM, sq_gettop(_SqVM) - top);
    // Return the long integer instance
    return inst.value;
}

// ------------------------------------------------------------------------------------------------
Object Attribute::AsUlong(Object & def) const
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
    _SqMod->PushULongObject(_SqVM, m_Attr.as_ullong(longint));
    // Obtain the object from the stack
    Var< Object > inst(_SqVM, -1);
    // Remove an pushed values (if any) to restore the stack
    sq_pop(_SqVM, sq_gettop(_SqVM) - top);
    // Return the long integer instance
    return inst.value;
}

// ------------------------------------------------------------------------------------------------
bool Attribute::ApplyLong(Object & value)
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
        res = m_Attr.set_value(longint);
    // Remove an pushed values (if any) to restore the stack
    sq_pop(_SqVM, sq_gettop(_SqVM) - top);
    // Return the result
    return res;
}

// ------------------------------------------------------------------------------------------------
bool Attribute::ApplyUlong(Object & value)
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
        res = m_Attr.set_value(longint);
    // Remove an pushed values (if any) to restore the stack
    sq_pop(_SqVM, sq_gettop(_SqVM) - top);
    // Return the result
    return res;
}

// ------------------------------------------------------------------------------------------------
Object Attribute::GetLong() const
{
    // Obtain the initial stack size
    const Int32 top = sq_gettop(_SqVM);
    // Push a long integer instance with the requested value on the stack
    _SqMod->PushSLongObject(_SqVM, m_Attr.as_llong());
    // Obtain the object from the stack
    Var< Object > inst(_SqVM, -1);
    // Remove an pushed values (if any) to restore the stack
    sq_pop(_SqVM, sq_gettop(_SqVM) - top);
    // Return the long integer instance
    return inst.value;
}

// ------------------------------------------------------------------------------------------------
void Attribute::SetLong(Object & value)
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
        m_Attr = longint;
    // Remove an pushed values (if any) to restore the stack
    sq_pop(_SqVM, sq_gettop(_SqVM) - top);
}

// ------------------------------------------------------------------------------------------------
Object Attribute::GetUlong() const
{
    // Obtain the initial stack size
    const Int32 top = sq_gettop(_SqVM);
    // Push a long integer instance with the requested value on the stack
    _SqMod->PushULongObject(_SqVM, m_Attr.as_ullong());
    // Obtain the object from the stack
    Var< Object > inst(_SqVM, -1);
    // Remove an pushed values (if any) to restore the stack
    sq_pop(_SqVM, sq_gettop(_SqVM) - top);
    // Return the long integer instance
    return inst.value;
}

// ------------------------------------------------------------------------------------------------
void Attribute::SetUlong(Object & value)
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
        m_Attr = longint;
    // Remove an pushed values (if any) to restore the stack
    sq_pop(_SqVM, sq_gettop(_SqVM) - top);
}

} // Namespace:: SqMod
