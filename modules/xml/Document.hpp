#ifndef _XML_DOCUMENT_HPP_
#define _XML_DOCUMENT_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class that can read/write and alter the contents of XML files.
*/
class Document
{
protected:

    // --------------------------------------------------------------------------------------------
    typedef xml_document Type;

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    Document(const Document & o);

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    Document & operator = (const Document & o);

    /* --------------------------------------------------------------------------------------------
     * Validate the document reference and throw an error if invalid.
    */
    void Validate() const;

    /* --------------------------------------------------------------------------------------------
     * See if the document is allowed to overwrite its contents and throw an error if not.
    */
    void CanLoad() const;

private:

    // ---------------------------------------------------------------------------------------------
    DocumentRef  m_Doc; /* The main xml document instance. */

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Document()
        : m_Doc(NULL)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Document()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Document & o) const
    {
        if (m_Doc && !o.m_Doc)
            return 1;
        else if (!m_Doc && o.m_Doc)
            return -1;
        else if (!m_Doc && !o.m_Doc)
            return 0;
        else if (*m_Doc == *o.m_Doc)
            return 0;
        else if (*m_Doc > *o.m_Doc)
            return 1;
        else
            return -1;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        if (m_Doc)
            return m_Doc->name();
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
        Validate();
        // Perform the requested operation
        m_Doc->reset();
    }

    /* --------------------------------------------------------------------------------------------
     * Removes all nodes, then copies the entire contents of the specified document.
    */
    void Reset(const Document & doc)
    {
        // Validate the document handles
        Validate();
        doc.Validate();
        // Perform the requested operation
        m_Doc->reset(*doc.m_Doc);
    }

    /* --------------------------------------------------------------------------------------------
     * Load document from zero-terminated string. (LoadString collides with the windows api)
    */
    ParseResult LoadData(CSStr source)
    {
        // Make sure that we are allowed to load in data
        CanLoad();
        // Perform the requested operation and return the result
        return ParseResult(m_Doc, m_Doc->load_string(source));
    }

    /* --------------------------------------------------------------------------------------------
     * Load document from zero-terminated string. (LoadString collides with the windows api)
    */
    ParseResult LoadData(CSStr source, Uint32 options)
    {
        // Make sure that we are allowed to load in data
        CanLoad();
        // Perform the requested operation and return the result
        return ParseResult(m_Doc, m_Doc->load_string(source, options));
    }

    /* --------------------------------------------------------------------------------------------
     * Load document from file on disk.
    */
    ParseResult LoadFile(CSStr filepath)
    {
        // Make sure that we are allowed to load in data
        CanLoad();
        // Perform the requested operation and return the result
        return ParseResult(m_Doc, m_Doc->load_file(filepath));
    }

    /* --------------------------------------------------------------------------------------------
     * Load document from file on disk.
    */
    ParseResult LoadFile(CSStr filepath, Uint32 options)
    {
        // Make sure that we are allowed to load in data
        CanLoad();
        // Perform the requested operation and return the result
        return ParseResult(m_Doc, m_Doc->load_file(filepath, options));
    }

    /* --------------------------------------------------------------------------------------------
     * Load document from file on disk.
    */
    ParseResult LoadFile(CSStr filepath, Uint32 options, Int32 encoding)
    {
        // Make sure that we are allowed to load in data
        CanLoad();
        // Perform the requested operation and return the result
        return ParseResult(m_Doc, m_Doc->load_file(filepath, options, (xml_encoding)encoding));
    }

    /* --------------------------------------------------------------------------------------------
     * Save XML to file on disk.
    */
    void SaveFile(CSStr filepath)
    {
        // Validate the document handle
        Validate();
        // Perform the requested operation
        m_Doc->save_file(filepath);
    }

    /* --------------------------------------------------------------------------------------------
     * Save XML to file on disk.
    */
    void SaveFile(CSStr filepath, CSStr indent)
    {
        // Validate the document handle
        Validate();
        // Perform the requested operation
        m_Doc->save_file(filepath, indent);
    }

    /* --------------------------------------------------------------------------------------------
     * Save XML to file on disk.
    */
    void SaveFile(CSStr filepath, CSStr indent, Uint32 format)
    {
        // Validate the document handle
        Validate();
        // Perform the requested operation
        m_Doc->save_file(filepath, indent, format);
    }

    /* --------------------------------------------------------------------------------------------
     * Save XML to file on disk.
    */
    void SaveFile(CSStr filepath, CSStr indent, Uint32 format, Int32 encoding)
    {
        // Validate the document handle
        Validate();
        // Perform the requested operation
        m_Doc->save_file(filepath, indent, format, (xml_encoding)encoding);
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the document root node.
    */
    Node GetNode() const;
};

} // Namespace:: SqMod

#endif // _XML_DOCUMENT_HPP_
