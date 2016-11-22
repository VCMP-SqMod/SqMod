// ------------------------------------------------------------------------------------------------
#include "Statement.hpp"
#include "Connection.hpp"
#include "Column.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstring>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQMODE_DECL_TYPENAME(Typename, _SC("SqLiteStatement"))

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Statement::Validate(CCStr file, Int32 line) const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid SQLite statement reference =>[%s:%d]", file, line);
    }
}
#else
void Statement::Validate() const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid SQLite statement reference");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Statement::ValidateCreated(CCStr file, Int32 line) const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid SQLite statement reference =>[%s:%d]", file, line);
    }
    else if (m_Handle->mPtr == nullptr)
    {
        SqThrowF("Invalid SQLite statement =>[%s:%d]", file, line);
    }
}
#else
void Statement::ValidateCreated() const
{
    if (!m_Handle)
    {
        SqThrowF("Invalid SQLite statement reference");
    }
    else if (m_Handle->mPtr == nullptr)
    {
        SqThrowF("Invalid SQLite statement");
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
void Statement::ValidateColumn(Int32 idx, CCStr file, Int32 line) const
{
    ValidateCreated(file, line);
    // Is the specified index in range?
    if (!m_Handle->CheckColumn(idx))
    {
        SqThrowF("Column index is out of range: %d:%d =>[%s:%d]", idx, m_Handle->mColumns,
                    file, line);
    }
}
#else
void Statement::ValidateColumn(Int32 idx) const
{
    ValidateCreated();
    // Is the specified index in range?
    if (!m_Handle->CheckColumn(idx))
    {
        SqThrowF("Column index is out of range: %d:%d", idx, m_Handle->mColumns);
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Statement::ValidateParam(Int32 idx, CCStr file, Int32 line) const
{
    ValidateCreated(file, line);
    // Is the specified index in range?
    if (!m_Handle->CheckParameter(idx))
    {
        SqThrowF("Parameter index is out of range: %d:%d =>[%s:%d]", idx, m_Handle->mParameters,
                    file, line);
    }
}
#else
void Statement::ValidateParam(Int32 idx) const
{
    ValidateCreated();
    // Is the specified index in range?
    if (!m_Handle->CheckParameter(idx))
    {
        SqThrowF("Parameter index is out of range: %d:%d", idx, m_Handle->mParameters);
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Statement::ValidateRow(CCStr file, Int32 line) const
{
    ValidateCreated(file, line);
    // Do we have any rows available?
    if (!m_Handle->mGood)
    {
        SqThrowF("No row available =>[%s:%d]", file, line);
    }
}
#else
void Statement::ValidateRow() const
{
    ValidateCreated();
    // Do we have any rows available?
    if (!m_Handle->mGood)
    {
        SqThrowF("No row available");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
Statement::Statement(const Connection & connection, const StackStrF & query)
    : m_Handle(new StmtHnd(connection.GetHandle()))
{
    SQMOD_GET_VALID(*this)->Create(query.mPtr, query.mLen);
}

// ------------------------------------------------------------------------------------------------
Object Statement::GetConnection() const
{
    return Object(new Connection(SQMOD_GET_VALID(*this)->mConn));
}

// ------------------------------------------------------------------------------------------------
Statement & Statement::Reset()
{
    SQMOD_VALIDATE_CREATED(*this);
    // Specify that we don't have a row available and we haven't finished stepping
    m_Handle->mGood = false;
    m_Handle->mDone = false;
    // Attempt to reset the statement to it's initial state
    m_Handle->mStatus = sqlite3_reset(m_Handle->mPtr);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF("Unable to reset statement [%s]", m_Handle->ErrStr());
    }
    // Allow further chaining of operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Statement & Statement::Clear()
{
    SQMOD_VALIDATE_CREATED(*this);
    // Specify that we don't have a row available and we haven't finished stepping
    m_Handle->mGood = false;
    m_Handle->mDone = false;
    // Attempt to clear the statement
    m_Handle->mStatus = sqlite3_clear_bindings(m_Handle->mPtr);
    // Validate the result
    if (m_Handle->mStatus != SQLITE_OK)
    {
        STHROWF("Unable to clear statement [%s]", m_Handle->ErrStr());
    }
    // Allow further chaining of operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Int32 Statement::Exec()
{
    SQMOD_VALIDATE_CREATED(*this);
    // Did we reset first?
    if (m_Handle->mDone)
    {
        STHROWF("Executed without resetting first");
    }
    // Attempt to step the statement
    m_Handle->mStatus = sqlite3_step(m_Handle->mPtr);
    // Have we finished stepping?
    if (m_Handle->mStatus == SQLITE_DONE)
    {
        // Specify that we don't have row available and we finished stepping
        m_Handle->mGood = false;
        m_Handle->mDone = true;
        // Return the changes made by this statement
        return sqlite3_changes(m_Handle->mConn->mPtr);
    }
    // Specify that we don't have any row and we haven't finished stepping
    m_Handle->mGood = false;
    m_Handle->mDone = false;
    // Inspect the result
    switch (m_Handle->mStatus)
    {
        // We don't expect any rows to be returned in this case!
        case SQLITE_ROW:    STHROWF("Results were found");
        case SQLITE_BUSY:   STHROWF("Database was busy");
        case SQLITE_ERROR:  STHROWF("Runtime error occurred");
        case SQLITE_MISUSE: STHROWF("Statement misuse");
        default:            STHROWF("Unknown failure [%s]", m_Handle->ErrStr());
    }
    // Operation failed (shouldn't reach this point!)
    return -1;
}

// ------------------------------------------------------------------------------------------------
bool Statement::Step()
{
    SQMOD_VALIDATE_CREATED(*this);
    // Did we reset first?
    if (m_Handle->mDone)
    {
        STHROWF("Stepped without resetting first");
    }
    // Attempt to step the statement
    m_Handle->mStatus = sqlite3_step(m_Handle->mPtr);
    // Do we have a row available?
    if (m_Handle->mStatus == SQLITE_ROW)
    {
        // Specify that we have a row available
        return (m_Handle->mGood = true);
    }
    // Have we finished stepping?
    else if (m_Handle->mStatus == SQLITE_DONE)
    {
        // Specify that we finished stepping
        m_Handle->mDone = true;
        // Specify that we don't have a row available
        return (m_Handle->mGood = false);
    }
    // Specify that we don't have any row and we haven't finished stepping
    m_Handle->mGood = false;
    m_Handle->mDone = false;
    // Inspect the result
    switch (m_Handle->mStatus)
    {
        case SQLITE_BUSY:   STHROWF("Database was busy");
        case SQLITE_ERROR:  STHROWF("Runtime error occurred");
        case SQLITE_MISUSE: STHROWF("Statement misuse");
        default:            STHROWF("Unknown failure [%s]", m_Handle->ErrStr());
    }
    // Operation failed (shouldn't reach this point!)
    return false;
}

// ------------------------------------------------------------------------------------------------
Statement & Statement::SetArray(Int32 idx, const Array & arr)
{
    // Obtain a script iterator
    Array::iterator itr;
    // Create a parameter instance to bind the values
    Parameter param(m_Handle);
    // Process each element until _next returns null
    while (idx <= m_Handle->mParameters && arr.Next(itr))
    {
        // Update the parameter index
        param.SetIndex(idx++);
        // Bind it to the current index
        param.SetValue(Object(itr.getValue()));
    }
    // Allow further chaining of operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Statement & Statement::SetTable(const Table & tbl)
{
    // Is there anything to bind?
    if (tbl.GetSize() <= 0)
    {
        return *this; // Nothing to bind!
    }
    // Obtain a table iterator
    Table::iterator itr;
    // Create a parameter instance to bind the values
    Parameter param(m_Handle);
    // Process each element until _next returns null
    while (tbl.Next(itr))
    {
        // Update the parameter index
        param.SetIndex(itr.getName());
        // Bind it to the current index
        param.SetValue(Object(itr.getValue()));
    }
    // Allow further chaining of operations
    return *this;
}

// ------------------------------------------------------------------------------------------------
Array Statement::GetArray(Int32 min, Int32 max) const
{
    SQMOD_VALIDATE_ROW(*this);
    // Is the specified minimum index valid?
    if (min < 0)
    {
        STHROWF("Minimum is bellow zero: %d", min);
    }
    // Is the minimum actually the minimum?
    else if (min > max)
    {
        STHROWF("Minimum is higher than maximum: %d > %d", min, max);
    }
    // Is the minimum in range?
    else if (!m_Handle->CheckColumn(min))
    {
        STHROWF("Minimum is out of range: %d:%d", min, m_Handle->mColumns);
    }
    // Is the maximum in range?
    else if (!m_Handle->CheckColumn(max))
    {
        STHROWF("Maximum is out of range: %d:%d", max, m_Handle->mColumns);
    }
    // Allocate an array large enough to hold the values from selected columns
    Array arr(DefaultVM::Get(), max-min);
    // Create a column instance to retrieve the values
    Column column(m_Handle);
    // Array element counter
    Int32 elem = 0;
    // Process the range of selected columns
    while (min <= max)
    {
        // Update the column index
        column.SetIndex(min++);
        // Retrieve the column value and bind it to the array
        arr.SetValue(elem++, column.GetValue());
    }
    // Return the resulted array
    return arr;
}

// ------------------------------------------------------------------------------------------------
Table Statement::GetTable(Int32 min, Int32 max) const
{
    SQMOD_VALIDATE_ROW(*this);
    // Is the specified minimum index valid?
    if (min < 0)
    {
        STHROWF("Minimum is bellow zero: %d", min);
    }
    // Is the minimum actually the minimum?
    else if (min > max)
    {
        STHROWF("Minimum is higher than maximum: %d > %d", min, max);
    }
    // Is the minimum in range>
    else if (!m_Handle->CheckColumn(min))
    {
        STHROWF("Minimum is out of range: %d:%d", min, m_Handle->mColumns);
    }
    // Is the maximum in range?
    else if (!m_Handle->CheckColumn(max))
    {
        STHROWF("Maximum is out of range: %d:%d", max, m_Handle->mColumns);
    }
    // Create a table to hold the selected column values
    Table tbl(DefaultVM::Get());
    // Create a column instance to retrieve the values
    Column column(m_Handle);
    // Process the range of selected columns
    while (min <= max)
    {
        // Attempt to obtain the column name
        CSStr name = sqlite3_column_name(m_Handle->mPtr, min);
        // Validate the obtained name
        if (!name)
        {
            STHROWF("Unable to retrieve name of column (%d)", min);
        }
        // Update the column index
        column.SetIndex(min++);
        // Retrieve the column value and bind it to the table
        tbl.SetValue(name, column.GetValue());
    }
    // Return the resulted table
    return tbl;
}

// ================================================================================================
void Register_Statement(Table & sqlns)
{
    sqlns.Bind(_SC("Statement"),
        Class< Statement >(sqlns.GetVM(), Typename::Str)
        // Constructors
        .Ctor()
        .Ctor< const Statement & >()
        .FmtCtor< const Connection & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &Typename::Fn)
        .Func(_SC("_tostring"), &Statement::ToString)
        // Properties
        .Prop(_SC("IsValid"), &Statement::IsValid)
        .Prop(_SC("Prepared"), &Statement::IsPrepared)
        .Prop(_SC("References"), &Statement::GetReferences)
        .Prop(_SC("Connection"), &Statement::GetConnection)
        .Prop(_SC("Status"), &Statement::GetStatus)
        .Prop(_SC("ErrCode"), &Statement::GetErrorCode)
        .Prop(_SC("ExErrCode"), &Statement::GetExtendedErrorCode)
        .Prop(_SC("ExtendedErrCode"), &Statement::GetExtendedErrorCode)
        .Prop(_SC("ErrStr"), &Statement::GetErrStr)
        .Prop(_SC("ErrMsg"), &Statement::GetErrMsg)
        .Prop(_SC("Columns"), &Statement::GetColumns)
        .Prop(_SC("Parameters"), &Statement::GetParameters)
        .Prop(_SC("Query"), &Statement::GetQuery)
        .Prop(_SC("Good"), &Statement::GetGood)
        .Prop(_SC("Done"), &Statement::GetDone)
        .Prop(_SC("DataCount"), &Statement::GetDataCount)
        // Member Methods
        .Func(_SC("Release"), &Statement::Release)
        .Func(_SC("CheckParameter"), &Statement::CheckParameter)
        .FmtFunc(_SC("GetParameterIndex"), &Statement::GetParameterIndex)
        .Func(_SC("GetParameterName"), &Statement::GetParameterName)
        .Func(_SC("CheckColumn"), &Statement::CheckColumn)
        .Func(_SC("IsColumnNull"), &Statement::IsColumnNull)
        .FmtFunc(_SC("ColumnIndex"), &Statement::GetColumnIndex)
        .Func(_SC("ColumnName"), &Statement::GetColumnName)
        .Func(_SC("ColumnOriginName"), &Statement::GetColumnOriginName)
        .Func(_SC("ColumnType"), &Statement::GetColumnType)
        .Func(_SC("ColumnBytes"), &Statement::GetColumnBytes)
        .Func(_SC("Reset"), &Statement::Reset)
        .Func(_SC("Clear"), &Statement::Clear)
        .Func(_SC("Exec"), &Statement::Exec)
        .Func(_SC("Step"), &Statement::Step)
        .Func(_SC("Param"), &Statement::GetParameter)
        .Func(_SC("Parameter"), &Statement::GetParameter)
        .Func(_SC("SetValue"), &Statement::SetValue)
        .Func(_SC("SetBool"), &Statement::SetBool)
        .Func(_SC("SetChar"), &Statement::SetChar)
        .Func(_SC("SetInteger"), &Statement::SetInteger)
        .Func(_SC("SetInt8"), &Statement::SetInt8)
        .Func(_SC("SetUint8"), &Statement::SetUint8)
        .Func(_SC("SetInt16"), &Statement::SetInt16)
        .Func(_SC("SetUint16"), &Statement::SetUint16)
        .Func(_SC("SetInt32"), &Statement::SetInt32)
        .Func(_SC("SetUint32"), &Statement::SetUint32)
        .Func(_SC("SetInt64"), &Statement::SetInt64)
        .Func(_SC("SetUint64"), &Statement::SetUint64)
        .Func(_SC("SetFloat"), &Statement::SetFloat)
        .Func(_SC("SetFloat32"), &Statement::SetFloat32)
        .Func(_SC("SetFloat64"), &Statement::SetFloat64)
        .FmtFunc(_SC("SetString"), &Statement::SetString)
        .Func(_SC("SetZeroBlob"), &Statement::SetZeroBlob)
        .Func(_SC("SetBlob"), &Statement::SetBlob)
        .Func(_SC("SetData"), &Statement::SetData)
        .Func(_SC("SetDate"), &Statement::SetDate)
        .Func(_SC("SetDateEx"), &Statement::SetDateEx)
        .Func(_SC("SetTime"), &Statement::SetTime)
        .Func(_SC("SetTimeEx"), &Statement::SetTimeEx)
        .Func(_SC("SetDatetime"), &Statement::SetDatetime)
        .Func(_SC("SetDatetimeEx"), &Statement::SetDatetimeEx)
        .Func(_SC("SetNow"), &Statement::SetNow)
        .Func(_SC("SetNull"), &Statement::SetNull)
        .Func(_SC("SetArray"), &Statement::SetArray)
        .Func(_SC("SetTable"), &Statement::SetTable)
        .Func(_SC("Field"), &Statement::GetColumn)
        .Func(_SC("Column"), &Statement::GetColumn)
        .Func(_SC("GetValue"), &Statement::GetValue)
        .Func(_SC("GetNumber"), &Statement::GetNumber)
        .Func(_SC("GetInteger"), &Statement::GetInteger)
        .Func(_SC("GetFloat"), &Statement::GetFloat)
        .Func(_SC("GetLong"), &Statement::GetLong)
        .Func(_SC("GetString"), &Statement::GetString)
        .Func(_SC("GetBoolean"), &Statement::GetBoolean)
        .Func(_SC("GetChar"), &Statement::GetChar)
        .Func(_SC("GetBuffer"), &Statement::GetBuffer)
        .Func(_SC("GetBlob"), &Statement::GetBlob)
        // Member overloads
        .Overload< Array (Statement::*)(void) const >(_SC("GetArray"), &Statement::GetArray)
        .Overload< Array (Statement::*)(Int32) const >(_SC("GetArray"), &Statement::GetArray)
        .Overload< Array (Statement::*)(Int32, Int32) const >(_SC("GetArray"), &Statement::GetArray)
        .Overload< Table (Statement::*)(void) const >(_SC("GetTable"), &Statement::GetTable)
        .Overload< Table (Statement::*)(Int32) const >(_SC("GetTable"), &Statement::GetTable)
        .Overload< Table (Statement::*)(Int32, Int32) const >(_SC("GetTable"), &Statement::GetTable)
    );
}

} // Namespace:: SqMod
