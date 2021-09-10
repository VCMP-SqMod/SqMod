// ------------------------------------------------------------------------------------------------
#include "Library/MMDB.hpp"

// ------------------------------------------------------------------------------------------------
#include <sqratConst.h>

// ------------------------------------------------------------------------------------------------
namespace SqMod {

// ------------------------------------------------------------------------------------------------
// N[0] - contains least significant bits, N[3] - most significant
static SQChar * Bin128ToDec(const uint32_t N[4])
{
    // log10(x) = log2(x) / log2(10) ~= log2(x) / 3.322
    static SQChar s[128 / 3 + 1 + 1];
    uint32_t n[4];
    SQChar * p = s;
    int i;

    std::memset(s, '0', sizeof(s) - 1);
    s[sizeof(s) - 1] = '\0';

    std::memcpy(n, N, sizeof(n));

    for (i = 0; i < 128; i++)
    {
        int j, carry;

        carry = (n[3] >= 0x80000000);
        // Shift n[] left, doubling it
        n[3] = ((n[3] << 1u) & 0xFFFFFFFFu) + (n[2] >= 0x80000000u);
        n[2] = ((n[2] << 1u) & 0xFFFFFFFFu) + (n[1] >= 0x80000000u);
        n[1] = ((n[1] << 1u) & 0xFFFFFFFFu) + (n[0] >= 0x80000000u);
        n[0] = ((n[0] << 1u) & 0xFFFFFFFFu);

        // Add s[] to itself in decimal, doubling it
        for (j = sizeof(s) - 2; j >= 0; j--)
        {
            s[j] += s[j] - '0' + carry; // NOLINT(cppcoreguidelines-narrowing-conversions)

            carry = (s[j] > '9');

            if (carry)
            {
                s[j] -= 10;
            }
        }
    }

    while ((p[0] == '0') && (p < &s[sizeof(s) - 2]))
    {
        p++;
    }

    return p;
}

// ------------------------------------------------------------------------------------------------
const SQChar * AsTypeStr(uint32_t id)
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
        default:                            return _SC("unknown");
    }
}

// ------------------------------------------------------------------------------------------------
bool GetEntryAsBool(const MMDB_entry_data_s & ed)
{
    bool value = false;
    // Identify the type of entry data
    switch (ed.type)
    {
        case MMDB_DATA_TYPE_POINTER: {
            value = ed.pointer > 0;
        } break;
        case MMDB_DATA_TYPE_UTF8_STRING: {
            if (ed.data_size > 0)
            {
                value = ConvTo< bool >::From(reinterpret_cast< const SQChar * >(ed.utf8_string));
            }
        } break;
        case MMDB_DATA_TYPE_DOUBLE: {
            value = ConvTo< bool >::From(ed.double_value);
        } break;
        case MMDB_DATA_TYPE_BYTES: {
            for (uint32_t i = 0; i < ed.data_size; ++i)
            {
                if (ed.bytes[i] != 0)
                {
                    value = true;
                    // Found something that isn't 0
                    break;
                }
            }
        } break;
        case MMDB_DATA_TYPE_UINT16: {
            value = ConvTo< bool >::From(ed.uint16);
        } break;
        case MMDB_DATA_TYPE_UINT32: {
            value = ConvTo< bool >::From(ed.uint16);
        } break;
        case MMDB_DATA_TYPE_INT32: {
            value = ConvTo< bool >::From(ed.uint16);
        } break;
        case MMDB_DATA_TYPE_UINT64: {
            value = ConvTo< bool >::From(ed.uint16);
        } break;
        case MMDB_DATA_TYPE_UINT128: {
#if defined(MMDB_UINT128_IS_BYTE_ARRAY) && (MMDB_UINT128_IS_BYTE_ARRAY == 1)
            for (uint32_t i = 0; i < sizeof(ed.uint128); ++i)
            {
                if (ed.uint128[i] != 0)
                {
                    value = true;
                    // Found somethinf that isn't 0
                    break;
                }
            }
#else
            value = ed.uint128 > 0;
#endif // MMDB_UINT128_IS_BYTE_ARRAY

        } break;
        case MMDB_DATA_TYPE_BOOLEAN: {
            value = ed.boolean;
        } break;
        case MMDB_DATA_TYPE_FLOAT: {
            value = ConvTo< bool >::From(ed.float_value);
        } break;
        default:
            STHROWF("Unsupported conversion from ({}) to (boolean)", AsTypeStr(ed.type));
    }
    // Return the extracted value
    return value;
}

// ------------------------------------------------------------------------------------------------
SQInteger GetEntryAsInteger(const MMDB_entry_data_s & ed)
{
    SQInteger value = 0;
    // Identify the type of entry data
    switch (ed.type)
    {
        case MMDB_DATA_TYPE_POINTER: {
            value = static_cast< SQInteger >(ed.pointer);
        } break;
        case MMDB_DATA_TYPE_UTF8_STRING: {
            if (ed.data_size > 0)
            {
                value = ConvTo< SQInteger >::From(reinterpret_cast< const SQChar * >(ed.utf8_string));
            }
        } break;
        case MMDB_DATA_TYPE_DOUBLE: {
            value = ConvTo< SQInteger >::From(ed.double_value);
        } break;
        case MMDB_DATA_TYPE_BYTES: {
            std::memcpy(&value, ed.bytes, Clamp(ed.data_size, 0U, sizeof(value)));
        } break;
        case MMDB_DATA_TYPE_UINT16: {
            value = ConvTo< SQInteger >::From(ed.uint16);
        } break;
        case MMDB_DATA_TYPE_UINT32: {
            value = ConvTo< SQInteger >::From(ed.uint32);
        } break;
        case MMDB_DATA_TYPE_INT32: {
            value = ConvTo< SQInteger >::From(ed.int32);
        } break;
        case MMDB_DATA_TYPE_UINT64: {
            value = ConvTo< SQInteger >::From(ed.uint64);
        } break;
        case MMDB_DATA_TYPE_UINT128: {
#if defined(MMDB_UINT128_IS_BYTE_ARRAY) && (MMDB_UINT128_IS_BYTE_ARRAY == 1)
            std::memcpy(&value, ed.uint128, sizeof(value));
#else
            std::memcpy(&value, &ed.uint128, sizeof(value));
#endif // MMDB_UINT128_IS_BYTE_ARRAY
        } break;
        case MMDB_DATA_TYPE_BOOLEAN: {
            value = ed.boolean ? 1 : 0;
        } break;
        case MMDB_DATA_TYPE_FLOAT: {
            value = ConvTo< SQInteger >::From(ed.float_value);
        } break;
        default:
            STHROWF("Unsupported conversion from ({}) to (integer)", AsTypeStr(ed.type));
    }
    // Return the extracted value
    return value;
}

