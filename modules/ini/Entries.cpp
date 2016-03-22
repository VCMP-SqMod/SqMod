// ------------------------------------------------------------------------------------------------
#include "Entries.hpp"
#include "Module.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger Entries::Typename(HSQUIRRELVM vm)
{
    static SQChar name[] = _SC("SqIniEntries");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
Int32 Entries::Cmp(const Entries & o) const
{
    if (m_Elem == o.m_Elem)
        return 0;
    else if (m_List.size() > o.m_List.size())
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
    // is the current element valid?
    if (m_List.empty() || m_Elem == m_List.end())
        STHROWF("Invalid INI entry [item]");
    // Return the requested information
    return m_Elem->pItem;
}

// ------------------------------------------------------------------------------------------------
CSStr Entries::GetComment() const
{
    // is the current element valid?
    if (m_List.empty() || m_Elem == m_List.end())
        STHROWF("Invalid INI entry [comment]");
    // Return the requested information
    return m_Elem->pComment;
}

// ------------------------------------------------------------------------------------------------
Int32 Entries::GetOrder() const
{
    // is the current element valid?
    if (m_List.empty() || m_Elem == m_List.end())
        STHROWF("Invalid INI entry [order]");
    // Return the requested information
    return m_Elem->nOrder;
}

} // Namespace:: SqMod
