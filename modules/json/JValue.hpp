#ifndef _SQJSON_JVALUE_HPP_
#define _SQJSON_JVALUE_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Allows management and interaction with a JSON value.
*/
class JValue
{
public:

    // --------------------------------------------------------------------------------------------
    typedef json_t          Type; // The managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type*           Pointer; // Pointer to the managed type.
    typedef const Type*     ConstPtr; // Constant pointer to the managed type.

    // --------------------------------------------------------------------------------------------
    typedef Type&           Reference; // Reference to the managed type.
    typedef const Type&     ConstRef; // Constant reference to the managed type.

private:

    // --------------------------------------------------------------------------------------------
    Pointer m_Ptr; // Pointer to the managed value instance.

    // --------------------------------------------------------------------------------------------
    static std::size_t s_Flags; // Global flags used when dumping to a string.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor. (null)
    */
    JValue()
        : m_Ptr(json_null())
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Pointer constructor.
    */
    JValue(Pointer ptr)
        : m_Ptr(json_incref(ptr))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Pointer constructor.
    */
    JValue(Pointer ptr, bool inc)
        : m_Ptr(inc ? json_incref(ptr) : ptr)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Null pointer constructor.
    */
    JValue(std::nullptr_t)
        : m_Ptr(json_null())
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Boolean constructor.
    */
    JValue(bool val)
        : m_Ptr(json_boolean(val))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * String constructor.
    */
    JValue(CSStr val, bool check = true)
        : m_Ptr(check ? json_string(val) : json_string_nocheck(val))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * String constructor.
    */
    JValue(CSStr val, std::size_t len, bool check = true)
        : m_Ptr(check ? json_stringn(val, len) : json_stringn_nocheck(val,len))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * 32 bit signed integer constructor.
    */
    JValue(Int32 val)
        : m_Ptr(json_integer(ConvTo< json_int_t >::From(val)))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * 64 bit signed integer constructor.
    */
    JValue(Int64 val)
        : m_Ptr(json_integer(ConvTo< json_int_t >::From(val)))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * 32 bit floating point number constructor.
    */
    JValue(Float32 val)
        : m_Ptr(json_real(val))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * 64 bit floating point number constructor.
    */
    JValue(Float64 val)
        : m_Ptr(json_real(val))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    JValue(const JValue & o)
        : m_Ptr(json_incref(o.m_Ptr))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    JValue(JValue && o)
        : m_Ptr(o.m_Ptr)
    {
        // Prevent further interaction
        o.m_Ptr = nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~JValue()
    {
        // Decrease the reference count of the managed value
        json_decref(m_Ptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    JValue & operator = (const JValue & o)
    {
        // Avoid self assignment
        if (m_Ptr != o.m_Ptr)
        {
            // Release the current object
            json_decref(m_Ptr);
            // Grab the reference of the new object
            m_Ptr = json_incref(o.m_Ptr);
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Move assignment operator.
    */
    JValue & operator = (JValue && o)
    {
        // Avoid self assignment
        if (m_Ptr != o.m_Ptr)
        {
            // Release the current object
            json_decref(m_Ptr);
            // Steal reference
            m_Ptr = o.m_Ptr;
            // Prevent further interaction
            o.m_Ptr = nullptr;
        }
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const JValue & o) const
    {
        if (json_equal(m_Ptr, o.m_Ptr))
        {
            return 0;
        }
        else if (m_Ptr > o.m_Ptr)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    Object ToString() const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed JSON value.
    */
    operator Pointer ()
    {
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Implicit conversion to the managed JSON value.
    */
    operator ConstPtr () const
    {
        return m_Ptr;
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the managed value is valid.
    */
    bool IsValid() const
    {
        return (m_Ptr != nullptr);
    }


};

} // Namespace:: SqMod

#endif // _SQJSON_JVALUE_HPP_
