// ------------------------------------------------------------------------------------------------
#include "ResultSet.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdlib>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger ResultSet::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMySQLResultSet");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void ResultSet::Validate(CCStr file, Int32 line) const
{
    // Do we have a valid result-set handle?
    if (!m_Handle)
    {
        SqThrowF("Invalid MySQL result-set reference =>[%s:%d]", file, line);
    }
}
#else
void ResultSet::Validate() const
{
    // Do we have a valid result-set handle?
    if (!m_Handle)
    {
        SqThrowF("Invalid MySQL result-set reference");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void ResultSet::ValidateCreated(CCStr file, Int32 line) const
{
    // Do we have a valid result-set handle?
    if (!m_Handle)
    {
        SqThrowF("Invalid MySQL result-set reference =>[%s:%d]", file, line);
    }
    else if (m_Handle->mPtr == nullptr)
    {
        SqThrowF("Invalid MySQL result-set =>[%s:%d]", file, line);
    }
}
#else
void ResultSet::ValidateCreated() const
{
    // Do we have a valid result-set handle?
    if (!m_Handle)
    {
        SqThrowF("Invalid MySQL result-set reference");
    }
    else if (m_Handle->mPtr == nullptr)
    {
        SqThrowF("Invalid MySQL result-set");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void ResultSet::ValidateStepped(CCStr file, Int32 line) const
{
    // Do we have a valid result-set handle?
    if (!m_Handle)
    {
        SqThrowF("Invalid MySQL result-set reference =>[%s:%d]", file, line);
    }
    // Do we have a valid row available?
    else if (m_Handle->mRow == nullptr)
    {
        SqThrowF("No row available in MySQL result-set =>[%s:%d]", file, line);
    }
}
#else
void ResultSet::ValidateStepped() const
{
    // Do we have a valid result-set handle?
    if (!m_Handle)
    {
        SqThrowF("Invalid MySQL result-set reference");
    }
    // Do we have a valid row available?
    else if (m_Handle->mRow == nullptr)
    {
        SqThrowF("No row available in MySQL result-set");
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const ResRef & ResultSet::GetValid(CCStr file, Int32 line) const
{
    Validate(file, line);
    return m_Handle;
}
#else
const ResRef & ResultSet::GetValid() const
{
    Validate();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const ResRef & ResultSet::GetCreated(CCStr file, Int32 line) const
{
    ValidateCreated(file, line);
    return m_Handle;
}
#else
const ResRef & ResultSet::GetCreated() const
{
    ValidateCreated();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const ResRef & ResultSet::GetStepped(CCStr file, Int32 line) const
{
    ValidateStepped(file, line);
    return m_Handle;
}
#else
const ResRef & ResultSet::GetStepped() const
{
    ValidateStepped();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void ResultSet::ValidateField(Int32 idx, CCStr file, Int32 line) const
{
    ValidateCreated(file, line);
    m_Handle->ValidateField(idx, file, line);
}
#else
void ResultSet::ValidateField(Int32 idx) const
{
    ValidateCreated();
    m_Handle->ValidateField(idx);
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
Array ResultSet::FieldNames() const
{
    SQMOD_VALIDATE_CREATED(*this);
    // Grab the number of available fields
    const SQInteger fcount = ConvTo< SQInteger >::From(m_Handle->mFieldCount);
    // Grab the array with field instances
    const ResHnd::FieldType * fields = m_Handle->mFields;
    // Is there even something to process?
    if (!fcount || !fields)
    {
        return Array(DefaultVM::Get(), 0);
    }
    // Allocate an array with the same amount of elements as the number of fields
    Array arr(DefaultVM::Get(), fcount);
    // Iterate over all the available fields and insert them into the created array
    for (SQInteger n = 0; n < fcount; ++n)
    {
        arr.SetValue(n, (fields[n].name == nullptr) ? ToStrF("<field_%ld>", n) : fields[n].name);
    }
    // Return the resulted array
    return arr;
}

// ------------------------------------------------------------------------------------------------
Array ResultSet::GetFieldsArray() const
{
    SQMOD_VALIDATE_CREATED(*this);
    // Grab the number of available fields
    const SQInteger fcount = ConvTo< SQInteger >::From(m_Handle->mFieldCount);
    // Is there even something to process?
    if (!fcount)
    {
        return Array(DefaultVM::Get(), 0);
    }
    // Create a field instance to insert as copy
    Field field(m_Handle);
    // Allocate an array with the same amount of elements as the number of fields
    Array arr(DefaultVM::Get(), fcount);
    // Iterate over all the available fields and insert them into the created array
    for (SQInteger n = 0; n < fcount; ++n)
    {
        // Update the field index
        field.SetIndex(ConvTo< Int32 >::From(n));
        // Insert a copy of the field instance into the array
        arr.SetValue(n, field);
    }
    // Return the resulted array
    return arr;
}

// ------------------------------------------------------------------------------------------------
Table ResultSet::GetFieldsTable() const
{
    SQMOD_VALIDATE_CREATED(*this);
    // Grab the number of available fields
    const SQInteger fcount = ConvTo< SQInteger >::From(m_Handle->mFieldCount);
    // Grab the array with field instances
    const ResHnd::FieldType * fields = m_Handle->mFields;
    // Is there even something to process?
    if (!fcount || !fields)
    {
        return Table();
    }
    // Create a field instance to insert as copy
    Field field(m_Handle);
    // Allocate a table to be populated with field instances
    Table tbl;
    // Iterate over all the available fields and insert them into the created table
    for (SQInteger n = 0; n < fcount; ++n)
    {
        // Update the field index
        field.SetIndex(ConvTo< Int32 >::From(n));
        // Insert a copy of the field instance into the table
        tbl.SetValue((fields[n].name == nullptr) ? ToStrF("<field_%ld>", n) : fields[n].name, field);
    }
    // Return the resulted table
    return tbl;
}


// ================================================================================================
void Register_ResultSet(Table & sqlns)
{
    sqlns.Bind(_SC("ResultSet")
        , Class< ResultSet >(sqlns.GetVM(), _SC("SqMySQLResultSet"))
        // Constructors
        .Ctor()
        .Ctor< const ResultSet & >()
        // Core Meta-methods
        .Func(_SC("_cmp"), &ResultSet::Cmp)
        .SquirrelFunc(_SC("_typename"), &ResultSet::Typename)
        .Func(_SC("_tostring"), &ResultSet::ToString)
        // Properties
        .Prop(_SC("IsValid"), &ResultSet::IsValid)
        .Prop(_SC("FieldNames"), &ResultSet::FieldNames)
        .Prop(_SC("FieldsArray"), &ResultSet::GetFieldsArray)
        .Prop(_SC("FieldsTable"), &ResultSet::GetFieldsTable)
        .Prop(_SC("RowIndex"), &ResultSet::RowIndex)
        .Prop(_SC("RowCount"), &ResultSet::RowCount)
        // Member Methods
        .Func(_SC("Next"), &ResultSet::Next)
        .Func(_SC("Step"), &ResultSet::Next)
        .Func(_SC("SetRowIndex"), &ResultSet::SetRowIndex)
        .Func(_SC("SetLongRowIndex"), &ResultSet::SetLongRowIndex)
        .Func(_SC("Get"), &ResultSet::GetField)
        .Func(_SC("GetField"), &ResultSet::GetField)
        .Func(_SC("GetBool"), &ResultSet::GetBoolean)
        .Func(_SC("GetBoolean"), &ResultSet::GetBoolean)
        .Func(_SC("GetChar"), &ResultSet::GetChar)
        .Func(_SC("GetInteger"), &ResultSet::GetInteger)
        .Func(_SC("GetFloat"), &ResultSet::GetFloat)
        .Func(_SC("GetInt8"), &ResultSet::GetInt8)
        .Func(_SC("GetUint8"), &ResultSet::GetUint8)
        .Func(_SC("GetInt16"), &ResultSet::GetInt16)
        .Func(_SC("GetUint16"), &ResultSet::GetUint16)
        .Func(_SC("GetInt32"), &ResultSet::GetInt32)
        .Func(_SC("GetUint32"), &ResultSet::GetUint32)
        .Func(_SC("GetInt64"), &ResultSet::GetInt64)
        .Func(_SC("GetUint64"), &ResultSet::GetUint64)
        .Func(_SC("GetFloat32"), &ResultSet::GetFloat32)
        .Func(_SC("GetFloat64"), &ResultSet::GetFloat64)
        .Func(_SC("GetString"), &ResultSet::GetString)
        .Func(_SC("GetBuffer"), &ResultSet::GetBuffer)
        .Func(_SC("GetBlob"), &ResultSet::GetBlob)
    );
}

} // Namespace:: SqMod
