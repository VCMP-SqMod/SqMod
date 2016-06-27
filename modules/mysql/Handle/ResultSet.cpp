// ------------------------------------------------------------------------------------------------
#include "Handle/ResultSet.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <cstring>
#include <cstdlib>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
void ResBind::SetOutput(const FieldType & field, BindType * bind)
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
        case MYSQL_TYPE_SET:
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
ResHnd::ResHnd()
    : mPtr(nullptr)
    , mFieldCount(0)
    , mLengths(nullptr)
    , mFields(nullptr)
    , mBinds(nullptr)
    , mMyBinds(nullptr)
    , mRow(nullptr)
    , mConnection()
    , mStatement()
    , mIndexes()
{
    /* ... */
}

// ------------------------------------------------------------------------------------------------
ResHnd::~ResHnd()
{
    // Is there a result-set that we should free?
    if (mPtr)
    {
        mysql_free_result(mPtr);
    }
    // Are there any bind points that we should free?
    if (mMyBinds)
    {
        delete [] (mMyBinds);
    }
    // Was this result-set from a statement?
    if (mStatement)
    {
        // Are there any rows pointers we should free?
        if (mRow)
        {
            delete [] (mRow);
        }
        // Free the result-set in the statement
        mysql_stmt_free_result(mStatement->mPtr);
    }
    // Are there any bind wrappers that we should free?
    if (mBinds)
    {
        delete [] (mBinds);
    }
}

// ------------------------------------------------------------------------------------------------
void ResHnd::GrabCurrent()
{
    if (mConnection)
    {
        mConnection->GrabCurrent();
    }
    else if (mStatement)
    {
        mStatement->GrabCurrent();
    }
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void ResHnd::ThrowCurrent(CCStr act, CCStr file, Int32 line)
{
    GrabCurrent();
    // Throw the exception with the resulted message
    if (mConnection)
    {
        mConnection->ThrowCurrent(act, file, line);
    }
    else if (mStatement)
    {
        mStatement->ThrowCurrent(act, file, line);
    }
}
#else
void ResHnd::ThrowCurrent(CCStr act)
{
    GrabCurrent();
    // Throw the exception with the resulted message
    if (mConnection)
    {
        mConnection->ThrowCurrent(act);
    }
    else if (mStatement)
    {
        mStatement->ThrowCurrent(act);
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void ResHnd::ValidateField(Uint32 idx, CCStr file, Int32 line) const
{
    // Is the handle valid?
    if (mPtr == nullptr)
    {
        STHROWF("Invalid MySQL result-set reference =>[%s:%d]", file, line);
    }
    else if (idx >= mFieldCount)
    {
        STHROWF("Field index is out of range: %u >= %lu =>[%s:%d]", idx, mFieldCount, file, line);
    }
}
#else
void ResHnd::ValidateField(Uint32 idx) const
{
    // Is the handle valid?
    if (mPtr == nullptr)
    {
        STHROWF("Invalid MySQL result-set reference");
    }
    else if (idx >= mFieldCount)
    {
        STHROWF("Field index is out of range: %u >= %lu", idx, mFieldCount);
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
void ResHnd::Create(const ConnRef & conn)
{
    // Is this result-set already created?
    if (mPtr != nullptr)
    {
        STHROWF("MySQL result-set was already created");
    }
    // Validate the specified connection handle
    else if (!conn)
    {
        STHROWF("Invalid MySQL connection reference");
    }
    else if (conn->mPtr == nullptr)
    {
        STHROWF("Invalid MySQL connection");
    }
    // Store the connection handle
    mConnection = conn;
    // Retrieve the complete result-set to the client, if any
    mPtr = mysql_store_result(mConnection->mPtr);
    // Did this query return any results?
    if (!mPtr)
    {
        return; // We're done here!
    }
    // Obtain the number of fields in the result-set
    mFieldCount = mysql_num_fields(mPtr);
    // Obtain an array representing the fields in the result-set
    mFields = mysql_fetch_fields(mPtr);
    // Associate the field names with their index
    for (Uint32 i = 0; i < mFieldCount; ++i)
    {
        mIndexes[mFields[i].name] = i;
    }
}

// ------------------------------------------------------------------------------------------------
void ResHnd::Create(const StmtRef & stmt)
{
    // Is this result-set already created?
    if (mPtr != nullptr)
    {
        STHROWF("MySQL result-set was already created");
    }
    // Validate the given statement handle
    else if (!stmt)
    {
        STHROWF("Invalid MySQL statement reference");
    }
    else if (stmt->mPtr == nullptr)
    {
        STHROWF("Invalid MySQL statement");
    }
    // Store the statement handle
    mStatement = stmt;
    // Set the parameter value for the next operation
    int max_length = 1;
    // Force mysql_stmt_store_result() to update the meta-data MYSQL_FIELD->max_length value
    if (mysql_stmt_attr_set(mStatement->mPtr, STMT_ATTR_UPDATE_MAX_LENGTH, &max_length) != 0)
    {
        SQMOD_THROW_CURRENT(*mStatement, "Cannot apply MySQL statement attribute");
    }
    //  Attempt to buffer the complete result-set on the client
    if (mysql_stmt_store_result(mStatement->mPtr))
    {
        SQMOD_THROW_CURRENT(*mStatement, "Cannot buffer MySQL result-set");
    }
    // Obtain the number of fields in the result-set
    mFieldCount = mysql_stmt_field_count(mStatement->mPtr);
    // Obtain the result-set meta-data
    mPtr = mysql_stmt_result_metadata(mStatement->mPtr);
    // Obtain an array representing the fields in the result-set
    mFields = mysql_fetch_fields(mPtr);
    // Are there any fields to allocate
    if (mFieldCount > 0)
    {
        // Allocate the bind wrappers
        mBinds = new ResBind[mFieldCount];
        // Validate the allocated structures
        if (!mBinds)
        {
            STHROWF("Unable to allocate MySQL bind point wrappers");
        }
        // Allocate the bind points
        mMyBinds = new BindType[mFieldCount];
        // Validate the allocated structures
        if (!mMyBinds)
        {
            STHROWF("Unable to allocate MySQL bind point structures");
        }
        // Allocate the row pointers
        mRow = new CStr[mFieldCount];
        // Validate the allocated structures
        if (!mRow)
        {
            STHROWF("Unable to allocate MySQL row pointers");
        }
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
    if (mFieldCount > 0 && mysql_stmt_bind_result(mStatement->mPtr, mMyBinds) != 0)
    {
        SQMOD_THROW_CURRENT(*mStatement, "Cannot bind MySQL variables to statement");
    }
}

} // Namespace:: SqMod
