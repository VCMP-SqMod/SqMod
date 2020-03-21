#pragma once

// ------------------------------------------------------------------------------------------------
#include "Library/XML/Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * A helper for working with text inside PCDATA nodes.
*/
class XmlText
{
    // --------------------------------------------------------------------------------------------
    friend class XmlNode;

protected:

    // --------------------------------------------------------------------------------------------
    typedef xml_text Type;

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    XmlText(const DocumentRef doc, const Type & text)
        : m_Doc(doc), m_Text(text)
    {
        /* ... */
    }

private:

    // ---------------------------------------------------------------------------------------------
    DocumentRef m_Doc; // The main xml document instance.
    Type        m_Text; // The managed document node.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    XmlText()
        : m_Doc(), m_Text()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    XmlText(const XmlText & o)
        : m_Doc(o.m_Doc), m_Text(o.m_Text)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~XmlText()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    XmlText & operator = (const XmlText & o)
    {
        m_Doc = o.m_Doc;
        m_Text = o.m_Text;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const XmlText & o);
    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return m_Text.get();
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
     * See whether the text is empty.
    */
    bool IsEmpty() const
    {
        return m_Text.empty();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the text value.
    */
    CSStr GetValue() const
    {
        return m_Text.get();
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a string or the specified default value if empty.
    */
    CSStr AsString(CSStr def) const
    {
        return m_Text.as_string(def);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a integer or the specified default value if empty.
    */
    Int32 AsInt(Int32 def) const
    {
        return m_Text.as_int(def);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a unsigned integer or the specified default value if empty.
    */
    Uint32 AsUint(Uint32 def) const
    {
        return m_Text.as_uint(def);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a floating point or the specified default value if empty.
    */
    SQFloat AsFloat(SQFloat def) const
    {
        return static_cast< SQFloat >(m_Text.as_float(static_cast< Float32 >(def)));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a double floating point or the specified default value if empty.
    */
    SQFloat AsDouble(SQFloat def) const
    {
        return static_cast< SQFloat >(m_Text.as_double(static_cast< Float64 >(def)));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a long integer or the specified default value if empty.
    */
    LightObj AsLong(const SLongInt & def) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a unsigned long integer or the specified default value if empty.
    */
    LightObj AsUlong(const ULongInt & def) const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a boolean or the specified default value if empty.
    */
    bool AsBool(bool def) const
    {
        return m_Text.as_bool(def);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a string.
    */
    bool ApplyString(CSStr value)
    {
        return m_Text.set(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a integer.
    */
    bool ApplyInt(Int32 value)
    {
        return m_Text.set(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a unsigned integer.
    */
    bool ApplyUint(Uint32 value)
    {
        return m_Text.set(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a floating point.
    */
    bool ApplyFloat(SQFloat value)
    {
        return m_Text.set(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a double floating point.
    */
    bool ApplyDouble(SQFloat value)
    {
        return m_Text.set(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a long integer.
    */
    bool ApplyLong(const SLongInt & value);

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a unsigned long integer.
    */
    bool ApplyUlong(const ULongInt & value);

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a boolean.
    */
    bool ApplyBool(bool value)
    {
        return m_Text.set(value);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a string.
    */
    CSStr GetString() const
    {
        return m_Text.as_string();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a string.
    */
    void SetString(CSStr value)
    {
        m_Text = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a integer.
    */
    Int32 GetInt() const
    {
        return m_Text.as_int();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a integer.
    */
    void SetInt(Int32 value)
    {
        m_Text = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a unsigned integer.
    */
    Uint32 GetUint() const
    {
        return m_Text.as_uint();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a unsigned integer.
    */
    void SetUint(Uint32 value)
    {
        m_Text = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a floating point.
    */
    SQFloat GetFloat() const
    {
        return static_cast< SQFloat >(m_Text.as_float());
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a floating point.
    */
    void SetFloat(SQFloat value)
    {
        m_Text = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a double floating point.
    */
    SQFloat GetDouble() const
    {
        return static_cast< SQFloat >(m_Text.as_double());
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a double floating point.
    */
    void SetDouble(SQFloat value)
    {
        m_Text = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a long integer.
    */
    LightObj GetLong() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a long integer.
    */
    void SetLong(const SLongInt & value);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a unsigned long integer.
    */
    LightObj GetUlong() const;

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a unsigned long integer.
    */
    void SetUlong(const ULongInt & value);

    /* --------------------------------------------------------------------------------------------
     * Retrieve the value as a boolean.
    */
    bool GetBool() const
    {
        return m_Text.as_bool();
    }

    /* --------------------------------------------------------------------------------------------
     * Modify the value as a boolean.
    */
    void SetBool(bool value)
    {
        m_Text = value;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the data node (node_pcdata or node_cdata) for this object.
    */
    XmlNode GetData() const;
};

} // Namespace:: SqMod