// ------------------------------------------------------------------------------------------------
SQFloat GetEntryAsFloat(const MMDB_entry_data_s & ed)
{
    SQFloat value = 0.0;
    // Identify the type of entry data
    switch (ed.type)
    {
        case MMDB_DATA_TYPE_POINTER: {
            value = ConvTo< SQFloat >::From(static_cast< SQInteger >(ed.pointer));
        } break;
        case MMDB_DATA_TYPE_UTF8_STRING: {
            if (ed.data_size > 0)
            {
                value = ConvTo< SQFloat >::From(reinterpret_cast< const SQChar * >(ed.utf8_string));
            }
        } break;
        case MMDB_DATA_TYPE_DOUBLE: {
            value = ConvTo< SQFloat >::From(ed.double_value);
        } break;
        case MMDB_DATA_TYPE_BYTES: {
            // Not our problem if the result will be junk!
            std::memcpy(&value, ed.bytes, Clamp(ed.data_size, 0U, sizeof(value)));
        } break;
        case MMDB_DATA_TYPE_UINT16: {
            value = ConvTo< SQFloat >::From(ed.uint16);
        } break;
        case MMDB_DATA_TYPE_UINT32: {
            value = ConvTo< SQFloat >::From(ed.uint32);
        } break;
        case MMDB_DATA_TYPE_INT32: {
            value = ConvTo< SQFloat >::From(ed.int32);
        } break;
        case MMDB_DATA_TYPE_UINT64: {
            value = ConvTo< SQFloat >::From(ed.uint64);
        } break;
        case MMDB_DATA_TYPE_UINT128: {
            SQInteger num;
            // Convert to integer first
#if defined(MMDB_UINT128_IS_BYTE_ARRAY) && (MMDB_UINT128_IS_BYTE_ARRAY == 1)
            std::memcpy(&num, ed.uint128, sizeof(num));
#else
            std::memcpy(&num, &ed.uint128, sizeof(num));
#endif // MMDB_UINT128_IS_BYTE_ARRAY
            // Now convert to float
            value = ConvTo< SQFloat >::From(num);
        } break;
        case MMDB_DATA_TYPE_BOOLEAN: {
            value = ed.boolean ? 1.0 : 0.0;
        } break;
        case MMDB_DATA_TYPE_FLOAT: {
            value = ConvTo< SQFloat >::From(ed.float_value);
        } break;
        default:
            STHROWF("Unsupported conversion from ({}) to (float)", AsTypeStr(ed.type));
    }
    // Return the extracted value
    return value;
}

// ------------------------------------------------------------------------------------------------
SQInteger GetEntryAsLong(const MMDB_entry_data_s & ed)
{
    uint64_t value = 0;
    // Identify the type of entry data
    switch (ed.type)
    {
        case MMDB_DATA_TYPE_POINTER: {
            value = static_cast< uint64_t >(ed.pointer);
        } break;
        case MMDB_DATA_TYPE_UTF8_STRING: {
            if (ed.data_size > 0)
            {
                value = ConvTo< uint64_t >::From(reinterpret_cast< const SQChar * >(ed.utf8_string));
            }
        } break;
        case MMDB_DATA_TYPE_DOUBLE: {
            value = ConvTo< uint64_t >::From(ed.double_value);
        } break;
        case MMDB_DATA_TYPE_BYTES: {
            std::memcpy(&value, ed.bytes, Clamp(ed.data_size, 0U, sizeof(value)));
        } break;
        case MMDB_DATA_TYPE_UINT16: {
            value = ConvTo< uint64_t >::From(ed.uint16);
        } break;
        case MMDB_DATA_TYPE_UINT32: {
            value = ConvTo< uint64_t >::From(ed.uint32);
        } break;
        case MMDB_DATA_TYPE_INT32: {
            value = ConvTo< uint64_t >::From(ed.int32);
        } break;
        case MMDB_DATA_TYPE_UINT64: {
            value = ConvTo< uint64_t >::From(ed.uint64);
        } break;
        case MMDB_DATA_TYPE_UINT128: {
#if defined(MMDB_UINT128_IS_BYTE_ARRAY) && (MMDB_UINT128_IS_BYTE_ARRAY == 1)
            std::memcpy(&value, ed.uint128, sizeof(value));
#else
            std::memcpy(&value, &ed.uint128, sizeof(value));
#endif // MMDB_UINT128_IS_BYTE_ARRAY
        } break;
        case MMDB_DATA_TYPE_BOOLEAN: {
            value = ed.boolean ? 1 : 0;
        } break;
        case MMDB_DATA_TYPE_FLOAT: {
            value = ConvTo< uint64_t >::From(ed.float_value);
        } break;
        default:
            STHROWF("Unsupported conversion from ({}) to (long)", AsTypeStr(ed.type));
    }
    // Return a long integer instance with the requested value
    return static_cast< SQInteger >(value);
}

// ------------------------------------------------------------------------------------------------
LightObj GetEntryAsString(const MMDB_entry_data_s & ed)
{
    // Obtain the initial stack size
    const StackGuard sg;
    // The default vm
    HSQUIRRELVM vm = SqVM();
    // Identify the type of entry data
    switch (ed.type)
    {
        case MMDB_DATA_TYPE_POINTER: {
            sq_pushstring(vm, fmt::format(fmt::runtime("{:p}"), ed.pointer).c_str(), -1);
        } break;
        case MMDB_DATA_TYPE_UTF8_STRING: {
            sq_pushstring(vm, ed.utf8_string, ed.data_size);
        } break;
        case MMDB_DATA_TYPE_DOUBLE: {
            sq_pushstring(vm, fmt::format("{}", ed.double_value).c_str(), -1);
        } break;
        case MMDB_DATA_TYPE_BYTES: {
            sq_pushstring(vm, reinterpret_cast< const SQChar * >(ed.bytes), static_cast< SQInteger >(ed.data_size) / sizeof(SQChar));
        } break;
        case MMDB_DATA_TYPE_UINT16: {
            sq_pushstring(vm, fmt::format("{}", ed.uint16).c_str(), -1);
        } break;
        case MMDB_DATA_TYPE_UINT32: {
            sq_pushstring(vm, fmt::format("{}", ed.uint32).c_str(), -1);
        } break;
        case MMDB_DATA_TYPE_INT32: {
            sq_pushstring(vm, fmt::format("{}", ed.int32).c_str(), -1);
        } break;
        case MMDB_DATA_TYPE_UINT64: {
            sq_pushstring(vm, fmt::format("{}", ed.uint64).c_str(), -1);
        } break;
        case MMDB_DATA_TYPE_UINT128: {
#if defined(MMDB_UINT128_IS_BYTE_ARRAY) && (MMDB_UINT128_IS_BYTE_ARRAY == 1)
            sq_pushstring(vm, Bin128ToDec(reinterpret_cast< const uint32_t * >(ed.uint128)), -1);
#else
            sq_pushstring(vm, Bin128ToDec(reinterpret_cast< const uint32_t * >(&ed.uint128)), -1);
#endif // MMDB_UINT128_IS_BYTE_ARRAY
        } break;
        case MMDB_DATA_TYPE_BOOLEAN: {
            sq_pushstring(vm, ed.boolean ? _SC("true") : _SC("false"), -1);
        } break;
        case MMDB_DATA_TYPE_FLOAT: {
            sq_pushstring(vm, fmt::format("{}", ed.float_value).c_str(), -1);
        } break;
        default:
            STHROWF("Unsupported conversion from ({}) to (string)", AsTypeStr(ed.type));
    }
    // Obtain the object from the stack and return it
    return LightObj(-1);
}

