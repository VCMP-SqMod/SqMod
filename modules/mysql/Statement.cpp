// ------------------------------------------------------------------------------------------------
#include "Statement.hpp"
#include "Connection.hpp"
#include "ResultSet.hpp"

// ------------------------------------------------------------------------------------------------
#include <limits>
#include <cstring>

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
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Statement::Validate(CCStr file, Int32 line) const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid MySQL statement reference =>[%s:%d]", file, line);
    }
}
#else
void Statement::Validate() const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid MySQL statement reference");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Statement::ValidateCreated(CCStr file, Int32 line) const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid MySQL statement reference =>[%s:%d]", file, line);
    }
    else if (m_Handle->mPtr == nullptr)
    {
        SqThrowF("Invalid MySQL statement =>[%s:%d]", file, line);
    }
}
#else
void Statement::ValidateCreated() const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid MySQL statement reference");
    }
    else if (m_Handle->mPtr == nullptr)
    {
        SqThrowF("Invalid MySQL statement");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const StmtRef & Statement::GetValid(CCStr file, Int32 line) const
{
    Validate(file, line);
    return m_Handle;
}
#else
const StmtRef & Statement::GetValid() const
{
    Validate();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const StmtRef & Statement::GetCreated(CCStr file, Int32 line) const
{
    ValidateCreated(file, line);
    return m_Handle;
}
#else
const StmtRef & Statement::GetCreated() const
{
    ValidateCreated();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Statement::ValidateParam(Int32 idx, CCStr file, Int32 line) const
{
    ValidateCreated(file, line);
    m_Handle->ValidateParam(idx, file, line);
}
#else
void Statement::ValidateParam(Int32 idx) const
{
    ValidateCreated();
    m_Handle->ValidateParam(idx);
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
Statement::Statement(const Connection & connection, CSStr query)
    : Statement(connection.GetHandle(), query)
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
Connection Statement::GetConnection() const
{
    return Connection(SQMOD_GET_VALID(*this)->mConnection);
}

// ------------------------------------------------------------------------------------------------
void Statement::SetConnection(const Connection & conn)
{
    SQMOD_GET_VALID(*this)->mConnection = conn.GetHandle();
}

// ------------------------------------------------------------------------------------------------
Int32 Statement::Execute()
{
    // Attempt to bind the parameters
    if (mysql_stmt_bind_param(SQMOD_GET_CREATED(*this)->mPtr, m_Handle->mMyBinds))
    {
        SQMOD_THROW_CURRENT(*m_Handle, "Cannot bind MySQL statement parameters");
    }
    // Attempt to execute the statement
    else if (mysql_stmt_execute(m_Handle->mPtr))
    {
        SQMOD_THROW_CURRENT(*m_Handle, "Cannot execute MySQL statement");
    }
    // Return the number of rows affected by this query
    return mysql_stmt_affected_rows(m_Handle->mPtr);
}

// ------------------------------------------------------------------------------------------------
Uint32 Statement::Insert()
{
    // Attempt to bind the parameters
    if (mysql_stmt_bind_param(SQMOD_GET_CREATED(*this)->mPtr, m_Handle->mMyBinds))
    {
        SQMOD_THROW_CURRENT(*m_Handle, "Cannot bind MySQL statement parameters");
    }
    // Attempt to execute the statement
    else if (mysql_stmt_execute(m_Handle->mPtr))
    {
        SQMOD_THROW_CURRENT(*m_Handle, "Cannot execute MySQL statement");
    }
    // Return the identifier of the inserted row
    return mysql_stmt_insert_id(m_Handle->mPtr);
}

// ------------------------------------------------------------------------------------------------
ResultSet Statement::Query()
{
    // Attempt to bind the parameters
    if (mysql_stmt_bind_param(SQMOD_GET_CREATED(*this)->mPtr, m_Handle->mMyBinds))
    {
        SQMOD_THROW_CURRENT(*m_Handle, "Cannot bind MySQL statement parameters");
    }
    // Attempt to execute the statement
    else if (mysql_stmt_execute(m_Handle->mPtr))
    {
        SQMOD_THROW_CURRENT(*m_Handle, "Cannot execute MySQL statement");
    }
    // Return the results of this query
    return ResultSet(m_Handle);
}

// ------------------------------------------------------------------------------------------------
void Statement::SetInt8(Uint32 idx, SQInteger val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_TINY, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mInt64 = ConvTo< Int8 >::From(val);
}

// ------------------------------------------------------------------------------------------------
void Statement::SetUint8(Uint32 idx, SQInteger val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
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
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_SHORT, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mInt64 = ConvTo< Int16 >::From(val);
}

// ------------------------------------------------------------------------------------------------
void Statement::SetUint16(Uint32 idx, SQInteger val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
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
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_LONG, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mInt64 = ConvTo< Int32 >::From(val);
}

// ------------------------------------------------------------------------------------------------
void Statement::SetUint32(Uint32 idx, SQInteger val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_LONG, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mUint64 = ConvTo< Uint32 >::From(val);
    // Specify that this value is unsigned
    m_Handle->mMyBinds[idx].is_unsigned = true;
}

// ------------------------------------------------------------------------------------------------
void Statement::SetInt64(Uint32 idx, SQInteger val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_LONGLONG, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mInt64 = ConvTo< Int64 >::From(val);
}

// ------------------------------------------------------------------------------------------------
void Statement::SetUint64(Uint32 idx, SQInteger val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_LONGLONG, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mUint64 = ConvTo< Uint64 >::From(val);
    // Specify that this value is unsigned
    m_Handle->mMyBinds[idx].is_unsigned = true;
}

// ------------------------------------------------------------------------------------------------
void Statement::SetSLongInt(Uint32 idx, Object & val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_LONGLONG, &(m_Handle->mMyBinds[idx]));
    // Attempt to assign the numeric value inside the specified object
    m_Handle->mBinds[idx].mInt64 = FetchSLongObjVal(val);
}

// ------------------------------------------------------------------------------------------------
void Statement::SetULongInt(Uint32 idx, Object & val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_LONGLONG, &(m_Handle->mMyBinds[idx]));
    // Attempt to assign the numeric value inside the specified object
    m_Handle->mBinds[idx].mUint64 = FetchULongObjVal(val);
    // Specify that this value is unsigned
    m_Handle->mMyBinds[idx].is_unsigned = true;
}

// ------------------------------------------------------------------------------------------------
void Statement::SetInteger(Uint32 idx, SQInteger val) const
{
#ifdef _SQ64
    SetInt64(idx, val);
#else
    SetInt32(idx, val);
#endif // _SQ64
}

// ------------------------------------------------------------------------------------------------
void Statement::SetFloat32(Uint32 idx, SQFloat val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_FLOAT, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mFloat32[0] = ConvTo< Float32 >::From(val);
}

// ------------------------------------------------------------------------------------------------
void Statement::SetFloat64(Uint32 idx, SQFloat val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_DOUBLE, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mFloat64 = ConvTo< Float64 >::From(val);
}

// ------------------------------------------------------------------------------------------------
void Statement::SetFloat(Uint32 idx, SQFloat val) const
{
#ifdef SQUSEDOUBLE
    SetFloat64(idx, val);
#else
    SetFloat32(idx, val);
#endif // SQUSEDOUBLE
}

// ------------------------------------------------------------------------------------------------
void Statement::SetBoolean(Uint32 idx, bool val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_TINY, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    m_Handle->mBinds[idx].mUint64 = val;
}

// ------------------------------------------------------------------------------------------------
void Statement::SetDate(Uint32 idx, Object & val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_DATE, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    SqDateToMySQLTime(val, m_Handle->mBinds[idx].mTime);
}

// ------------------------------------------------------------------------------------------------
void Statement::SetTime(Uint32 idx, Object & val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_TIME, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    SqTimeToMySQLTime(val, m_Handle->mBinds[idx].mTime);
}

// ------------------------------------------------------------------------------------------------
void Statement::SetDatetime(Uint32 idx, Object & val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_DATETIME, &(m_Handle->mMyBinds[idx]));
    // Assign the value to the input
    SqDatetimeToMySQLTime(val, m_Handle->mBinds[idx].mTime);
}

// ------------------------------------------------------------------------------------------------
void Statement::SetString(Uint32 idx, CSStr val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_STRING, &(m_Handle->mMyBinds[idx]), val, std::strlen(val));
}

