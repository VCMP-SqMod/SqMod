// ------------------------------------------------------------------------------------------------
#include "Handle/Statement.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <cstring>
#include <cstdlib>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
void StmtHnd::Validate() const
{
    // Is the handle valid?
    if ((m_Hnd == nullptr) || (m_Hnd->mPtr == nullptr))
    {
        STHROWF("Invalid MySQL statement reference");
    }
}

// ------------------------------------------------------------------------------------------------
void StmtHnd::ValidateIndex(Uint32 idx) const
{
    // Is the handle valid?
    if ((m_Hnd == nullptr) || (m_Hnd->mPtr == nullptr))
    {
        STHROWF("Invalid MySQL statement reference");
    }
    else if (idx >= m_Hnd->mParams)
    {
        STHROWF("Parameter index is out of range: %u >= %lu", idx, m_Hnd->mParams);
    }
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    void StmtHnd::Handle::ThrowCurrent(CCStr act, CCStr file, Int32 line)
#else
    void StmtHnd::Handle::ThrowCurrent(CCStr act)
#endif // _DEBUG
{
    // Grab the error number and message
    mErrNo = mysql_stmt_errno(mPtr);
    mErrStr.assign(mysql_stmt_error(mPtr));
    // Throw the exception with the resulted message
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
    throw Sqrat::Exception(FmtStr("%s (%u) : %s =>[%s:%d]", act,
                            mErrNo, mErrStr.c_str(), file, line));
#else
    throw Sqrat::Exception(FmtStr("%s (%u) : %s", act,
                            mErrNo, mErrStr.c_str()));
#endif // _DEBUG
}

// ------------------------------------------------------------------------------------------------
void StmtHnd::Bind::SetInput(enum_field_types type, BindType * bind, CCStr buffer, Ulong length)
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
StmtHnd::Handle::Handle(const ConnHnd & conn, CSStr query)
    : mPtr(mysql_stmt_init(conn))
    , mRef(1)
    , mErrNo(0)
    , mParams(0)
    , mBinds(nullptr)
    , mMyBinds(nullptr)
    , mConnection(conn)
    , mQuery(query ? query : _SC(""))
{
    // Validate the obtained statement handle
    if (!mPtr)
    {
        THROW_CURRENT(mConnection, "Cannot initialize statement");
    }
    // Attempt to prepare the statement
    else if (mysql_stmt_prepare(mPtr, mQuery.c_str(), mQuery.size()))
    {
        THROW_CURRENT_HND((*this), "Cannot prepare statement");
    }
    // Retrieve the amount of parameters supported by this statement
    mParams = mysql_stmt_param_count(mPtr);
    // Are there any parameters to allocate?
    if (mParams <= 0)
    {
        return;
    }
    // Allocate the necessary structures
    mBinds = new Bind[mParams];
    mMyBinds = new BindType[mParams];
    // Reset the allocated structures
    std::memset(mBinds, 0, sizeof(Bind) * mParams);
    std::memset(mMyBinds, 0, sizeof(BindType) * mParams);
}

// ------------------------------------------------------------------------------------------------
StmtHnd::Handle::~Handle()
{
    // Should delete native bindings?
    if (mMyBinds)
    {
        delete [] mMyBinds;
    }
    // Should we delete binding wrappers?
    if (mBinds)
    {
        delete [] mBinds;
    }
    // Should we release any statement?
    if (mPtr)
    {
        mysql_stmt_close(mPtr);
    }
}

// ------------------------------------------------------------------------------------------------


} // Namespace:: SqMod
