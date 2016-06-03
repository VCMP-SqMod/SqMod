#ifndef _SQXML_WRAPPER_PARSERESULT_HPP_
#define _SQXML_WRAPPER_PARSERESULT_HPP_

// ------------------------------------------------------------------------------------------------
#include "Handle/Document.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Allows the user to inspect the result of certain operations and act accordingly.
*/
class ParseResult
{
    // --------------------------------------------------------------------------------------------
    friend class Document;
    friend class Node;

protected:

    // --------------------------------------------------------------------------------------------
    typedef xml_parse_result Result;

    /* --------------------------------------------------------------------------------------------
     * Explicit constructor.
    */
    ParseResult(const DocumentRef doc, const Result & result)
        : m_Doc(doc), m_Result(result)
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
    Result      m_Result; /* The managed parse result. */

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    ParseResult()
        : m_Doc(), m_Result()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor. (disabled)
    */
    ParseResult(const ParseResult & o)
        : m_Doc(o.m_Doc), m_Result(o.m_Result)
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~ParseResult()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator. (disabled)
    */
    ParseResult & operator = (const ParseResult & o)
    {
        m_Doc = o.m_Doc;
        m_Result = o.m_Result;
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const ParseResult & o)
    {
        if (m_Result.status == o.m_Result.status)
        {
            return 0;
        }
        else if (m_Result.status > o.m_Result.status)
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
        return m_Result.description();
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
     * Cast to bool operator.
    */
    bool IsOk() const
    {
        return m_Result;
    }

    /* --------------------------------------------------------------------------------------------
     * Parsing status code.
    */
    Int32 GetStatus() const
    {
        return (Int32)m_Result.status;
    }

    /* --------------------------------------------------------------------------------------------
     * Last parsed offset. (in char_t units from start of input data)
    */
    SQInteger GetOffset() const
    {
        return (SQInteger)m_Result.offset;
    }

    /* --------------------------------------------------------------------------------------------
     * Source document encoding.
    */
    Int32 GetEncoding() const
    {
        return m_Result.encoding;
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve error description as a string.
    */
    CSStr GetDescription() const
    {
        return m_Result.description();
    }

    /* --------------------------------------------------------------------------------------------
     * Check the parse result and throw the necessary errors.
    */
    void Check() const;
};

} // Namespace:: SqMod

#endif // _SQXML_WRAPPER_PARSERESULT_HPP_
