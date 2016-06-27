// ------------------------------------------------------------------------------------------------
#include "Handle/Statement.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <cstring>
#include <cstdlib>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
void StmtBind::SetInput(enum_field_types type, BindType * bind, CCStr buffer, Ulong length)
{
    // Associate the library bind point with our bind wrapper
    mBind = bind;
    // Assign the 64 bit unsigned integer as the default buffer
    mBind->buffer = &mUint64;
    // Match the bind point type to the one from the field
    mBind->buffer_type = type;
    // Default to n empty buffer until type identification
    mBind->buffer_length = 0;
    // Allow the library to specify whether the value is null
    mBind->is_null = &mIsNull;
    // Allow the library to specify if errors occurred
    mBind->error = &mError;
    // Tell the library where to read the buffer size
    mBind->length = &(mBind->buffer_length);
    // Identify and configure the parameter type
    switch (type)
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
            // Allocate a buffer to match the specified size
            mData.Adjust(length);
            // Should we copy anything into the buffer?
            if (buffer)
            {
                mData.Append(buffer, length);
            }
            // Assign the buffer as the new bind point buffer
            mBind->buffer = mData.Data();
            // Assign the buffer cursor position as the new bind point size
            mBind->buffer_length = mData.Position();
        } break;
        default: STHROWF("Unknown MySQL parameter type");
    }
}

// ------------------------------------------------------------------------------------------------
void StmtHnd::GrabCurrent()
{
    mErrNo = mysql_stmt_errno(mPtr);
    mErrStr.assign(mysql_stmt_error(mPtr));
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void StmtHnd::ThrowCurrent(CCStr act, CCStr file, Int32 line)
{
    GrabCurrent();
    // Throw the exception with the resulted message
    throw Sqrat::Exception(FmtStr("%s (%u) : %s =>[%s:%d]", act,
                            mErrNo, mErrStr.c_str(), file, line));
}
#else
void StmtHnd::ThrowCurrent(CCStr act)
{
    GrabCurrent();
    // Throw the exception with the resulted message
    throw Sqrat::Exception(FmtStr("%s (%u) : %s", act,
                            mErrNo, mErrStr.c_str()));
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void StmtHnd::ValidateParam(Uint32 idx, CCStr file, Int32 line) const
{
    // Is the handle valid?
    if (mPtr == nullptr)
    {
        STHROWF("Invalid MySQL statement reference =>[%s:%d]", file, line);
    }
    else if (idx >= mParams)
    {
        STHROWF("Parameter index is out of range: %u >= %lu =>[%s:%d]", idx, mParams, file, line);
    }
}
#else
void StmtHnd::ValidateParam(Uint32 idx) const
{
    // Is the handle valid?
    if (mPtr == nullptr)
    {
        STHROWF("Invalid MySQL statement reference");
    }
    else if (idx >= mParams)
    {
        STHROWF("Parameter index is out of range: %u >= %lu", idx, mParams);
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
StmtHnd::StmtHnd()
    : mPtr(nullptr)
    , mErrNo(0)
    , mErrStr(_SC(""))
    , mParams(0)
    , mBinds(nullptr)
    , mMyBinds(nullptr)
    , mConnection()
    , mQuery(_SC(""))
{

}

// ------------------------------------------------------------------------------------------------
StmtHnd::~StmtHnd()
{
    // Should delete native bindings?
    if (mMyBinds)
    {
        delete [] (mMyBinds);
    }
    // Should we delete binding wrappers?
    if (mBinds)
    {
        delete [] (mBinds);
    }
    // Should we release any statement?
    if (mPtr)
    {
        mysql_stmt_close(mPtr);
    }
}

// ------------------------------------------------------------------------------------------------
void StmtHnd::Create(const ConnRef & conn, CSStr query)
{
    // Is this statement already created?
    if (mPtr != nullptr)
    {
        STHROWF("MySQL statement was already created");
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
    // Validate the specified query string
    else if (!query || *query == '\0')
    {
        STHROWF("Invalid or empty MySQL query");
    }
    // Store the connection handle and query string
    mConnection = conn;
    mQuery.assign(query);
    // Attempt to initialize the statement handle
    mPtr = mysql_stmt_init(mConnection->mPtr);
    // Validate the obtained statement handle
    if (!mPtr)
    {
        SQMOD_THROW_CURRENT(*mConnection, "Cannot initialize MySQL statement");
    }
    // Attempt to prepare the statement with the given query
    else if (mysql_stmt_prepare(mPtr, mQuery.c_str(), mQuery.size()))
    {
        SQMOD_THROW_CURRENT(*this, "Cannot prepare MySQL statement");
    }
    // Retrieve the amount of parameters supported by this statement
    mParams = mysql_stmt_param_count(mPtr);
    // Are there any parameters to allocate?
    if (mParams <= 0)
    {
        // We're done here!
        return;
    }
    // Allocate the binding wrappers
    mBinds = new StmtBind[mParams];
    // Validate the allocated memory
    if (!mBinds)
    {
        STHROWF("Unable to allocate MySQL bind point wrappers");
    }
    // Allocate the binding points
    mMyBinds = new BindType[mParams];
    // Validate the allocated memory
    if (!mMyBinds)
    {
        STHROWF("Unable to allocate MySQL bind point structures");
    }
    // Reset the allocated points
    std::memset(mMyBinds, 0, sizeof(BindType) * mParams);
}


} // Namespace:: SqMod
