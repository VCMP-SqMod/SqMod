#pragma once

// ------------------------------------------------------------------------------------------------
#include "Library/XML/Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class that can read/write and alter the contents of XML files.
*/
class XmlDocument
{
protected:

    // --------------------------------------------------------------------------------------------
    typedef xml_document Type;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    XmlDocument(const XmlDocument & o);

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    XmlDocument & operator = (const XmlDocument & o);

    /* --------------------------------------------------------------------------------------------
     * See if the document is allowed to overwrite its contents and throw an error if not.
    */
    void CanLoad() const;

private:

    // ---------------------------------------------------------------------------------------------
    DocumentRef  m_Doc; // The main xml document instance.

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    XmlDocument()
        : m_Doc(nullptr)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~XmlDocument()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const XmlDocument & o) const
    {
        if (m_Doc && !o.m_Doc)
        {
            return 1;
        }
        else if (!m_Doc && o.m_Doc)
        {
            return -1;
        }
        else if (!m_Doc && !o.m_Doc)
        {
            return 0;
        }
        else if (*m_Doc == *o.m_Doc)
        {
            return 0;
        }
        else if (*m_Doc > *o.m_Doc)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        // Do we manage a valid document?
        if (m_Doc)
        {
            return m_Doc->name();
        }
        // Default to an empty name
        return _SC("");
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
     * Removes all nodes, leaving the empty document.
    */
    void Reset()
    {
        // Validate the document handle
        m_Doc.Validate();
        // Perform the requested operation
        m_Doc->reset();
    }

    /* --------------------------------------------------------------------------------------------
     * Removes all nodes, then copies the entire contents of the specified document.
    */
    void Reset(const XmlDocument & doc)
    {
        // Validate the document handles
        m_Doc.Validate();
        doc.m_Doc.Validate();
        // Perform the requested operation
        m_Doc->reset(*(doc.m_Doc));
    }

    /* --------------------------------------------------------------------------------------------
     * Load document from zero-terminated string. (LoadString collides with the windows api)
    */
    XmlParseResult LoadData(CSStr source)
    {
        // Make sure that we are allowed to load in data
        CanLoad();
        // Perform the requested operation and return the result
        return XmlParseResult(m_Doc, m_Doc->load_string(source));
    }

    /* --------------------------------------------------------------------------------------------
     * Load document from zero-terminated string. (LoadString collides with the windows api)
    */
    XmlParseResult LoadData(CSStr source, Uint32 options)
    {
        // Make sure that we are allowed to load in data
        CanLoad();
        // Perform the requested operation and return the result
        return XmlParseResult(m_Doc, m_Doc->load_string(source, options));
    }

    /* --------------------------------------------------------------------------------------------
     * Load document from file on disk.
    */
    XmlParseResult LoadFile(CSStr filepath)
    {
        // Make sure that we are allowed to load in data
        CanLoad();
        // Perform the requested operation and return the result
        return XmlParseResult(m_Doc, m_Doc->load_file(filepath));
    }

    /* --------------------------------------------------------------------------------------------
     * Load document from file on disk.
    */
    XmlParseResult LoadFile(CSStr filepath, Uint32 options)
    {
        // Make sure that we are allowed to load in data
        CanLoad();
        // Perform the requested operation and return the result
        return XmlParseResult(m_Doc, m_Doc->load_file(filepath, options));
    }

    /* --------------------------------------------------------------------------------------------
     * Load document from file on disk.
    */
    XmlParseResult LoadFile(CSStr filepath, Uint32 options, Int32 encoding)
    {
        // Make sure that we are allowed to load in data
        CanLoad();
        // Perform the requested operation and return the result
        return XmlParseResult(m_Doc, m_Doc->load_file(filepath, options,
                                                      static_cast< xml_encoding >(encoding)));
    }

    /* --------------------------------------------------------------------------------------------
     * Save XML to file on disk.
    */
    void SaveFile(CSStr filepath)
    {
        // Validate the document handle
        m_Doc.Validate();
        // Perform the requested operation
        m_Doc->save_file(filepath);
    }

    /* --------------------------------------------------------------------------------------------
     * Save XML to file on disk.
    */
    void SaveFile(CSStr filepath, CSStr indent)
    {
        // Validate the document handle
        m_Doc.Validate();
        // Perform the requested operation
        m_Doc->save_file(filepath, indent);
    }

    /* --------------------------------------------------------------------------------------------
     * Save XML to file on disk.
    */
    void SaveFile(CSStr filepath, CSStr indent, Uint32 format)
    {
        // Validate the document handle
        m_Doc.Validate();
        // Perform the requested operation
        m_Doc->save_file(filepath, indent, format);
    }

    /* --------------------------------------------------------------------------------------------
     * Save XML to file on disk.
    */
    void SaveFile(CSStr filepath, CSStr indent, Uint32 format, Int32 encoding)
    {
        // Validate the document handle
        m_Doc.Validate();
        // Perform the requested operation
        m_Doc->save_file(filepath, indent, format, static_cast< xml_encoding >(encoding));
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the document root node.
    */
    XmlNode GetNode() const;
};

} // Namespace:: SqMod
