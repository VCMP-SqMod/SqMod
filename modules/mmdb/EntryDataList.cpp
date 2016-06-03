// ------------------------------------------------------------------------------------------------
#include "EntryDataList.hpp"

// ------------------------------------------------------------------------------------------------
#include <cstdio>
#include <cstdlib>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
SQInteger EntryDataList::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMMDBEntryDataList");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
void EntryDataList::Validate() const
{
    // Is the document handle valid?
    if (!m_Db)
        STHROWF("Invalid Maxmind database reference");
    // Do we have a valid list?
    else if (!m_List)
        STHROWF("Invalid entry data list");
}

// ------------------------------------------------------------------------------------------------
void EntryDataList::ValidateElem() const
{
    // Is the document handle valid?
    if (!m_Db)
        STHROWF("Invalid Maxmind database reference");
    // Do we have a valid list?
    else if (!m_List)
        STHROWF("Invalid entry data list");
    // Do we have a valid element?
    else if (!m_Elem)
        STHROWF("Invalid entry data element");
}

// ------------------------------------------------------------------------------------------------
void EntryDataList::ValidateData() const
{
    // Is the document handle valid?
    if (!m_Db)
        STHROWF("Invalid Maxmind database reference");
    // Do we have a valid list?
    else if (!m_List)
        STHROWF("Invalid entry data list");
    // Do we have a valid element?
    else if (!m_Elem)
        STHROWF("Invalid entry data element");
    // Do we have some valid data?
    else if (!m_Elem->entry_data.has_data)
        STHROWF("Entry data element has no data");
}

// ------------------------------------------------------------------------------------------------
CSStr EntryDataList::AsTypeStr(Uint32 id)
{
    switch (id)
    {
        case MMDB_DATA_TYPE_EXTENDED:       return _SC("extended");
        case MMDB_DATA_TYPE_POINTER:        return _SC("pointer");
        case MMDB_DATA_TYPE_UTF8_STRING:    return _SC("string");
        case MMDB_DATA_TYPE_DOUBLE:         return _SC("double");
        case MMDB_DATA_TYPE_BYTES:          return _SC("bytes");
        case MMDB_DATA_TYPE_UINT16:         return _SC("uint16");
        case MMDB_DATA_TYPE_UINT32:         return _SC("uint32");
        case MMDB_DATA_TYPE_MAP:            return _SC("map");
        case MMDB_DATA_TYPE_INT32:          return _SC("int32");
        case MMDB_DATA_TYPE_UINT64:         return _SC("uint64");
        case MMDB_DATA_TYPE_UINT128:        return _SC("uint128");
        case MMDB_DATA_TYPE_ARRAY:          return _SC("array");
        case MMDB_DATA_TYPE_CONTAINER:      return _SC("container");
        case MMDB_DATA_TYPE_END_MARKER:     return _SC("endmarker");
        case MMDB_DATA_TYPE_BOOLEAN:        return _SC("boolean");
        case MMDB_DATA_TYPE_FLOAT:          return _SC("float");
        default:                            return _SC("unknonw");
    }
}

// ------------------------------------------------------------------------------------------------
EntryDataList::~EntryDataList()
{
    // Do we have to free any list?
    if (m_List)
        MMDB_free_entry_data_list(m_List);
}

// ------------------------------------------------------------------------------------------------
Int32 EntryDataList::Cmp(const EntryDataList & o) const
{
    if (m_List == o.m_List)
        return 0;
    else if (m_List > o.m_List)
        return 1;
    else
        return -1;
}

// ------------------------------------------------------------------------------------------------
Uint32 EntryDataList::GetCount() const
{
    // Do we even have a list?
    if (!m_List)
        return 0;
    // Get the start of the list
    Pointer elem = m_List;
    // Prepare a counter
    Uint32 count = 1;
    // Loop through list elements
    while ((elem = elem->next)) ++count;
    // Return the counter
    return count;
}

// ------------------------------------------------------------------------------------------------
bool EntryDataList::Next()
{
    // Validate the database and list handle
    Validate();
    // Do we have a valid element currently?
    if (m_Elem)
        return (m_Elem = m_Elem->next);
    // Nothing to advance
    return false;
}

// ------------------------------------------------------------------------------------------------
bool EntryDataList::Advance(Int32 n)
{
    // Validate the database and list handle
    Validate();
    // Do we have a valid element currently?
    if (m_Elem)
        // Attempt to skip as many elements as possible
        while ((n > 0) && (m_Elem = m_Elem->next)) --n;
    // Return whether we have a valid element
    return m_Elem;
}

// ------------------------------------------------------------------------------------------------
void EntryDataList::Reset()
{
    // Validate the database and list handle
    Validate();
    // Go back to the first element
    m_Elem = m_List;
}

