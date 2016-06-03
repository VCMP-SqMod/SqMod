// ------------------------------------------------------------------------------------------------
#include "Handle/Result.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <cstring>
#include <cstdlib>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
void ResHnd::Validate() const
{
    // Is the handle valid?
    if ((m_Hnd == nullptr) || (m_Hnd->mPtr == nullptr))
    {
        STHROWF("Invalid MySQL query result reference");
    }
}

// ------------------------------------------------------------------------------------------------
void ResHnd::ValidateIndex(Uint32 idx) const
{
    // Is the handle valid?
    if ((m_Hnd == nullptr) || (m_Hnd->mPtr == nullptr))
    {
        STHROWF("Invalid MySQL query result reference");
    }
    else if (idx >= m_Hnd->mFieldCount)
    {
        STHROWF("The specified index is out of range: %u >= %u", idx, m_Hnd->mFieldCount);
    }
}

// ------------------------------------------------------------------------------------------------
void ResHnd::Bind::SetOutput(const FieldType & field, BindType * bind)
{
    // Associate the library bind point with our bind wrapper
    mBind = bind;
    // Assign the 64 bit unsigned integer as the default buffer
    mBind->buffer = &mUint64;
    // Match the bind point type to the one from the field
    mBind->buffer_type = field.type;
    // Default to n empty buffer until type identification
    mBind->buffer_length = 0;
    // Allow the library to specify whether the value is null
    mBind->is_null = &mIsNull;
    // Allow the library to specify if errors occurred
    mBind->error = &mError;
    // Tell the library where to read the buffer size
    mBind->length = &(mBind->buffer_length);
    // Identify and configure the field type
    switch (field.type)
    {
        case MYSQL_TYPE_NULL:
        case MYSQL_TYPE_TINY:
        case MYSQL_TYPE_BIT:
        {
            mBind->buffer_length = 1;
        } break;
        case MYSQL_TYPE_YEAR:
        case MYSQL_TYPE_SHORT:
        {
            mBind->buffer_length = sizeof(Int16);
        } break;
        case MYSQL_TYPE_INT24:
        case MYSQL_TYPE_LONG:
        {
            mBind->buffer_length = sizeof(Int32);
        } break;
        case MYSQL_TYPE_LONGLONG:
        {
            mBind->buffer_length = sizeof(Int64);
        } break;
        case MYSQL_TYPE_FLOAT:
        {
            mBind->buffer_length = sizeof(Float32);
        } break;
        case MYSQL_TYPE_DOUBLE:
        {
            mBind->buffer_length = sizeof(Float64);
        } break;
        case MYSQL_TYPE_NEWDATE:
        case MYSQL_TYPE_DATE:
        case MYSQL_TYPE_TIME:
        case MYSQL_TYPE_DATETIME:
        case MYSQL_TYPE_TIMESTAMP:
        {
            mBind->buffer = &mTime;
        } break;
        case MYSQL_TYPE_DECIMAL:
        case MYSQL_TYPE_NEWDECIMAL:
        case MYSQL_TYPE_TINY_BLOB:
        case MYSQL_TYPE_MEDIUM_BLOB:
        case MYSQL_TYPE_LONG_BLOB:
        case MYSQL_TYPE_BLOB:
        case MYSQL_TYPE_ENUM:
        case MYSQL_TYPE_VARCHAR:
        case MYSQL_TYPE_VAR_STRING:
        case MYSQL_TYPE_STRING:
        {
            // Allocate a buffer to match the field size
            mData.Adjust(field.max_length);
            // Assign the buffer as the new bind point buffer
            mBind->buffer = mData.Data();
            // Assign the buffer size as the new bind point size
            mBind->buffer_length = mData.Capacity();
        } break;
        default: STHROWF("Unknown MySQL field type");
    }
}

