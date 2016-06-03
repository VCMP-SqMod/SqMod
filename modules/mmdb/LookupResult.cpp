// ------------------------------------------------------------------------------------------------
#include "LookupResult.hpp"

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger LookupResult::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMMDBLookupResult");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
void LookupResult::Validate() const
{
    // Is the document handle valid?
    if (!m_Db)
        STHROWF("Invalid Maxmind database reference");
}

// ------------------------------------------------------------------------------------------------
LookupResult::LookupResult()
    : m_Db(), m_Result()
{
    memset(&m_Result, 0, sizeof(Type));
}

// ------------------------------------------------------------------------------------------------
Int32 LookupResult::Cmp(const LookupResult & o) const
{
    if (m_Db == o.m_Db)
        return 0;
    else if (m_Db.DbPtr() > o.m_Db.DbPtr())
        return 1;
    else
        return -1;
}

// ------------------------------------------------------------------------------------------------
EntryDataList LookupResult::GetValueA(CSStr path, Array & arr) const
{

}

// ------------------------------------------------------------------------------------------------
EntryDataList LookupResult::GetValueT(CSStr path, Table & tbl) const
{
    
}

} // Namespace:: SqMod