// ------------------------------------------------------------------------------------------------
LightObj GetEntryAsBytes(const MMDB_entry_data_s & ed)
{
    // Identify the type of entry data
    switch (ed.type)
    {
        case MMDB_DATA_TYPE_POINTER: {
            return LightObj(SqTypeIdentity< SqBuffer >{}, SqVM(),
                    reinterpret_cast< const char * >(&ed.pointer), sizeof(ed.pointer), 0);
        }
        case MMDB_DATA_TYPE_UTF8_STRING: {
            return LightObj(SqTypeIdentity< SqBuffer >{}, SqVM(),
                    reinterpret_cast< const char * >(ed.utf8_string), ed.data_size, 0);
        }
        case MMDB_DATA_TYPE_DOUBLE: {
            return LightObj(SqTypeIdentity< SqBuffer >{}, SqVM(),
                    reinterpret_cast< const char * >(&ed.double_value), sizeof(ed.double_value), 0);
        }
        case MMDB_DATA_TYPE_BYTES: {
            return LightObj(SqTypeIdentity< SqBuffer >{}, SqVM(),
                    reinterpret_cast< const char * >(ed.bytes), ed.data_size, 0);
        }
        case MMDB_DATA_TYPE_UINT16: {
            return LightObj(SqTypeIdentity< SqBuffer >{}, SqVM(),
                    reinterpret_cast< const char * >(&ed.uint16), sizeof(ed.uint16), 0);
        }
        case MMDB_DATA_TYPE_UINT32: {
            return LightObj(SqTypeIdentity< SqBuffer >{}, SqVM(),
                    reinterpret_cast< const char * >(&ed.uint32), sizeof(ed.uint32), 0);
        }
        case MMDB_DATA_TYPE_INT32: {
            return LightObj(SqTypeIdentity< SqBuffer >{}, SqVM(),
                    reinterpret_cast< const char * >(&ed.int32), sizeof(ed.int32), 0);
        }
        case MMDB_DATA_TYPE_UINT64: {
            return LightObj(SqTypeIdentity< SqBuffer >{}, SqVM(),
                    reinterpret_cast< const char * >(&ed.uint64), sizeof(ed.uint64), 0);
        }
        case MMDB_DATA_TYPE_UINT128: {
            return LightObj(SqTypeIdentity< SqBuffer >{}, SqVM(),
                    reinterpret_cast< const char * >(&ed.uint128), sizeof(ed.uint128), 0);
        }
        case MMDB_DATA_TYPE_BOOLEAN: {
            return LightObj(SqTypeIdentity< SqBuffer >{}, SqVM(),
                    reinterpret_cast< const char * >(&ed.boolean), sizeof(ed.boolean), 0);
        }
        case MMDB_DATA_TYPE_FLOAT: {
            return LightObj(SqTypeIdentity< SqBuffer >{}, SqVM(),
                                 reinterpret_cast< const char * >(&ed.float_value),
                                 sizeof(ed.float_value), 0);
        }
        default:
            STHROWF("Unsupported conversion from ({}) to (buffer)", AsTypeStr(ed.type));
    }
    // Return a null object (shouldn't reach here)
    return LightObj{};
}