// ------------------------------------------------------------------------------------------------
CSStr EntryDataList::GetString() const
{
    // Validate the database, list and element handle
    ValidateData();
    // Attempt to perform the requested conversion
    switch (m_Elem->entry_data.type)
    {
        case MMDB_DATA_TYPE_UTF8_STRING:
            return m_Elem->entry_data.utf8_string;
        case MMDB_DATA_TYPE_DOUBLE:
            return FmtStr("%f", m_Elem->entry_data.double_value);
        case MMDB_DATA_TYPE_UINT16:
            return FmtStr("%u", m_Elem->entry_data.uint16);
        case MMDB_DATA_TYPE_UINT32:
            return FmtStr("%u", m_Elem->entry_data.uint32);
        case MMDB_DATA_TYPE_INT32:
            return FmtStr("%d", m_Elem->entry_data.int32);
        case MMDB_DATA_TYPE_UINT64:
            return FmtStr("%llu", m_Elem->entry_data.uint64);
        case MMDB_DATA_TYPE_BOOLEAN:
            return m_Elem->entry_data.boolean ? _SC("true") : _SC("false");
        case MMDB_DATA_TYPE_FLOAT:
            return FmtStr("%f", m_Elem->entry_data.float_value);
        default:
            STHROWF("Unsupported conversion from (%s) to (string)", AsTypeStr(m_Elem->entry_data.type));
    }
    // Shouldn't really reach this point
    return _SC("");
}

// ------------------------------------------------------------------------------------------------
SQInteger EntryDataList::GetInteger() const
{
    // Validate the database, list and element handle
    ValidateData();
    // Attempt to perform the requested conversion
    switch (m_Elem->entry_data.type)
    {
#ifdef _SQ64
        case MMDB_DATA_TYPE_UTF8_STRING:
            return strtoll(m_Elem->entry_data.utf8_string, NULL, 10);
        case MMDB_DATA_TYPE_DOUBLE:
            return llround(m_Elem->entry_data.double_value);
        case MMDB_DATA_TYPE_UINT16:
            return m_Elem->entry_data.uint16;
        case MMDB_DATA_TYPE_UINT32:
            return m_Elem->entry_data.uint32;
        case MMDB_DATA_TYPE_INT32:
            return m_Elem->entry_data.int32;
        case MMDB_DATA_TYPE_UINT64:
            return Clamp(m_Elem->entry_data.uint64, 0, Uint64(NumLimit< SQInteger >::Max));
        case MMDB_DATA_TYPE_BOOLEAN:
            return m_Elem->entry_data.boolean ? 1 : 0;
        case MMDB_DATA_TYPE_FLOAT:
            return llround(m_Elem->entry_data.float_value);
        default:
            STHROWF("Unsupported conversion from (%s) to (int32)", AsTypeStr(m_Elem->entry_data.type));
#else
        case MMDB_DATA_TYPE_UTF8_STRING:
            return strtol(m_Elem->entry_data.utf8_string, NULL, 10);
        case MMDB_DATA_TYPE_DOUBLE:
            return lround(m_Elem->entry_data.double_value);
        case MMDB_DATA_TYPE_UINT16:
            return m_Elem->entry_data.uint16;
        case MMDB_DATA_TYPE_UINT32:
            return Clamp(m_Elem->entry_data.uint32, 0U, Uint32(NumLimit< SQInteger >::Max));
        case MMDB_DATA_TYPE_INT32:
            return m_Elem->entry_data.int32;
        case MMDB_DATA_TYPE_UINT64:
            return Clamp(m_Elem->entry_data.uint64, 0ULL, Uint64(NumLimit< SQInteger >::Max));
        case MMDB_DATA_TYPE_BOOLEAN:
            return m_Elem->entry_data.boolean ? 1 : 0;
        case MMDB_DATA_TYPE_FLOAT:
            return lround(m_Elem->entry_data.float_value);
        default:
            STHROWF("Unsupported conversion from (%s) to (int64)", AsTypeStr(m_Elem->entry_data.type));
#endif // _SQ64
    }
    // Shouldn't really reach this point
    return 0;
}

// ------------------------------------------------------------------------------------------------
SQFloat EntryDataList::GetFloat() const
{
    // Validate the database, list and element handle
    ValidateData();
    // Attempt to perform the requested conversion
    switch (m_Elem->entry_data.type)
    {
#ifdef SQUSEDOUBLE
        case MMDB_DATA_TYPE_UTF8_STRING:
            return static_cast< SQFloat >(strtod(m_Elem->entry_data.utf8_string, NULL));
#else
            return static_cast< SQFloat >(strtof(m_Elem->entry_data.utf8_string, NULL));
#endif // SQUSEDOUBLE
        case MMDB_DATA_TYPE_DOUBLE:
            return static_cast< SQFloat >(m_Elem->entry_data.double_value);
        case MMDB_DATA_TYPE_UINT16:
            return static_cast< SQFloat >(m_Elem->entry_data.uint16);
        case MMDB_DATA_TYPE_UINT32:
            return static_cast< SQFloat >(round(m_Elem->entry_data.uint32));
        case MMDB_DATA_TYPE_INT32:
            return static_cast< SQFloat >(round(m_Elem->entry_data.int32));
        case MMDB_DATA_TYPE_UINT64:
            return static_cast< SQFloat >(round(m_Elem->entry_data.uint64));
        case MMDB_DATA_TYPE_BOOLEAN:
            return m_Elem->entry_data.boolean ? 1.0 : 0.0;
        case MMDB_DATA_TYPE_FLOAT:
            return static_cast< SQFloat >(m_Elem->entry_data.float_value);
        default:
            STHROWF("Unsupported conversion from (%s) to (float)", AsTypeStr(m_Elem->entry_data.type));
    }
    // Shouldn't really reach this point
    return 0.0;
}