// ------------------------------------------------------------------------------------------------
void Statement::SetEnum(Uint32 idx, CSStr val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_ENUM, &(m_Handle->mMyBinds[idx]), val, std::strlen(val));
}

// ------------------------------------------------------------------------------------------------
void Statement::SetSet(Uint32 idx, CSStr val) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_SET, &(m_Handle->mMyBinds[idx]), val, std::strlen(val));
}

// ------------------------------------------------------------------------------------------------
void Statement::SetBlob(Uint32 /*idx*/, Object & /*val*/) const
{
    // TODO...
}

// ------------------------------------------------------------------------------------------------
void Statement::SetData(Uint32 /*idx*/, Object & /*val*/) const
{
    // TODO...
}

// ------------------------------------------------------------------------------------------------
void Statement::SetNull(Uint32 idx) const
{
    SQMOD_VALIDATE_PARAM(*this, idx);
    // Attempt to set the input value
    m_Handle->mBinds[idx].SetInput(MYSQL_TYPE_NULL, &(m_Handle->mMyBinds[idx]));
}

// ================================================================================================
void Register_Statement(Table & sqlns)
{
    sqlns.Bind(_SC("Statement")
        , Class< Statement >(sqlns.GetVM(), _SC("SqMySQLStatement"))
        // Constructors
        .Ctor()
        .Ctor< const Statement & >()
        .Ctor< const Connection &, CSStr >()
        // Core Meta-methods
        .Func(_SC("_cmp"), &Statement::Cmp)
        .SquirrelFunc(_SC("_typename"), &Statement::Typename)
        .Func(_SC("_tostring"), &Statement::ToString)
        // Properties
        .Prop(_SC("IsValid"), &Statement::IsValid)
        .Prop(_SC("Connection"), &Statement::GetConnection, &Statement::SetConnection)
        // Member Methods
        .Func(_SC("Execute"), &Statement::Execute)
        .Func(_SC("Insert"), &Statement::Insert)
        .Func(_SC("Query"), &Statement::Query)
        .Func(_SC("SetInt8"), &Statement::SetInt8)
        .Func(_SC("SetUint8"), &Statement::SetUint8)
        .Func(_SC("SetInt16"), &Statement::SetInt16)
        .Func(_SC("SetUint16"), &Statement::SetUint16)
        .Func(_SC("SetInt32"), &Statement::SetInt32)
        .Func(_SC("SetUint32"), &Statement::SetUint32)
        .Func(_SC("SetInt64"), &Statement::SetInt64)
        .Func(_SC("SetUint64"), &Statement::SetUint64)
        .Func(_SC("SetSLongInt"), &Statement::SetSLongInt)
        .Func(_SC("SetULongInt"), &Statement::SetULongInt)
        .Func(_SC("SetInteger"), &Statement::SetInteger)
        .Func(_SC("SetFloat32"), &Statement::SetFloat32)
        .Func(_SC("SetFloat64"), &Statement::SetFloat64)
        .Func(_SC("SetFloat"), &Statement::SetFloat)
        .Func(_SC("SetBoolean"), &Statement::SetBoolean)
        .Func(_SC("SetDate"), &Statement::SetDate)
        .Func(_SC("SetTime"), &Statement::SetTime)
        .Func(_SC("SetDatetime"), &Statement::SetDatetime)
        .Func(_SC("SetString"), &Statement::SetString)
        .Func(_SC("SetEnum"), &Statement::SetEnum)
        .Func(_SC("SetSet"), &Statement::SetSet)
        .Func(_SC("SetBlob"), &Statement::SetBlob)
        .Func(_SC("SetData"), &Statement::SetData)
        .Func(_SC("SetBuffer"), &Statement::SetData)
        .Func(_SC("SetNull"), &Statement::SetNull)
    );
}

} // Namespace:: SqMod