// ------------------------------------------------------------------------------------------------
SQInteger SockAddr::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMMSockAddr");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void SockAddr::Validate(const SQChar * file, int32_t line) const
{
    if (!m_Handle)
    {
        SqThrowF(SQMOD_RTFMT("Invalid sockaddr structure handle =>[{}:{}]"), file, line);
    }
}
#else
void SockAddr::Validate() const
{
    if (!m_Handle)
    {
        SqThrowF(fmt::runtime("Invalid sockaddr structure handle"));
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
SockAddr::Pointer SockAddr::GetValid(const SQChar * file, int32_t line) const
{
    Validate(file, line);
    return m_Handle;
}
#else
SockAddr::Pointer SockAddr::GetValid() const
{
    Validate();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
SockAddr::SockAddr(const SQChar * addr)
    : m_Handle(nullptr)
{
    struct addrinfo hints{};
    // Configure the hints structure
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_NUMERICHOST;
    // We set ai_socktype so that we only get one result back
    hints.ai_socktype = SOCK_STREAM;
    // Attempt to obtain information about the specified address
    int32_t status = getaddrinfo(addr, nullptr, &hints, &m_Handle);
    // Validate the success of the operation
    if (!status)
    {
        // See if we must free any handles (just in case)
        if (m_Handle)
        {
            freeaddrinfo(m_Handle);
        }
        // Now it's safe to throw the error
#if defined(UNICODE) || defined(_UNICODE)
        STHROWF("Unable to query the specified address for information [{}]", gai_strerrorA(status));
#else
        STHROWF("Unable to query the specified address for information [{}]", gai_strerror(status));
#endif
    }
    // Save the specified string address
    m_Addres.assign(addr ? addr : _SC(""));
}

// ------------------------------------------------------------------------------------------------
SockAddr::~SockAddr()
{
    if (m_Handle)
    {
        freeaddrinfo(m_Handle);
    }
}

// ------------------------------------------------------------------------------------------------
DbHnd::DbHnd(const SQChar * filepath, uint32_t flags)
    : mDb()
{
    // Validate the specified file path
    if (!filepath || *filepath == '\0')
    {
        STHROWF("Invalid database file path");
    }
    // Let's attempt to open the specified database
    const int32_t status = MMDB_open(filepath, flags, &mDb);
    // Validate the result of the operation
    if (status != MMDB_SUCCESS)
    {
        STHROWF("Unable to open the specified database [{}]", MMDB_strerror(status));
    }
}

// ------------------------------------------------------------------------------------------------
DbHnd::~DbHnd()
{
    // We don't need the database handle anymore
    MMDB_close(&mDb);
}

// ------------------------------------------------------------------------------------------------
SQInteger Database::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMMDatabase");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Database::Validate(const SQChar * file, int32_t line) const
{
    if (!m_Handle)
    {
        SqThrowF(SQMOD_RTFMT("Invalid Maxmind database reference =>[{}:{}]"), file, line);
    }
}
#else
void Database::Validate() const
{
    if (!m_Handle)
    {
        SqThrowF(fmt::runtime("Invalid Maxmind database reference"));
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const DbRef & Database::GetValid(const SQChar * file, int32_t line) const
{
    Validate(file, line);
    return m_Handle;
}
#else
const DbRef & Database::GetValid() const
{
    Validate();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
Metadata Database::GetMetadata() const
{
    return Metadata(m_Handle, &(SQMOD_GET_VALID(*this)->mDb).metadata);
}

// ------------------------------------------------------------------------------------------------
LightObj Database::GetMetadataAsEntryDataList() const
{
    MMDB_entry_data_list_s * entry_data_list = nullptr;
    // Attempt to retrieve the database meta-data as an entry data list
    const int status = MMDB_get_metadata_as_entry_data_list(&(SQMOD_GET_VALID(*this)->mDb), &entry_data_list);
    // Validate the status code
    if (status != MMDB_SUCCESS)
    {
        STHROWF("Unable to get meta-data entry data list [{}]", MMDB_strerror(status));
    }
    // Return the resulted list
    return LightObj(SqTypeIdentity< EntryDataList >{}, SqVM(), m_Handle, entry_data_list);
}

// ------------------------------------------------------------------------------------------------
LookupResult Database::LookupString(const SQChar * addr)
{
    // Validate the database handle
    SQMOD_VALIDATE(*this);
    // Validate the specified string
    if (!addr || *addr == '\0')
    {
        STHROWF("Invalid address string");
    }
    // Dummy variables to obtain the status codes
    int gai_error, mmdb_error;
    // Attempt to perform the actual lookup
    MMDB_lookup_result_s result = MMDB_lookup_string(&m_Handle->mDb, addr, &gai_error, &mmdb_error);
    // Validate the result of the getaddrinfo() function call
    if (gai_error != 0)
    {
#if defined(UNICODE) || defined(_UNICODE)
        STHROWF("Unable to resolve address ({}) because [{}]", addr, gai_strerrorA(gai_error));
#else
        STHROWF("Unable to resolve address ({}) because [{}]", addr, gai_strerror(gai_error));
#endif
    }
    // Validate the lookup status code
    else if (mmdb_error != MMDB_SUCCESS)
    {
        STHROWF("Unable to lookup address ({}) because [{}]", addr, MMDB_strerror(mmdb_error));
    }
    // Now it's safe to return the lookup result
    return LookupResult(m_Handle, result);
}

// ------------------------------------------------------------------------------------------------
LookupResult Database::LookupSockAddr(SockAddr & addr)
{
    // Validate the database handle
    SQMOD_VALIDATE(*this);
    // Validate the specified socket address
    if (!addr.IsValid())
    {
        STHROWF("Invalid address instance");
    }
    // Dummy variable to obtain the status codes
    int mmdb_error;
    // Attempt to perform the actual lookup
    MMDB_lookup_result_s result = MMDB_lookup_sockaddr(&m_Handle->mDb, addr.GetHandle()->ai_addr, &mmdb_error);
    // Validate the lookup status code
    if (mmdb_error != MMDB_SUCCESS)
    {
        STHROWF("Unable to lookup address ({}) because [{}]", addr.GetAddress(), MMDB_strerror(mmdb_error));
    }
    // Now it's safe to return the lookup result
    return LookupResult(m_Handle, result);
}

// ------------------------------------------------------------------------------------------------
SearchNode Database::ReadNode(uint32_t node) const
{
    // Validate the database handle
    SQMOD_VALIDATE(*this);
    // Prepare a temporary search node
    MMDB_search_node_s search_node;
    // Attempt to retrieve the requested node from the database
    const int status = MMDB_read_node(&(SQMOD_GET_VALID(*this)->mDb), node, &search_node);
    // Validate the status code
    if (status != MMDB_SUCCESS)
    {
        STHROWF("Unable to get node [{}]", MMDB_strerror(status));
    }
    // Return the resulted list
    return SearchNode(m_Handle, search_node);
}

// ------------------------------------------------------------------------------------------------
SQInteger Description::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMMDescription");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Description::Validate(const SQChar * file, int32_t line) const
{
    if (!m_Handle)
    {
        SqThrowF(SQMOD_RTFMT("Invalid Maxmind database reference =>[{}:{}]"), file, line);
    }
}
#else
void Description::Validate() const
{
    if (!m_Handle)
    {
        SqThrowF(fmt::runtime("Invalid Maxmind database reference"));
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
Description::Pointer Description::GetValid(const SQChar * file, int32_t line) const
{
    Validate(file, line);
    // Validate the referenced description
    if (!m_Description)
    {
        SqThrowF(SQMOD_RTFMT("Invalid Maxmind meta-data description reference =>[{}:{}]"), file, line);
    }
    // Return the description pointer
    return m_Description;
}
#else
Description::Pointer Description::GetValid() const
{
    Validate();
    // Validate the referenced description
    if (!m_Description)
    {
        SqThrowF(fmt::runtime("Invalid Maxmind meta-data description reference"));
    }
    // Return the description pointer
    return m_Description;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
Database Description::GetDatabase() const
{
    return Database(m_Handle);
}

// ------------------------------------------------------------------------------------------------
SQInteger EntryData::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMMEntryData");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void EntryData::Validate(const SQChar * file, int32_t line) const
{
    if (!m_Handle)
    {
        SqThrowF(SQMOD_RTFMT("Invalid Maxmind database reference =>[{}:{}]"), file, line);
    }
}
#else
void EntryData::Validate() const
{
    if (!m_Handle)
    {
        SqThrowF(fmt::runtime("Invalid Maxmind database reference"));
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
EntryData::ConstRef EntryData::GetValid(const SQChar * file, int32_t line) const
{
    Validate(file, line);
    // See if the entry has any data
    if (!m_Entry.has_data)
    {
        SqThrowF(SQMOD_RTFMT("The referenced entry has no data =>[{}:{}]"), file, line);
    }
    // Return the entry
    return m_Entry;
}
#else
EntryData::ConstRef EntryData::GetValid() const
{
    Validate();
    // See if the entry has any data
    if (!m_Entry.has_data)
    {
        SqThrowF(fmt::runtime("The referenced entry has no data"));
    }
    // Return the entry
    return m_Entry;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
EntryData::EntryData()
    : m_Handle(), m_Entry()
{
    std::memset(&m_Entry, 0, sizeof(Type));
}

// ------------------------------------------------------------------------------------------------
void EntryData::Release()
{
    std::memset(&m_Entry, 0, sizeof(Type));
    m_Handle.Reset();
}

// ------------------------------------------------------------------------------------------------
Database EntryData::GetDatabase() const
{
    return Database(m_Handle);
}

// ------------------------------------------------------------------------------------------------
SQInteger EntryDataList::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMMEntryDataList");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void EntryDataList::Validate(const SQChar * file, int32_t line) const
{
    if (!m_Handle)
    {
        SqThrowF(SQMOD_RTFMT("Invalid Maxmind database reference =>[{}:{}]"), file, line);
    }
}
#else
void EntryDataList::Validate() const
{
    if (!m_Handle)
    {
        SqThrowF(fmt::runtime("Invalid Maxmind database reference"));
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
EntryDataList::Pointer EntryDataList::GetValid(const SQChar * file, int32_t line) const
{
    Validate(file, line);
    // Validate the managed list
    if (!m_List)
    {
        SqThrowF(SQMOD_RTFMT("Invalid Maxmind entry data list reference =>[{}:{}]"), file, line);
    }
    // return the list
    return m_List;
}
#else
EntryDataList::Pointer EntryDataList::GetValid() const
{
    Validate();
    // Validate the managed list
    if (!m_List)
    {
        SqThrowF(fmt::runtime("Invalid Maxmind entry data list reference"));
    }
    // return the list
    return m_List;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
EntryDataList::Pointer EntryDataList::GetValidElem(const SQChar * file, int32_t line) const
{
    Validate(file, line);
    // Validate the current element
    if (!m_List)
    {
        SqThrowF(SQMOD_RTFMT("Invalid Maxmind entry data element reference =>[{}:{}]"), file, line);
    }
    // return the element
    return m_Elem;
}
#else
EntryDataList::Pointer EntryDataList::GetValidElem() const
{
    Validate();
    // Validate the current element
    if (!m_List)
    {
        SqThrowF(fmt::runtime("Invalid Maxmind entry data element reference"));
    }
    // return the element
    return m_Elem;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
Database EntryDataList::GetDatabase() const
{
    return Database(m_Handle);
}

// ------------------------------------------------------------------------------------------------
uint32_t EntryDataList::GetCount() const
{
    // Prepare a counter
    uint32_t count = 0;
    // Do we even have a list?
    if (m_List)
    {
        for (Pointer elem = m_List; elem; elem = elem->next)
        {
            ++count;
        }
    }
    // Return the counter
    return count;
}

// ------------------------------------------------------------------------------------------------
bool EntryDataList::Next()
{
    // Attempt to fetch the next element
    m_Elem = SQMOD_GET_VALID(*this) ? m_Elem->next : nullptr;
    // Return whether we have a valid element
    return (m_Elem != nullptr);
}

// ------------------------------------------------------------------------------------------------
bool EntryDataList::Advance(SQInteger n)
{
    // Validate the database handle
    SQMOD_VALIDATE(*this);
    // Attempt to skip as many elements as possible
    while (n && m_Elem)
    {
        // Fetch the next element
        m_Elem = m_Elem->next;
        // Decrease the counter
        --n;
    }
    // Return whether we have a valid element
    return m_Elem;
}

// ------------------------------------------------------------------------------------------------
void EntryDataList::DumpTo(const SQChar * filepath, int32_t indent) const
{
    // Validate the database and list handle
    Pointer ptr = SQMOD_GET_VALID(*this);
    // Validate the specified file path
    if (!filepath || *filepath == '\0')
    {
        STHROWF("Invalid file path");
    }
    // Attempt to open the specified file
    FILE * fp = fopen(filepath, "w");
    // Validate the file handle
    if (!fp)
    {
        STHROWF("Unable to open file {}", filepath);
    }
    // Attempt to dump the entry data list
    int32_t status = MMDB_dump_entry_data_list(fp, ptr, indent);
    // Close the file handle
    fclose(fp);
    // Validate the result of the operation
    if (status != MMDB_SUCCESS)
    {
        STHROWF("Unable to dump the list [{}]", MMDB_strerror(status));
    }
}

// ------------------------------------------------------------------------------------------------
SQInteger LookupResult::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMMLookupResult");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void LookupResult::Validate(const SQChar * file, int32_t line) const
{
    if (!m_Handle)
    {
        SqThrowF(SQMOD_RTFMT("Invalid Maxmind database reference =>[{}:{}]"), file, line);
    }
}
#else
void LookupResult::Validate() const
{
    if (!m_Handle)
    {
        SqThrowF(fmt::runtime("Invalid Maxmind database reference"));
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
const DbRef & LookupResult::GetValid(const SQChar * file, int32_t line) const
{
    Validate(file, line);
    return m_Handle;
}
#else
const DbRef & LookupResult::GetValid() const
{
    Validate();
    return m_Handle;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
LookupResult::LookupResult()
    : m_Handle(), m_Result()
{
    std::memset(&m_Result, 0, sizeof(Type));
}

// ------------------------------------------------------------------------------------------------
void LookupResult::Release()
{
    std::memset(&m_Result, 0, sizeof(Type));
    m_Handle.Reset();
}

// ------------------------------------------------------------------------------------------------
Database LookupResult::GetDatabase() const
{
    return Database(m_Handle);
}

// ------------------------------------------------------------------------------------------------
Object LookupResult::GetEntryDataList()
{
    // See if there's an entry
    if (!m_Result.found_entry)
    {
        STHROWF("Result does not have an entry");
    }

    MMDB_entry_data_list_s * entry_data_list = nullptr;
    // Attempt to retrieve the entire entry data list at once
    const int status = MMDB_get_entry_data_list(&m_Result.entry, &entry_data_list);
    // Validate the status code
    if (status != MMDB_SUCCESS)
    {
        STHROWF("Unable to get entry data list [{}]", MMDB_strerror(status));
    }
    // Return the resulted list
    return Object(new EntryDataList(m_Handle, entry_data_list));
}

// ------------------------------------------------------------------------------------------------
SQInteger LookupResult::GetValue(HSQUIRRELVM vm)
{
    const auto top = sq_gettop(vm);
    // The lookup result instance
    LookupResult * lookup;
    // Attempt to extract the lookup result instance
    try
    {
        lookup = Var< LookupResult * >(vm, 1).value;
    }
    catch (const std::exception & e)
    {
        return sq_throwerror(vm, e.what());
    }
    // Do we have a valid lookup result instance?
    if (!lookup)
    {
        return sq_throwerror(vm, "Invalid lookup result instance");
    }
    // See if there's a handle
    else if (!lookup->m_Handle)
    {
        return sq_throwerror(vm, "Invalid Maxmind database reference");
    }
    // See if there's an entry
    else if (!(lookup->m_Result.found_entry))
    {
        return sq_throwerror(vm, "Result does not have an entry");
    }

    typedef std::vector< StackStrF > ArgList;
    // The list of extracted arguments
    ArgList arglist;
    // Extract each argument as a string
    for (SQInteger i = 2; i <= top; ++i)
    {
        arglist.emplace_back(vm, i);
        // Did we fail to extract the argument value?
        if (SQ_FAILED(arglist.back().Proc(false)))
        {
            return arglist.back().mRes; // Propagate the error
        }
    }

    typedef std::vector< const SQChar * > PtrList;
    // The list of pointers to path segments
    PtrList ptrlist;
    // Grab the pointers to argument values
    for (const auto & a : arglist)
    {
        ptrlist.push_back(a.mPtr);
    }
    // Push null to specify the end of the list
    ptrlist.push_back(nullptr);

    MMDB_entry_data_s entry_data;
    // Attempt to retrieve the specified entry data
    const int status = MMDB_aget_value(&(lookup->m_Result.entry), &entry_data, ptrlist.data());
    // Validate the status code
    if (status != MMDB_SUCCESS)
    {
        return sq_throwerrorf(vm, "Unable to get entry data [%s]", MMDB_strerror(status));
    }
    // Push the resulted list object onto the stack
    try
    {
        ClassType< EntryData >::PushInstance(vm, new EntryData(lookup->m_Handle, entry_data));
    }
    catch (const std::exception & e)
    {
        return sq_throwerror(vm, e.what());
    }
    // Specify that we returned a value
    return 1;
}

// ------------------------------------------------------------------------------------------------
SQInteger Metadata::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMMMetadata");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void Metadata::Validate(const SQChar * file, int32_t line) const
{
    if (!m_Handle)
    {
        SqThrowF(SQMOD_RTFMT("Invalid Maxmind database reference =>[{}:{}]"), file, line);
    }
}
#else
void Metadata::Validate() const
{
    if (!m_Handle)
    {
        SqThrowF(fmt::runtime("Invalid Maxmind database reference"));
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
Metadata::Pointer Metadata::GetValid(const SQChar * file, int32_t line) const
{
    Validate(file, line);
    // Validate the referenced meta-data
    if (!m_Metadata)
    {
        SqThrowF(SQMOD_RTFMT("Invalid Maxmind meta-data reference =>[{}:{}]"), file, line);
    }
    // Return the meta-data pointer
    return m_Metadata;
}
#else
Metadata::Pointer Metadata::GetValid() const
{
    Validate();
    // Validate the referenced meta-data
    if (!m_Metadata)
    {
        SqThrowF(fmt::runtime("Invalid Maxmind meta-data reference"));
    }
    // Return the meta-data pointer
    return m_Metadata;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
Database Metadata::GetDatabase() const
{
    return Database(m_Handle);
}

// ------------------------------------------------------------------------------------------------
Description Metadata::GetDescriptionHandle(uint32_t idx) const
{
    // Validate the specified index
    if (idx > SQMOD_GET_VALID(*this)->description.count)
    {
        STHROWF("The specified description index is out of range: {} > {}", idx, m_Metadata->description.count);
    }
    // Return the requested description
    return Description(m_Handle, m_Metadata->description.descriptions[idx]);
}

// ------------------------------------------------------------------------------------------------
SQInteger SearchNode::Typename(HSQUIRRELVM vm)
{
    static const SQChar name[] = _SC("SqMMSearchNode");
    sq_pushstring(vm, name, sizeof(name));
    return 1;
}

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
void SearchNode::Validate(const SQChar * file, int32_t line) const
{
    if (!m_Handle)
    {
        SqThrowF(SQMOD_RTFMT("Invalid Maxmind database reference =>[{}:{}]"), file, line);
    }
}
#else
void SearchNode::Validate() const
{
    if (!m_Handle)
    {
        SqThrowF(fmt::runtime("Invalid Maxmind database reference"));
    }
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
#if defined(_DEBUG) || defined(SQMOD_EXCEPTLOC)
SearchNode::Reference SearchNode::GetValid(const SQChar * file, int32_t line)
{
    Validate(file, line);
    return m_Node;
}
#else
SearchNode::Reference SearchNode::GetValid()
{
    Validate();
    return m_Node;
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------------
SearchNode::SearchNode()
    : m_Handle(), m_Node()
{
    std::memset(&m_Node, 0, sizeof(Type));
}

// ------------------------------------------------------------------------------------------------
void SearchNode::Release()
{
    std::memset(&m_Node, 0, sizeof(Type));
    m_Handle.Reset();
}

// ------------------------------------------------------------------------------------------------
Database SearchNode::GetDatabase() const
{
    return Database(m_Handle);
}

// ------------------------------------------------------------------------------------------------
Object SearchNode::GetLeftRecordEntryDataList()
{
    // Prepare a temporary entry data list pointer
    MMDB_entry_data_list_s * entry_data_list = nullptr;
    // Attempt to retrieve the entire entry data list at once
    const int status = MMDB_get_entry_data_list(&(SQMOD_GET_VALID(*this).left_record_entry), &entry_data_list);
    // Validate the status code
    if (status != MMDB_SUCCESS)
    {
        STHROWF("Unable to get entry data list [{}]", MMDB_strerror(status));
    }
    // Return the resulted list
    return Object(new EntryDataList(m_Handle, entry_data_list));
}

// ------------------------------------------------------------------------------------------------
Object SearchNode::GetRightRecordEntryDataList()
{
    // Prepare a temporary entry data list pointer
    MMDB_entry_data_list_s * entry_data_list = nullptr;
    // Attempt to retrieve the entire entry data list at once
    const int status = MMDB_get_entry_data_list(&(SQMOD_GET_VALID(*this).right_record_entry), &entry_data_list);
    // Validate the status code
    if (status != MMDB_SUCCESS)
    {
        STHROWF("Unable to get entry data list [{}]", MMDB_strerror(status));
    }
    // Return the resulted list
    return Object(new EntryDataList(m_Handle, entry_data_list));
}

// ------------------------------------------------------------------------------------------------
SQInteger SearchNode::GetRecordEntryData(HSQUIRRELVM vm, bool right)
{
    const auto top = sq_gettop(vm);
    // The search node result instance
    SearchNode * node;
    // Attempt to extract the search node result instance
    try
    {
        node = Var< SearchNode * >(vm, 1).value;
    }
    catch (const std::exception & e)
    {
        return sq_throwerror(vm, e.what());
    }
    // Do we have a valid search node result instance?
    if (!node)
    {
        return sq_throwerror(vm, "Invalid search node result instance");
    }
    // See if there's a handle
    else if (!node->m_Handle)
    {
        return sq_throwerror(vm, "Invalid Maxmind database reference");
    }

    typedef std::vector< StackStrF > ArgList;
    // The list of extracted arguments
    ArgList arglist;
    // Extract each argument as a string
    for (SQInteger i = 2; i <= top; ++i)
    {
        arglist.emplace_back(vm, i);
        // Did we fail to extract the argument value?
        if (SQ_FAILED(arglist.back().Proc(false)))
        {
            return arglist.back().mRes; // Propagate the error
        }
    }

    typedef std::vector< const SQChar * > PtrList;
    // The list of pointers to path segments
    PtrList ptrlist;
    // Grab the pointers to argument values
    for (const auto & a : arglist)
    {
        ptrlist.push_back(a.mPtr);
    }
    // Push null to specify the end of the list
    ptrlist.push_back(nullptr);

    // Grab the requested entry
    MMDB_entry_s * entry = &(right ? node->m_Node.right_record_entry : node->m_Node.left_record_entry);

    MMDB_entry_data_s entry_data;
    // Attempt to retrieve the specified entry data
    const int status = MMDB_aget_value(entry, &entry_data, ptrlist.data());
    // Validate the status code
    if (status != MMDB_SUCCESS)
    {
        return sq_throwerrorf(vm, "Unable to get entry data [%s]", MMDB_strerror(status));
    }
    // Push the resulted list object onto the stack
    try
    {
        ClassType< EntryData >::PushInstance(vm, new EntryData(node->m_Handle, entry_data));
    }
    catch (const std::exception & e)
    {
        return sq_throwerror(vm, e.what());
    }

    // Specify that we returned a value
    return 1;
}

// ================================================================================================
void Register_MMDB(HSQUIRRELVM vm)
{
    Table mmns(vm);

    mmns.Bind(_SC("SockAddr"),
        Class< SockAddr, NoCopy< SockAddr > >(mmns.GetVM(), _SC("SqMMSockAddr"))
        // Constructors
        .Ctor()
        .Ctor< const SQChar * >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SockAddr::Typename)
        .Func(_SC("_tostring"), &SockAddr::ToString)
        // Properties
        .Prop(_SC("IsValid"), &SockAddr::IsValid)
        .Prop(_SC("Address"), &SockAddr::GetAddress)
    );

    mmns.Bind(_SC("Database"),
        Class< Database >(mmns.GetVM(), _SC("SqMMDatabase"))
        // Constructors
        .Ctor()
        .Ctor< StackStrF & >()
        .Ctor< uint32_t, StackStrF & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &Database::Typename)
        .Func(_SC("_tostring"), &Database::ToString)
        // Properties
        .Prop(_SC("IsValid"), &Database::IsValid)
        .Prop(_SC("References"), &Database::GetRefCount)
        .Prop(_SC("Metadata"), &Database::GetMetadata)
        .Prop(_SC("MetadataAsEntryDataList"), &Database::GetMetadataAsEntryDataList)
        // Member methods
        .Func(_SC("Release"), &Database::Release)
        .Func(_SC("LookupString"), &Database::LookupString)
        .Func(_SC("LookupSockAddr"), &Database::LookupSockAddr)
        .Func(_SC("ReadNode"), &Database::ReadNode)
        // Member overloads
        .Overload< void (Database::*)(const SQChar *) >(_SC("Open"), &Database::Open)
        .Overload< void (Database::*)(const SQChar *, uint32_t) >(_SC("Open"), &Database::Open)
    );

    mmns.Bind(_SC("Description"),
        Class< Description >(mmns.GetVM(), _SC("SqMMDescription"))
        // Constructors
        .Ctor()
        .Ctor< const Description & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &Description::Typename)
        .Func(_SC("_tostring"), &Description::ToString)
        // Properties
        .Prop(_SC("IsValid"), &Description::IsValid)
        .Prop(_SC("Database"), &Description::GetDatabase)
        .Prop(_SC("References"), &Description::GetRefCount)
        .Prop(_SC("Value"), &Description::GetDescriptionValue)
        .Prop(_SC("Language"), &Description::GetDescriptionLanguage)
        // Member methods
        .Func(_SC("Release"), &Description::Release)
    );

    mmns.Bind(_SC("EntryData"),
        Class< EntryData >(mmns.GetVM(), _SC("SqMMEntryData"))
        // Constructors
        .Ctor()
        .Ctor< const EntryData & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &EntryData::Typename)
        .Func(_SC("_tostring"), &EntryData::ToString)
        // Properties
        .Prop(_SC("IsValid"), &EntryData::IsValid)
        .Prop(_SC("Database"), &EntryData::GetDatabase)
        .Prop(_SC("References"), &EntryData::GetRefCount)
        .Prop(_SC("TypeName"), &EntryData::TypeName)
        .Prop(_SC("HasData"), &EntryData::HasData)
        .Prop(_SC("Type"), &EntryData::GetType)
        .Prop(_SC("Offset"), &EntryData::GetOffset)
        .Prop(_SC("DataSize"), &EntryData::DataSize)
        .Prop(_SC("String"), &EntryData::GetString)
        .Prop(_SC("Integer"), &EntryData::GetInteger)
        .Prop(_SC("Float"), &EntryData::GetFloat)
        .Prop(_SC("Long"), &EntryData::GetLong)
        .Prop(_SC("Bool"), &EntryData::GetBool)
        .Prop(_SC("Bytes"), &EntryData::GetBytes)
        // Member methods
        .Func(_SC("Release"), &EntryData::Release)
    );

    mmns.Bind(_SC("EntryDataList"),
        Class< EntryDataList, NoCopy< EntryDataList > >(mmns.GetVM(), _SC("SqMMEntryDataList"))
        // Constructors
        .Ctor()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &EntryDataList::Typename)
        .Func(_SC("_tostring"), &EntryDataList::ToString)
        // Properties
        .Prop(_SC("IsValid"), &EntryDataList::IsValid)
        .Prop(_SC("Database"), &EntryDataList::GetDatabase)
        .Prop(_SC("References"), &EntryDataList::GetRefCount)
        .Prop(_SC("HaveElement"), &EntryDataList::HaveElement)
        .Prop(_SC("TypeName"), &EntryDataList::TypeName)
        .Prop(_SC("Count"), &EntryDataList::GetCount)
        .Prop(_SC("HasData"), &EntryDataList::HasData)
        .Prop(_SC("Type"), &EntryDataList::GetType)
        .Prop(_SC("Offset"), &EntryDataList::GetOffset)
        .Prop(_SC("DataSize"), &EntryDataList::DataSize)
        .Prop(_SC("String"), &EntryDataList::GetString)
        .Prop(_SC("Integer"), &EntryDataList::GetInteger)
        .Prop(_SC("Float"), &EntryDataList::GetFloat)
        .Prop(_SC("Long"), &EntryDataList::GetLong)
        .Prop(_SC("Bool"), &EntryDataList::GetBool)
        .Prop(_SC("Bytes"), &EntryDataList::GetBytes)
        // Member methods
        .Func(_SC("Release"), &EntryDataList::Release)
        .Func(_SC("Next"), &EntryDataList::Next)
        .Func(_SC("Advance"), &EntryDataList::Advance)
        .Func(_SC("Reset"), &EntryDataList::Reset)
        // Member Overloads
        .Overload< void (EntryDataList::*)(const SQChar *) const >(_SC("DumpTo"), &EntryDataList::DumpTo)
        .Overload< void (EntryDataList::*)(const SQChar *, int32_t) const >(_SC("DumpTo"), &EntryDataList::DumpTo)
    );

    mmns.Bind(_SC("LookupResult"),
        Class< LookupResult >(mmns.GetVM(), _SC("SqMMLookupResult"))
        // Constructors
        .Ctor()
        .Ctor< const LookupResult & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &LookupResult::Typename)
        .Func(_SC("_tostring"), &LookupResult::ToString)
        // Properties
        .Prop(_SC("IsValid"), &LookupResult::IsValid)
        .Prop(_SC("Database"), &LookupResult::GetDatabase)
        .Prop(_SC("References"), &LookupResult::GetRefCount)
        .Prop(_SC("FoundEntry"), &LookupResult::FoundEntry)
        .Prop(_SC("NetMask"), &LookupResult::GetNetMask)
        .Prop(_SC("EntryDataList"), &LookupResult::GetEntryDataList)
        // Member methods
        .Func(_SC("Release"), &LookupResult::Release)
        // Squirrel functions
        .SquirrelFunc(_SC("GetValue"), &LookupResult::GetValue)
    );

    mmns.Bind(_SC("Metadata"),
        Class< Metadata >(mmns.GetVM(), _SC("SqMMMetadata"))
        // Constructors
        .Ctor()
        .Ctor< const Metadata & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &Metadata::Typename)
        .Func(_SC("_tostring"), &Metadata::ToString)
        // Properties
        .Prop(_SC("IsValid"), &Metadata::IsValid)
        .Prop(_SC("Database"), &Metadata::GetDatabase)
        .Prop(_SC("References"), &Metadata::GetRefCount)
        .Prop(_SC("NodeCount"), &Metadata::GetNodeCount)
        .Prop(_SC("RecordSize"), &Metadata::GetRecordSize)
        .Prop(_SC("IpVersion"), &Metadata::GetIpVersion)
        .Prop(_SC("DatabaseType"), &Metadata::GetDatabaseType)
        .Prop(_SC("LanguageCount"), &Metadata::GetLanguageCount)
        .Prop(_SC("BinaryFormatMajorVersion"), &Metadata::GetBinaryFormatMajorVersion)
        .Prop(_SC("BinaryFormatMinorVersion"), &Metadata::GetBinaryFormatMinorVersion)
        .Prop(_SC("BuildEpoch"), &Metadata::GetBuildEpoch)
        .Prop(_SC("DescriptionCount"), &Metadata::GetDescriptionCount)
        // Member methods
        .Func(_SC("Release"), &Metadata::Release)
        .Func(_SC("GetLanguageName"), &Metadata::GetLanguageName)
        .Func(_SC("GetDescriptionHandle"), &Metadata::GetDescriptionHandle)
        .Func(_SC("GetDescriptionValue"), &Metadata::GetDescriptionValue)
        .Func(_SC("GetDescriptionLanguage"), &Metadata::GetDescriptionLanguage)
    );

    mmns.Bind(_SC("SearchNode"),
        Class< SearchNode >(mmns.GetVM(), _SC("SqMMSearchNode"))
        // Constructors
        .Ctor()
        .Ctor< const SearchNode & >()
        // Meta-methods
        .SquirrelFunc(_SC("_typename"), &SearchNode::Typename)
        .Func(_SC("_tostring"), &SearchNode::ToString)
        // Properties
        .Prop(_SC("IsValid"), &SearchNode::IsValid)
        .Prop(_SC("Database"), &SearchNode::GetDatabase)
        .Prop(_SC("References"), &SearchNode::GetRefCount)
        .Prop(_SC("LeftRecord"), &SearchNode::GetLeftRecord)
        .Prop(_SC("RightRecord"), &SearchNode::GetRightRecord)
        .Prop(_SC("LeftRecordType"), &SearchNode::GetLeftRecordType)
        .Prop(_SC("RightRecordType"), &SearchNode::GetRightRecordType)
        .Prop(_SC("LeftRecordEntryDataList"), &SearchNode::GetLeftRecordEntryDataList)
        .Prop(_SC("RightRecordEntryDataList"), &SearchNode::GetRightRecordEntryDataList)
        // Member methods
        .Func(_SC("Release"), &SearchNode::Release)
        // Squirrel methods
        .SquirrelFunc(_SC("GetLeftRecordValue"), &SearchNode::GetLeftRecordEntryData)
        .SquirrelFunc(_SC("GetRightRecordValue"), &SearchNode::GetRightRecordEntryData)
    );

    mmns.Func(_SC("StrError"), MMDB_strerror);
    mmns.Func(_SC("LibVersion"), MMDB_lib_version);
    mmns.Func(_SC("TypeStr"), AsTypeStr);

    RootTable(vm).Bind(_SC("SqMMDB"), mmns);

    ConstTable(vm).Enum(_SC("SqMMDataType"), Enumeration(vm)
        .Const(_SC("Extended"),     MMDB_DATA_TYPE_EXTENDED)
        .Const(_SC("Pointer"),      MMDB_DATA_TYPE_POINTER)
        .Const(_SC("Utf8String"),   MMDB_DATA_TYPE_UTF8_STRING)
        .Const(_SC("Double"),       MMDB_DATA_TYPE_DOUBLE)
        .Const(_SC("Bytes"),        MMDB_DATA_TYPE_BYTES)
        .Const(_SC("Uint16"),       MMDB_DATA_TYPE_UINT16)
        .Const(_SC("Uint32"),       MMDB_DATA_TYPE_UINT32)
        .Const(_SC("Map"),          MMDB_DATA_TYPE_MAP)
        .Const(_SC("Int32"),        MMDB_DATA_TYPE_INT32)
        .Const(_SC("Uint64"),       MMDB_DATA_TYPE_UINT64)
        .Const(_SC("Uint128"),      MMDB_DATA_TYPE_UINT128)
        .Const(_SC("Array"),        MMDB_DATA_TYPE_ARRAY)
        .Const(_SC("Container"),    MMDB_DATA_TYPE_CONTAINER)
        .Const(_SC("EndMarker"),    MMDB_DATA_TYPE_END_MARKER)
        .Const(_SC("Boolean"),      MMDB_DATA_TYPE_BOOLEAN)
        .Const(_SC("Float"),        MMDB_DATA_TYPE_FLOAT)
    );

    ConstTable(vm).Enum(_SC("SqMMRecordType"), Enumeration(vm)
        .Const(_SC("SearchNode"),   MMDB_RECORD_TYPE_SEARCH_NODE)
        .Const(_SC("Empty"),        MMDB_RECORD_TYPE_EMPTY)
        .Const(_SC("Data"),         MMDB_RECORD_TYPE_DATA)
        .Const(_SC("Invalid"),      MMDB_RECORD_TYPE_INVALID)
    );

    ConstTable(vm).Enum(_SC("SqMMErrCode"), Enumeration(vm)
        .Const(_SC("Success"),                          MMDB_SUCCESS)
        .Const(_SC("FileOpenError"),                    MMDB_FILE_OPEN_ERROR)
        .Const(_SC("CorruptSearchTreeError"),           MMDB_CORRUPT_SEARCH_TREE_ERROR)
        .Const(_SC("InvalidMetadataError"),             MMDB_INVALID_METADATA_ERROR)
        .Const(_SC("IOError"),                          MMDB_IO_ERROR)
        .Const(_SC("OutOfMemoryError"),                 MMDB_OUT_OF_MEMORY_ERROR)
        .Const(_SC("UnknownDatabaseFormatError"),       MMDB_UNKNOWN_DATABASE_FORMAT_ERROR)
        .Const(_SC("InvalidDataError"),                 MMDB_INVALID_DATA_ERROR)
        .Const(_SC("InvalidLookupPathError"),           MMDB_INVALID_LOOKUP_PATH_ERROR)
        .Const(_SC("LookupPathDoesNotMatchDataError"),  MMDB_LOOKUP_PATH_DOES_NOT_MATCH_DATA_ERROR)
        .Const(_SC("InvalidNodeNumberError"),           MMDB_INVALID_NODE_NUMBER_ERROR)
        .Const(_SC("Ipv6LookupInIpv4DatabaseError"),    MMDB_IPV6_LOOKUP_IN_IPV4_DATABASE_ERROR)
    );

    Sqrat::ConstTable(vm)
        .Const(_SC("MMDB_MODE_MMAP"),  MMDB_MODE_MMAP)
        .Const(_SC("MMDB_MODE_MASK"),  MMDB_MODE_MASK);

}

} // Namespace:: SqMod
