// ------------------------------------------------------------------------------------------------
#include "Entries.hpp"
#include "Module.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
Int32 Entries::Cmp(const Entries & o) const
{
    if (m_Elem == o.m_Elem)
        return 0;
    else if (this > &o)
        return 1;
    else
        return -1;
}

// ------------------------------------------------------------------------------------------------
void Entries::Next()
{
    // Are there any other elements ahead?
    if (!m_List.empty() && m_Elem != m_List.end())
        ++m_Elem; /* Go ahead one element */
}

// ------------------------------------------------------------------------------------------------
void Entries::Prev()
{
    // Are there any other elements behind?
    if (!m_List.empty() && m_Elem != m_List.begin())
        --m_Elem; /* Go back one element */
}

// ------------------------------------------------------------------------------------------------
void Entries::Advance(Int32 n)
{
    // Are there any other elements ahead?
    if (m_List.empty() || m_Elem == m_List.end())
        return;
    // Jump as many elements as possible within the specified distance
    while ((--n >= 0) && m_Elem != m_List.end()) ++m_Elem;
}

// ------------------------------------------------------------------------------------------------
void Entries::Retreat(Int32 n)
{
    // Are there any other elements behind?
    if (m_List.empty() || m_Elem == m_List.begin())
        return;
    // Jump as many elements as possible within the specified distance
    while ((--n >= 0) && m_Elem != m_List.begin()) --m_Elem;
}

// ------------------------------------------------------------------------------------------------
CSStr Entries::GetItem() const
{
    if (m_List.empty() || m_Elem == m_List.end())
        _SqMod->SqThrow("Invalid INI entry [item]");
    else
        return m_Elem->pItem;
    return _SC("");
}

// ------------------------------------------------------------------------------------------------
CSStr Entries::GetComment() const
{
    if (m_List.empty() || m_Elem == m_List.end())
        _SqMod->SqThrow("Invalid INI entry [comment]");
    else
        return m_Elem->pComment;
    return _SC("");
}

// ------------------------------------------------------------------------------------------------
Int32 Entries::GetOrder() const
{
    if (m_List.empty() || m_Elem == m_List.end())
        _SqMod->SqThrow("Invalid INI entry [order]");
    else
        return m_Elem->nOrder;
    return -1;
}

} // Namespace:: SqMod
