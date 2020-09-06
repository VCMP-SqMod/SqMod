// ------------------------------------------------------------------------------------------------
#include "Library/Worker/Parameter.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
Parameter::Parameter(CCStr str)
	: Parameter()
{
    // Make sure there's a string
    if (str)
    {
        mSize = static_cast< uint32_t >(std::strlen(str));
        mString = new SQChar[mSize+1];
        std::strcpy(mString, str);
    }
    // Even an empty string should still be marked as a string
    mType = T_STRING;
}
// ------------------------------------------------------------------------------------------------
Parameter::Parameter(CCStr str, size_t len)
	: Parameter()
{
    // Make sure there's a string
    if (str && len)
    {
        mSize = ConvTo< uint32_t >::From(len);
        mString = new SQChar[mSize+1];
        std::strncpy(mString, str, mSize);
        mString[mSize] = '\0'; // Null terminator
    }
    // Even an empty string should still be marked as a string
    mType = T_STRING;
}
// ------------------------------------------------------------------------------------------------
Parameter::Parameter(ArrayType && v)
    : mType(T_ARRAY), mSize(static_cast< uint32_t >(v.size()))
    , mArray(new ArrayType(std::forward< ArrayType >(v)))
{
}
// ------------------------------------------------------------------------------------------------
Parameter::Parameter(const ArrayType & v)
    : mType(T_ARRAY), mSize(static_cast< uint32_t >(v.size()))
    , mArray(new ArrayType(v))
{
}
// ------------------------------------------------------------------------------------------------
Parameter::Parameter(TableType && v)
    : mType(T_ARRAY), mSize(static_cast< uint32_t >(v.size()))
    , mTable(new TableType(std::forward< TableType >(v)))
{
}
// ------------------------------------------------------------------------------------------------
Parameter::Parameter(const TableType & v)
    : mType(T_ARRAY), mSize(static_cast< uint32_t >(v.size()))
    , mTable(new TableType(v))
{
}
// ------------------------------------------------------------------------------------------------
Parameter::Parameter(const Parameter & o)
    : mType(o.mType), mSize(o.mSize), mData(o.mData)
{
    // Identify the type to be copied
    switch (mType)
    {
        // Fundamental types can be copied bit-wise (which we did)
        case T_NULL:
        case T_INT:
        case T_BOOL:
        case T_FLOAT: break;
        case T_STRING:
            if (mSize)
            {
                mString = new SQChar[mSize];
                std::strncpy(mString, o.mString, mSize);
                mString[mSize] = '\0'; // Null terminator
            }
            else
            {
                mString = nullptr; // Empty string?
            }
            break;
        case T_ARRAY:
            mArray = o.mArray ? new ArrayType(*o.mArray) : nullptr;
            break;
        case T_TABLE:
            mTable = o.mTable ? new TableType(*o.mTable) : nullptr;
            break;
            // How did we get here?
        default: break;
    }
}
// ------------------------------------------------------------------------------------------------
Parameter::Parameter(Parameter && o)
    : mType(o.mType), mSize(o.mSize), mData(o.mData)
{
    o.Discard(); // Take ownership
}
// ------------------------------------------------------------------------------------------------
bool Parameter::operator == (const Parameter & o) const noexcept
{
    // If they're not the same type then there's no point in comparing
    if (mType != o.mType)
    {
        return false;
    }
    // Identify which type to compare
    switch (mType)
    {
        // Null is same regardless
        case T_NULL: return true;
        // Boolean is stored as integer
        case T_INT:
        case T_BOOL: return (mInt == o.mInt);
        // Take into account precision errors
        case T_FLOAT: return EpsEq(mFloat, o.mFloat);
        case T_STRING:
            // Only perform a comparison if there's actually a string to compare
            if (mSize && mSize == o.mSize)
            {
                return std::strncmp(mString, o.mString, mSize) == 0;
            }
            else
            {
                return false; // If they're not the same size then they can't be the same
            }
        // For table or arrays we only test if they're the same rather then each value individually
        case T_ARRAY: return (mArray == o.mArray);
        case T_TABLE: return (mTable == o.mTable);
        // How did we get here?
        default: return false;
    }
}
// ------------------------------------------------------------------------------------------------
void Parameter::Assign(const Parameter & o)
{
    // Avoid self assignment
    if (this == &o) return;
    /* We could probably optimize this by reusing current container memory.
     * But chances are we would complicate code for the simpler case.
     * And the simpler case is likely to be the more common scenario.
    */
    // Discard current information
    Clear();
    // The size and type are copied bit-wise
    mType = o.mType;
    mSize = o.mSize;
    // Identify the type to be copied
    switch (mType)
    {
        // Fundamental types can be copied bit-wise
        case T_NULL:
        case T_INT:
        case T_BOOL:
        case T_FLOAT:
            mData = o.mData;
        break;
        // Strings require memory to be allocated
        case T_STRING:
            if (mSize)
            {
                mString = new SQChar[mSize];
                std::strncpy(mString, o.mString, mSize);
                mString[mSize] = '\0'; // Null terminator
            }
            else
            {
                mString = nullptr; // Empty string?
            }
        break;
        case T_ARRAY:
            mArray = o.mArray ? new ArrayType(*o.mArray) : nullptr;
        break;
        case T_TABLE:
            mTable = o.mTable ? new TableType(*o.mTable) : nullptr;
        break;
            // How did we get here?
        default: break;
    }
}
// ------------------------------------------------------------------------------------------------
void Parameter::Assign(Parameter && o)
{
    // Avoid self assignment
    if (this == &o) return;
    // Discard current information
    Clear();
    // We don't care about the type since we take ownership
    mType = o.mType;
    mSize = o.mSize;
    mData = o.mData;
    // Take ownership
    o.Discard();
}
// ------------------------------------------------------------------------------------------------
void Parameter::Clear()
{
    switch (mType)
    {
        case T_STRING: delete[] mString; break;
        case T_ARRAY: delete mArray; break;
        case T_TABLE: delete mTable; break;
        default: break;
    }
}

} // Namespace:: SqMod
