// ------------------------------------------------------------------------------------------------
#include "Statement.hpp"
#include "Connection.hpp"

// ------------------------------------------------------------------------------------------------
#include <limits>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger Statement::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMySQLStatement");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
Int32 Statement::Cmp(const Statement & o) const
{
    if (m_Handle == o.m_Handle)
    {
        return 0;
    }
    else if (m_Handle.HndPtr() > o.m_Handle.HndPtr())
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

// ------------------------------------------------------------------------------------------------
Statement::Statement()
    : m_Handle()
{

}

// ------------------------------------------------------------------------------------------------
Statement::Statement(const ConnHnd & connection, CSStr query)
    : m_Handle(connection, query)
{

}

// ------------------------------------------------------------------------------------------------
Statement::Statement(const Connection & connection, CSStr query)
    : m_Handle(connection.GetHnd(), query)
{

}

// ------------------------------------------------------------------------------------------------
Int32 Statement::Execute()
{
    // Validate the managed handle
    m_Handle.Validate();
    // Attempt to bind the parameters
    if (mysql_stmt_bind_param(m_Handle, m_Handle->mMyBinds))
    {
        THROW_CURRENT(m_Handle, "Cannot bind statement parameters");
    }
    // Attempt to execute the statement
    else if (mysql_stmt_execute(m_Handle))
    {
        THROW_CURRENT(m_Handle, "Cannot execute statement");
    }
    else
    {
        return mysql_stmt_affected_rows(m_Handle);
    }
    // Default to a negative value to indicate error
    return -1;
}

// ------------------------------------------------------------------------------------------------
Uint32 Statement::Insert()
{
    // Validate the managed handle
    m_Handle.Validate();
    // Attempt to bind the parameters
    if (mysql_stmt_bind_param(m_Handle, m_Handle->mMyBinds))
    {
        THROW_CURRENT(m_Handle, "Cannot bind statement parameters");
    }
    // Attempt to execute the statement
    else if (mysql_stmt_execute(m_Handle))
    {
        THROW_CURRENT(m_Handle, "Cannot execute statement");
    }
    else
    {
        return mysql_stmt_insert_id(m_Handle);
    }
    // Default to 0
    return 0;
}

// ------------------------------------------------------------------------------------------------
void Statement::SetInt8(Uint32 idx, SQInteger val) const
{
    // Validate the managed handle and specified index
    m_Handle.ValidateIndex(idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_TINY, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mInt64 = ConvTo< Int8 >::From(val);
}

// ------------------------------------------------------------------------------------------------
void Statement::SetUint8(Uint32 idx, SQInteger val) const
{
    // Validate the managed handle and specified index
    m_Handle.ValidateIndex(idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_TINY, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mUint64 = ConvTo< Uint8 >::From(val);
    // Specify that this value is unsigned
    m_Handle->mMyBinds[idx].is_unsigned = true;
}

// ------------------------------------------------------------------------------------------------
void Statement::SetInt16(Uint32 idx, SQInteger val) const
{
    // Validate the managed handle and specified index
    m_Handle.ValidateIndex(idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_SHORT, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mInt64 = ConvTo< Int16 >::From(val);
}

// ------------------------------------------------------------------------------------------------
void Statement::SetUint16(Uint32 idx, SQInteger val) const
{
    // Validate the managed handle and specified index
    m_Handle.ValidateIndex(idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_SHORT, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mUint64 = ConvTo< Uint16 >::From(val);
    // Specify that this value is unsigned
    m_Handle->mMyBinds[idx].is_unsigned = true;
}

// ------------------------------------------------------------------------------------------------
void Statement::SetInt32(Uint32 idx, SQInteger val) const
{
    // Validate the managed handle and specified index
    m_Handle.ValidateIndex(idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_LONG, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mInt64 = ConvTo< Int32 >::From(val);
}

// ------------------------------------------------------------------------------------------------
void Statement::SetUint32(Uint32 idx, Uint32 val) const
{
    // Validate the managed handle and specified index
    m_Handle.ValidateIndex(idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_LONG, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mUint64 = ConvTo< Uint32 >::From(val);
    // Specify that this value is unsigned
    m_Handle->mMyBinds[idx].is_unsigned = true;
}

// ------------------------------------------------------------------------------------------------
void Statement::SetInt64(Uint32 idx, Object & val) const
{
    // Validate the managed handle and specified index
    m_Handle.ValidateIndex(idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_LONGLONG, &(m_Handle->mMyBinds[idx]));
    // Obtain the initial stack size
    const StackGuard sg;
    // Push the specified object onto the stack
    Var< Object >::push(_SqVM, val);
    // Attempt to get the numeric value inside the specified object
    if (SQ_FAILED(_SqMod->GetSLongValue(_SqVM, -1, &(m_Handle->mBinds[idx].mInt64))))
    {
        STHROWF("Invalid long integer specified");
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::SetUint64(Uint32 idx, Object & val) const
{
    // Validate the managed handle and specified index
    m_Handle.ValidateIndex(idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_LONGLONG, &(m_Handle->mMyBinds[idx]));
    // Specify that this value is unsigned
    m_Handle->mMyBinds[idx].is_unsigned = true;
    // Obtain the initial stack size
    const StackGuard sg;
    // Push the specified object onto the stack
    Var< Object >::push(_SqVM, val);
    // Attempt to get the numeric value inside the specified object
    if (SQ_FAILED(_SqMod->GetULongValue(_SqVM, -1, &(m_Handle->mBinds[idx].mUint64))))
    {
        STHROWF("Invalid long integer specified");
    }
}

// ------------------------------------------------------------------------------------------------
void Statement::SetFloat32(Uint32 idx, SQFloat val) const
{
    // Validate the managed handle and specified index
    m_Handle.ValidateIndex(idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_FLOAT, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mFloat32[0] = ConvTo< Float32 >::From(val);
}

// ------------------------------------------------------------------------------------------------
void Statement::SetFloat64(Uint32 idx, SQFloat val) const
{
    // Validate the managed handle and specified index
    m_Handle.ValidateIndex(idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_DOUBLE, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mFloat64 = ConvTo< Float64 >::From(val);
}

// ------------------------------------------------------------------------------------------------
void Statement::SetBoolean(Uint32 idx, bool val) const
{
    // Validate the managed handle and specified index
    m_Handle.ValidateIndex(idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_TINY, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mUint64 = val;
}

// ------------------------------------------------------------------------------------------------
void Statement::SetNull(Uint32 idx) const
{
    // Validate the managed handle and specified index
    m_Handle.ValidateIndex(idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_NULL, &(m_Handle->mMyBinds[idx]));
}

} // Namespace:: SqMod
