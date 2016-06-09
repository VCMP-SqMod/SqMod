#ifndef _SQJSON_ARRAY_HPP_
#define _SQJSON_ARRAY_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Allows management and interaction with a JSON array.
*/
class JArray
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
    Pointer m_Ptr; // Pointer to the managed array instance.

    // --------------------------------------------------------------------------------------------
    static std::size_t s_Flags; // Global flags used when dumping to a string.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor. (empty)
    */
    JArray()
        : m_Ptr(json_array())
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Pointer constructor.
    */
    JArray(Pointer ptr)
        : m_Ptr(ptr)
    {
        if (json_is_array(m_Ptr))
        {
            json_incref(m_Ptr);
        }
        else
        {
            STHROWF("Expected JSON array got: %s", JSONTypeStr(m_Ptr));
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Pointer constructor.
    */
    JArray(Pointer ptr, bool inc)
        : m_Ptr(ptr)
    {
        if (json_is_array(m_Ptr))
        {
            if (inc)
            {
                json_incref(m_Ptr);
            }
        }
        else
        {
            STHROWF("Expected JSON array got: %s", JSONTypeStr(m_Ptr));
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    JArray(const JArray & o)
        : m_Ptr(json_incref(o.m_Ptr))
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Move constructor.
    */
    JArray(JArray && o)
        : m_Ptr(o.m_Ptr)
    {
        // Prevent further interaction
        o.m_Ptr = nullptr;
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~JArray()
    {
        // Decrease the reference count of the managed object
        json_decref(m_Ptr);
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    JArray & operator = (const JArray & o)
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
    JArray & operator = (JArray && o)
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
    Int32 Cmp(const JArray & o) const
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

#endif // _SQJSON_ARRAY_HPP_
