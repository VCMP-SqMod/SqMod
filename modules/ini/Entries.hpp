#ifndef _SQINI_ENTRIES_HPP_
#define _SQINI_ENTRIES_HPP_

// ------------------------------------------------------------------------------------------------
#include "Common.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

/* ------------------------------------------------------------------------------------------------
 * Class that can access and iterate a series of entries in the INI document.
*/
class Entries
{
    // --------------------------------------------------------------------------------------------
    friend class Document;

protected:

    // --------------------------------------------------------------------------------------------
    typedef DocumentRef::Type::TNamesDepend     Container;

    // --------------------------------------------------------------------------------------------
    typedef Container::iterator                 Iterator;

    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    Entries(const DocumentRef & ini, Container & list)
        : m_Doc(ini), m_List(), m_Elem()
    {
        m_List.swap(list);
        Reset();
    }

private:

    // ---------------------------------------------------------------------------------------------
    DocumentRef     m_Doc; /* The document that contains the elements. */
    Container       m_List; /* The list of elements to iterate. */
    Iterator        m_Elem; /* The currently processed element. */

public:

    /* --------------------------------------------------------------------------------------------
     * Default constructor. (null)
    */
    Entries()
        : m_Doc(), m_List(), m_Elem(m_List.end())
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy constructor.
    */
    Entries(const Entries & o)
        : m_Doc(o.m_Doc), m_List(o.m_List), m_Elem()
    {
        Reset();
    }

    /* --------------------------------------------------------------------------------------------
     * Destructor.
    */
    ~Entries()
    {
        /* ... */
    }

    /* --------------------------------------------------------------------------------------------
     * Copy assignment operator.
    */
    Entries & operator = (const Entries & o)
    {
        m_Doc = o.m_Doc;
        m_List = o.m_List;
        Reset();
        return *this;
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to compare two instances of this type.
    */
    Int32 Cmp(const Entries & o) const;

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to convert an instance of this type to a string.
    */
    CSStr ToString() const
    {
        return GetItem();
    }

    /* --------------------------------------------------------------------------------------------
     * Used by the script engine to retrieve the name from instances of this type.
    */
    static SQInteger Typename(HSQUIRRELVM vm);

    /* --------------------------------------------------------------------------------------------
     * Return whether the current element is valid and can be accessed.
    */
    bool IsValid() const
    {
        return !(m_List.empty() || m_Elem == m_List.end());
    }

    /* --------------------------------------------------------------------------------------------
     * Return whether the entry list is empty.
    */
    bool IsEmpty() const
    {
        return m_List.empty();
    }

    /* --------------------------------------------------------------------------------------------
     * Return the number of active references to this document instance.
    */
    Uint32 GetRefCount() const
    {
        return m_Doc.Count();
    }

    /* --------------------------------------------------------------------------------------------
     * Return the total entries in the list.
    */
    Int32 GetSize() const
    {
        return (Int32)m_List.size();
    }

    /* --------------------------------------------------------------------------------------------
     * Reset the internal iterator to the first element.
    */
    void Reset()
    {
        if (m_List.empty())
            m_Elem = m_List.end();
        else
            m_Elem = m_List.begin();
    }

    /* --------------------------------------------------------------------------------------------
     * Go to the next element.
    */
    void Next();

    /* --------------------------------------------------------------------------------------------
     * Go to the previous element.
    */
    void Prev();

    /* --------------------------------------------------------------------------------------------
     * Advance a certain number of elements.
    */
    void Advance(Int32 n);

    /* --------------------------------------------------------------------------------------------
     * Retreat a certain number of elements.
    */
    void Retreat(Int32 n);

    /* --------------------------------------------------------------------------------------------
     * Sort the entries using the default options.
    */
    void Sort()
    {
        if (!m_List.empty())
            m_List.sort(DocumentRef::Type::Entry::KeyOrder());
    }

    /* --------------------------------------------------------------------------------------------
     * Sort the entries by name of key only.
    */
    void SortByKeyOrder()
    {
        if (!m_List.empty())
            m_List.sort(DocumentRef::Type::Entry::KeyOrder());
    }

    /* --------------------------------------------------------------------------------------------
     * Sort the entries by their load order and then name of key.
    */
    void SortByLoadOrder()
    {
        if (!m_List.empty())
            m_List.sort(DocumentRef::Type::Entry::LoadOrder());
    }

    /* --------------------------------------------------------------------------------------------
     * Retrieve the string value of the current element item.
    */
    CSStr GetItem() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the string value of the current element comment.
    */
    CSStr GetComment() const;

    /* --------------------------------------------------------------------------------------------
     * Retrieve the order of the current element.
    */
    Int32 GetOrder() const;
};

} // Namespace:: SqMod

#endif // _SQINI_ENTRIES_HPP_