// ------------------------------------------------------------------------------------------------
Object EntryDataList::GetLong() const
{
    // Validate the database, list and element handle
    ValidateData();
    // Where the long number is retrieved
    Uint64 longint = 0;
    Int64 slong = 0;
    // Attempt to perform the requested conversion
    switch (m_Elem->entry_data.type)
    {
        case MMDB_DATA_TYPE_UTF8_STRING:
            longint = strtoull(m_Elem->entry_data.utf8_string, NULL, 10);
        break;
        case MMDB_DATA_TYPE_DOUBLE:
        {
            slong = llround(m_Elem->entry_data.double_value);
            longint = slong >= 0 ? slong : 0;
        } break;
        case MMDB_DATA_TYPE_UINT16:
            longint = m_Elem->entry_data.uint16;
        break;
        case MMDB_DATA_TYPE_UINT32:
            longint = m_Elem->entry_data.uint32;
        break;
        case MMDB_DATA_TYPE_INT32:
            longint = m_Elem->entry_data.int32 >= 0 ? m_Elem->entry_data.int32 : 0;
        break;
        case MMDB_DATA_TYPE_UINT64:
            longint = m_Elem->entry_data.uint64;
        break;
        case MMDB_DATA_TYPE_BOOLEAN:
            longint = m_Elem->entry_data.boolean ? 1 : 0;
        break;
        case MMDB_DATA_TYPE_FLOAT:
        {
            slong = llround(m_Elem->entry_data.float_value);
            longint = slong >= 0 ? slong : 0;
        }
        break;
        default:
            STHROWF("Unsupported conversion from (%s) to (uint64)", AsTypeStr(m_Elem->entry_data.type));
    }
    // Obtain the initial stack size
    const StackGuard sg(_SqVM);
    // Push a long integer instance with the requested value on the stack
    _SqMod->PushULongObject(_SqVM, longint);
    // Get the object from the stack and return it
    return Var< Object >(_SqVM, -1).value;
}

// ------------------------------------------------------------------------------------------------
bool EntryDataList::GetBool() const
{
    // Validate the database, list and element handle
    ValidateData();
    // Attempt to perform the requested conversion
    switch (m_Elem->entry_data.type)
    {
        case MMDB_DATA_TYPE_UTF8_STRING:
            return !!(m_Elem->entry_data.utf8_string);
        case MMDB_DATA_TYPE_DOUBLE:
            return EpsGt(m_Elem->entry_data.double_value, 0.0d);
        case MMDB_DATA_TYPE_UINT16:
            return (m_Elem->entry_data.uint16 > 0);
        case MMDB_DATA_TYPE_UINT32:
            return (m_Elem->entry_data.uint32 > 0);
        case MMDB_DATA_TYPE_INT32:
            return (m_Elem->entry_data.int32 > 0);
        case MMDB_DATA_TYPE_UINT64:
            return (m_Elem->entry_data.uint64 > 0);
        case MMDB_DATA_TYPE_BOOLEAN:
            return m_Elem->entry_data.boolean;
        case MMDB_DATA_TYPE_FLOAT:
            return EpsGt(m_Elem->entry_data.float_value, 0.0f);
        default:
            STHROWF("Unsupported conversion from (%s) to (boolean)", AsTypeStr(m_Elem->entry_data.type));
    }
    // Shouldn't really reach this point
    return false;
}

// ------------------------------------------------------------------------------------------------
void EntryDataList::DumpTo(CSStr filepath, Int32 indent) const
{
    // Validate the database and list handle
    Validate();
    // Validate the specified file path
    if (!filepath || strlen(filepath) <= 0)
        STHROWF("Invalid file path");
    // Attempt to open the specified file
    FILE * fp = fopen(filepath, "w");
    // Validate the file handle
    if (!fp)
        STHROWF("Unable to open file %s", filepath);
    // Attempt to dump the entry data list
    Int32 status = MMDB_dump_entry_data_list(fp, m_List, indent);
    // Close the file handle
    fclose(fp);
    // Validate the result of the operation
    if (status != MMDB_SUCCESS)
        // Now it's safe to throw the error
        STHROWF("Unable to dump the list [%s]", MMDB_strerror(status));
}

} // Namespace:: SqMod
