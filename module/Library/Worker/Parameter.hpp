#pragma once

// ------------------------------------------------------------------------------------------------
#include "Base/Shared.hpp"
#include "Base/VecMap.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Used to transmit values between workers in a language agnostic way.
*/
struct Parameter
{
    enum
    {
        T_NULL=0, // Null/undefined type.
        T_INT, // Integer type.
        T_BOOL, // Boolean type.
        T_FLOAT, // Floating point type.
        T_STRING, // String type.
        T_ARRAY, // Array type.
        T_TABLE // Table type.
    };
    // --------------------------------------------------------------------------------------------
    using ArrayType = std::vector< Parameter >; // Parameter array.
    using TableType = VecMap< Parameter, Parameter >; // Parameter table.
    // --------------------------------------------------------------------------------------------
    uint32_t    mType; // Type of value stored in the parameter.
    uint32_t    mSize; // Container size. Mostly used for the string because there's space from padding.
    /* --------------------------------------------------------------------------------------------
     *
    */
    union {
        int64_t     mInt; // Parameter value represented as integer.
        uint64_t    mData; // Parameter value represented as raw bits.
        double      mFloat; // Parameter value represented as floating point.
        CStr        mString; // Parameter value represented as string pointer.
        ArrayType * mArray; // Parameter value represented as array pointer.
        TableType * mTable; // Parameter value represented as table pointer.
    };
    /* --------------------------------------------------------------------------------------------
     * Default constructor (null).
    */
    Parameter() noexcept
        : Parameter(nullptr)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Null constructor.
    */
    explicit Parameter(std::nullptr_t)
        : mType(T_NULL), mSize(0), mData(0ull)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Integer constructor.
    */
    explicit Parameter(int8_t v)
            : mType(T_INT), mSize(sizeof(v)), mInt(v)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Integer constructor.
    */
    explicit Parameter(uint8_t v)
            : mType(T_INT), mSize(sizeof(v)), mInt(v)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Integer constructor.
    */
    explicit Parameter(int16_t v)
            : mType(T_INT), mSize(sizeof(v)), mInt(v)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Integer constructor.
    */
    explicit Parameter(uint16_t v)
            : mType(T_INT), mSize(sizeof(v)), mInt(v)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Integer constructor.
    */
    explicit Parameter(int32_t v)
            : mType(T_INT), mSize(sizeof(v)), mInt(v)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Integer constructor.
    */
    explicit Parameter(uint32_t v)
            : mType(T_INT), mSize(sizeof(v)), mInt(v)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Integer constructor.
    */
    explicit Parameter(int64_t v)
            : mType(T_INT), mSize(sizeof(v)), mInt(v)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Integer constructor.
    */
    explicit Parameter(uint64_t v)
            : mType(T_INT), mSize(sizeof(v)), mInt(static_cast< int64_t >(v))
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Boolean constructor.
    */
    explicit Parameter(bool SQ_UNUSED_ARG(v)) //static analyzer shat the bed
            : mType(T_BOOL), mSize(1), mInt(v ? 1 : 0)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Floating point constructor.
    */
    explicit Parameter(float v)
            : mType(T_FLOAT), mSize(sizeof(v)), mFloat(v)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Floating point constructor.
    */
    explicit Parameter(double v)
            : mType(T_FLOAT), mSize(sizeof(v)), mFloat(v)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * String constructor.
    */
    explicit Parameter(CCStr str);
    /* --------------------------------------------------------------------------------------------
     * String constructor.
    */
    explicit Parameter(CCStr str, size_t len);
    /* --------------------------------------------------------------------------------------------
     * Array constructor.
    */
    explicit Parameter(ArrayType && v);
    /* --------------------------------------------------------------------------------------------
     * Array constructor.
    */
    explicit Parameter(const ArrayType & v);
    /* --------------------------------------------------------------------------------------------
     * Table constructor.
    */
    explicit Parameter(TableType && v);
    /* --------------------------------------------------------------------------------------------
     * Table constructor.
    */
    explicit Parameter(const TableType & v);
    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Parameter(const Parameter & o);
    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    Parameter(Parameter && o);
    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Parameter()
    {
        Reset();
    }
    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Parameter & operator = (const Parameter & o)
    {
        Assign(o);
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    Parameter & operator = (Parameter && o)
    {
        Assign(std::forward< Parameter >(o));
        return *this;
    }
    /* --------------------------------------------------------------------------------------------
     * Equality comparison operator.
    */
    bool operator == (const Parameter & o) const noexcept;
    /* --------------------------------------------------------------------------------------------
     * Inequality comparison operator.
    */
    bool operator != (const Parameter & o) const noexcept
    {
        return !(*this == o);
    }
    /* --------------------------------------------------------------------------------------------
     * Discard any current information and set to null.
    */
    void Reset()
    {
        Clear(nullptr);
    }
    /* --------------------------------------------------------------------------------------------
     * Swap parameter contents.
    */
    void Swap(Parameter & o) noexcept
    {
        std::swap(mType, o.mType);
        std::swap(mSize, o.mSize);
        std::swap(mData, o.mData);
    }
    /* --------------------------------------------------------------------------------------------
     * Assign a copy of another parameter.
    */
    void Assign(const Parameter & o);
    /* --------------------------------------------------------------------------------------------
     * Assign a ownership of another parameter.
    */
    void Assign(Parameter && o);
private:
    /* --------------------------------------------------------------------------------------------
     * Discard any and all information without performing any release of memory.
    */
    void Discard() noexcept
    {
        mType = T_NULL;
        mSize = 0;
        mData = 0ull;
    }
    /* --------------------------------------------------------------------------------------------
     * Clear/release any stored value and reset to default. Does not set to null.
    */
    void Clear();
    /* --------------------------------------------------------------------------------------------
     * Clear/release any stored value and reset to default. Specialization which sets to null.
    */
    void Clear(std::nullptr_t)
    {
        Clear(); // Do a regular clear first
        Discard(); // Now we can forget about it
    }
};

} // Namespace:: SqMod
