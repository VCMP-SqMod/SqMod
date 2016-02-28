#ifndef _XML_ATTRIBUTE_HPP_
#define _XML_ATTRIBUTE_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * A light-weight handle for manipulating attributes in DOM tree.
*/
class Attribute
{
    // --------------------------------------------------------------------------------------------
    friend class Node;

protected:

    // --------------------------------------------------------------------------------------------
    typedef xml_attribute Type;

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    Attribute(const DocumentRef doc, const Type & attr)
        : m_Doc(doc), m_Attr(attr)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Validate the document reference and throw an error if invalid.
    */
    void Validate() const;

private:

    // ---------------------------------------------------------------------------------------------
    DocumentRef m_Doc; /* The main xml document instance. */
    Type        m_Attr; /* The managed node attribute. */

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Attribute()
        : m_Doc(), m_Attr()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    Attribute(const Attribute & o)
        : m_Doc(o.m_Doc), m_Attr(o.m_Attr)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Attribute()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    Attribute & operator = (const Attribute & o)
    {
        m_Doc = o.m_Doc;
        m_Attr = o.m_Attr;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Attribute & o)
    {
        if (m_Attr == o.m_Attr)
            return 0;
        else if (m_Attr > o.m_Attr)
            return 1;
        else
            return -1;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return m_Attr.value();
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * See whether this instance references a valid xml document.
    */
    bool IsValid() const
    {
        return m_Doc;
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to this document instance.
    */
    Uint32 GetRefCount() const
    {
        return m_Doc.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * See whether the attribute is empty.
    */
    bool IsEmpty() const
    {
        return m_Attr.empty();
    }

    /* --------------------------------------------------------------------------------------------
     * Get hash value (unique for handles to the same object).
    */
    SQInteger GetHashValue() const
    {
        return (SQInteger)m_Attr.hash_value();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve attribute name.
    */
    CSStr GetName() const
    {
        return m_Attr.name();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve attribute name.
    */
    void SetName(CSStr name)
    {
        if (!m_Attr.set_name(name))
            SqThrowF("Unable to set xml attribute name");
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the attribute name.
    */
    bool ApplyName(CSStr name)
    {
        return m_Attr.set_name(name);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve attribute value.
    */
    CSStr GetValue() const
    {
        return m_Attr.value();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve attribute value.
    */
    void SetValue(CSStr name)
    {
        if (!m_Attr.set_value(name))
            SqThrowF("Unable to set xml attribute value");
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the attribute value.
    */
    bool ApplyValue(CSStr value)
    {
        return m_Attr.set_value(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a string or the specified default value if empty.
    */
    CSStr AsString(CSStr def) const
    {
        return m_Attr.as_string(def);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a integer or the specified default value if empty.
    */
    Int32 AsInt(Int32 def) const
    {
        return m_Attr.as_int(def);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a unsigned integer or the specified default value if empty.
    */
    Uint32 AsUint(Uint32 def) const
    {
        return m_Attr.as_uint(def);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a floating point or the specified default value if empty.
    */
    SQFloat AsFloat(SQFloat def) const
    {
        return (SQFloat)m_Attr.as_float(def);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a double floating point or the specified default value if empty.
    */
    SQFloat AsDouble(SQFloat def) const
    {
        return (SQFloat)m_Attr.as_double(def);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a long integer or the specified default value if empty.
    */
    Object AsLong(Object & def) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a unsigned long integer or the specified default value if empty.
    */
    Object AsUlong(Object & def) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a boolean or the specified default value if empty.
    */
    bool AsBool(bool def) const
    {
        return m_Attr.as_bool(def);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a string.
    */
    bool ApplyString(CSStr value)
    {
        return m_Attr.set_value(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a integer.
    */
    bool ApplyInt(Int32 value)
    {
        return m_Attr.set_value(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a unsigned integer.
    */
    bool ApplyUint(Uint32 value)
    {
        return m_Attr.set_value(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a floating point.
    */
    bool ApplyFloat(SQFloat value)
    {
        return m_Attr.set_value(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a double floating point.
    */
    bool ApplyDouble(SQFloat value)
    {
        return m_Attr.set_value(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a long integer.
    */
    bool ApplyLong(Object & value);

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a unsigned long integer.
    */
    bool ApplyUlong(Object & value);

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a boolean.
    */
    bool ApplyBool(bool value)
    {
        return m_Attr.set_value(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a string.
    */
    CSStr GetString() const
    {
        return m_Attr.as_string();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a string.
    */
    void SetString(CSStr value)
    {
        m_Attr = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a integer.
    */
    Int32 GetInt() const
    {
        return m_Attr.as_int();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a integer.
    */
    void SetInt(Int32 value)
    {
        m_Attr = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a unsigned integer.
    */
    Uint32 GetUint() const
    {
        return m_Attr.as_uint();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a unsigned integer.
    */
    void SetUint(Uint32 value)
    {
        m_Attr = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a floating point.
    */
    SQFloat GetFloat() const
    {
        return (SQFloat)m_Attr.as_float();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a floating point.
    */
    void SetFloat(SQFloat value)
    {
        m_Attr = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a double floating point.
    */
    SQFloat GetDouble() const
    {
        return (SQFloat)m_Attr.as_double();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a double floating point.
    */
    void SetDouble(SQFloat value)
    {
        m_Attr = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a long integer.
    */
    Object GetLong() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a long integer.
    */
    void SetLong(Object & value);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a unsigned long integer.
    */
    Object GetUlong() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a unsigned long integer.
    */
    void SetUlong(Object & value);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a boolean.
    */
    bool GetBool() const
    {
        return m_Attr.as_bool();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a boolean.
    */
    void SetBool(bool value)
    {
        m_Attr = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve next attribute in the attribute list of the parent node.
    */
    Attribute NextAttribute() const
    {
        return Attribute(m_Doc, m_Attr.next_attribute());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve previous attribute in the attribute list of the parent node.
    */
    Attribute PrevAttribute() const
    {
        return Attribute(m_Doc, m_Attr.previous_attribute());
    }
};

} // Namespace:: SqMod

#endif // _XML_ATTRIBUTE_HPP_