// ------------------------------------------------------------------------------------------------
ResHnd::Handle::Handle(const ConnHnd & conn)
    : mPtr(mysql_store_result(conn))
    , mRef(1)
    , mFieldCount(0)
    , mLengths(nullptr)
    , mFields(nullptr)
    , mBinds(nullptr)
    , mMyBinds(nullptr)
    , mRow(nullptr)
    , mStatement()
    , mIndexes()
{
    // Did this query return any results?
    if (!mPtr)
    {
        return; // We're done here!
    }
    // Obtain the number of fields in the result set
    mFieldCount = mysql_num_fields(mPtr);
    // Obtain an array representing the fields in the result set
    mFields = mysql_fetch_fields(mPtr);
    // Associate the field names with their index
    for (Uint32 i = 0; i < mFieldCount; ++i)
    {
        mIndexes[mFields[i].name] = i;
    }
}

// ------------------------------------------------------------------------------------------------
ResHnd::Handle::Handle(const StmtHnd & stmt)
    : mPtr(nullptr)
    , mRef(1)
    , mFieldCount(0)
    , mLengths(nullptr)
    , mFields(nullptr)
    , mBinds(nullptr)
    , mMyBinds(nullptr)
    , mRow(nullptr)
    , mStatement(stmt)
    , mIndexes()
{
    // Validate the given statement handle
    mStatement.Validate();
    // Set the parameter value for the next operation
    int max_length = 1;
    // Force mysql_stmt_store_result() to update the meta-data MYSQL_FIELD->max_length value
    if (mysql_stmt_attr_set(mStatement, STMT_ATTR_UPDATE_MAX_LENGTH, &max_length) != 0)
    {
        THROW_CURRENT(mStatement, "Cannot apply statement attribute");
    }
    //  Attempt to buffer the complete result set on the client
    if (mysql_stmt_store_result(mStatement))
    {
        THROW_CURRENT(mStatement, "Cannot buffer result-set");
    }
    // Obtain the number of fields in the result set
    mFieldCount = mysql_stmt_field_count(mStatement);
    // Obtain the result set meta-data
    mPtr = mysql_stmt_result_metadata(mStatement);
    // Obtain an array representing the fields in the result set
    mFields = mysql_fetch_fields(mPtr);
    // Are there any fields to allocate
    if (mFieldCount > 0)
    {
        // Allocate the bind wrappers
        mBinds = new Bind[mFieldCount];
        // Allocate the bind points
        mMyBinds = new BindType[mFieldCount];
        // Allocate the row pointers
        mRow = new CStr[mFieldCount];
        // Initialize the bind points to null
        std::memset(mMyBinds, 0, sizeof(BindType) * mFieldCount);
    }
    // Configure bind points and make associations
    for (Uint32 i = 0; i < mFieldCount; ++i)
    {
        // Associate the current field name with the current index
        mIndexes[mFields[i].name] = i;
        // Configure the current bind point according to the associated field
        mBinds[i].SetOutput(mFields[i], &mMyBinds[i]);
        // Store the bind point buffer into the associated row
        mRow[i] = mBinds[i].GetBuffer();
    }
    // Associate our bind points with the statement for result storage
    if (mFieldCount > 0 && mysql_stmt_bind_result(mStatement, mMyBinds) != 0)
    {
        THROW_CURRENT(mStatement, "Cannot bind variables to statement");
    }
}

// ------------------------------------------------------------------------------------------------
ResHnd::Handle::~Handle()
{
    // Is there a result set that we should free?
    if (mPtr)
    {
        mysql_free_result(mPtr);
    }
    // Are there any bind points that we should free?
    if (mMyBinds)
    {
        delete [] mMyBinds;
    }
    // Was this result set from a statement?
    if (mStatement)
    {
        // Are there any rows pointers we should free?
        if (mRow)
        {
            delete [] mRow;
        }
        // Free the result set in the statement
        mysql_stmt_free_result(mStatement);
    }
    // Are there any bind wrappers that we should free?
    if (mBinds)
    {
        delete [] mBinds;
    }
}

// ------------------------------------------------------------------------------------------------



} // Namespace:: SqMod
